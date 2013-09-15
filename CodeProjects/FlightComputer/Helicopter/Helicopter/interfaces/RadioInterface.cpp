/*
 * RadioInterface.cpp
 *
 * Created: 8/25/2013 6:11:13 PM
 *  Author: HP User
 */ 
#include <string.h>

#include "commonheader.h"
#include "RadioInterface.h"

using namespace helicopter::util::common;
using namespace helicopter::interfaces;

void RadioInterface::calculateChecksum (byte *msgPayload, int payloadSize, byte &checksumA, byte &checksumB)
{
	//These fields can overflow, but that is intentional.
	for (int i = 0; i < payloadSize; i++)
	{
		byte b = msgPayload[i];
		
		checksumA = (byte) (checksumA + b);
		checksumB = (byte) (checksumB + checksumA);
	}
}

int RadioInterface::transmit(SystemTelemetryMessage *msgToSend)
{
	int status = 0;
	
	if (msgToSend != NULL)
	{
		byte *msgPayload = msgToSend->getBytes();
		
		int payloadSize = msgToSend->getMessageSize();
		
		int completeMsgSize = payloadSize + 4;;

		byte completeMsg[completeMsgSize];
		
		completeMsg[0] = RadioInterface::SyncByte1;
		completeMsg[1] = RadioInterface::SyncByte2;
		
		memcpy(&completeMsg[2], msgPayload, payloadSize);
		
		byte checksumA = 0;
		byte checksumB = 0;
		
		calculateChecksum(msgPayload, payloadSize, checksumA, checksumB);
		
		completeMsg[completeMsgSize - 2] = checksumA;
		completeMsg[completeMsgSize - 1] = checksumB;
		
		//iterate over the bytes and transmit them, unless there was an error.
		for (int i = 0; i < completeMsgSize && status == 0; i++)
		{
			status = serialDriver->transmitByte(completeMsg[i]);
		}
		
		delete msgPayload;
	}
	
	return status;
}

int RadioInterface::receive(SystemTelemetryMessage * &receivedMessage)
{
	int status = 0;
	
	byte previousByte = 0;
	byte currentByte = 0;
	

	//Read until the sync bytes are received or we time out.
	//Throw away the 'garbage' bytes.
	while(!(previousByte == SyncByte1 && currentByte == SyncByte2) && status == 0)
	{
		previousByte = currentByte;
		status = serialDriver->receiveByte(currentByte);
		//TODO note:
		//While there is a timeout mechnism for waiting on the port, if a huge amount of
		//data arrived very rapidly and this kept processing the data, it would never timeout.
	}
	
	if (status == 0)
	{
		
		//once we have found a valid message, get the message ID
		byte msgType = 0;
		
		status = serialDriver->receiveByte(msgType);
		
		if (status == 0 && msgType == SystemTelemetryMessage::SystemTelemetryMessageType)
		{
			byte messagePayload[SystemTelemetryMessage::MessageSize];
			
			messagePayload[0] = msgType;
			
			
			//skip the first position since thats where the message type is located.
			for (int i = 1; i < SystemTelemetryMessage::MessageSize && status == 0; i++)
			{				
				status = serialDriver->receiveByte(messagePayload[i]);
			}
			
			if (status == 0)
			{
				//Read the two checksum bytes
				byte messageChecksumA = 0;
				byte messageChecksumB = 0;
				byte calculatedChecksumA = 0;
				byte calculatedChecksumB = 0;
							
				//TODO status is overwritten
				status = serialDriver->receiveByte(messageChecksumA);
				status = serialDriver->receiveByte(messageChecksumB);
				
				
				calculateChecksum(messagePayload, SystemTelemetryMessage::MessageSize, calculatedChecksumA, calculatedChecksumB);
				
				//verify that the checksum is correct
				if (calculatedChecksumA == messageChecksumA && calculatedChecksumB == messageChecksumB)
				{
					//build the message
					receivedMessage = SystemTelemetryMessage::buildMessageSt(messagePayload);
				}else
				{
					status = -3;
				}
			}
		}else if (msgType != SystemTelemetryMessage::SystemTelemetryMessageType)
		{
			status = -2;
		}
	}
	
	if (status != 0)
	{
		receivedMessage = NULL;
	}
	
	return status;
}
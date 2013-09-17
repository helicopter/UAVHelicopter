/*
 * RadioInterface.cpp
 *
 * Created: 8/25/2013 6:11:13 PM
 *  Author: HP User
 */ 
#include <string.h>

#include "commonheader.h"
#include "RadioInterface.h"
#include "SystemTelemetryMessage.h"

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

int RadioInterface::transmit(Message *msgToSend)
{
	int status = 0;
	
	if (msgToSend != NULL)
	{
		byte *msgPayload = msgToSend->getBytes();
		
		int payloadSize = msgToSend->getMessageSize();
		
		int completeMsgSize = payloadSize + MsgHeaderFooterSize;;

		byte completeMsg[completeMsgSize];
		
		completeMsg[0] = RadioInterface::SyncByte1;
		completeMsg[1] = RadioInterface::SyncByte2;
		completeMsg[2] = RadioInterface::SyncByte3;
		
		memcpy(&completeMsg[3], msgPayload, payloadSize);
		
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
		
		delete [] msgPayload;
	}
	
	return status;
}

int RadioInterface::receive(Message * &receivedMessage)
{
	int status = 0;
	
    byte firstSyncByte = 0;
    byte secondSyncByte = 0;
    byte thirdSyncByte = 0;
	

	//Read until the sync bytes are received or we time out.
	//Throw away the 'garbage' bytes.
	while(!(firstSyncByte == SyncByte1 && secondSyncByte == SyncByte2 && thirdSyncByte == SyncByte3) && status == 0)
	{
		firstSyncByte = secondSyncByte;
		secondSyncByte = thirdSyncByte;
		status = serialDriver->receiveByte(thirdSyncByte);
		/*previousByte = currentByte;
		status = serialDriver->receiveByte(currentByte);*/
		//TODO note:
		//While there is a timeout mechanism for waiting on the port, if a huge amount of
		//data arrived very rapidly and this kept processing the data, it would never timeout.
	}
	
	if (status == 0)
	{
		
		//once we have found a valid message, get the message ID
		byte msgType = 0;
		
		status = serialDriver->receiveByte(msgType);
		
		if (status == 0)
		{
			int msgSize = 0;
			
			switch(msgType)
			{
				case SystemTelemetryMessage::MessageType:
					msgSize = SystemTelemetryMessage::MessageSize;
					break;
				default:
					//unrecognized message type.
//					status = -2;
status = serialDriver->receiveByte(msgType);
if (status == -1) {break;}else {
					status = msgType;
					break;
}
			}
			
			if (status == 0)
			{
				byte messagePayload[msgSize];
							
				messagePayload[0] = msgType;
							
							
				//skip the first position since thats where the message type is located.
				for (int i = 1; i < msgSize && status == 0; i++)
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
								
								
					calculateChecksum(messagePayload, msgSize, calculatedChecksumA, calculatedChecksumB);
								
					//verify that the checksum is correct
					if (calculatedChecksumA == messageChecksumA && calculatedChecksumB == messageChecksumB)
					{
						//build the message
						switch(msgType)
						{
							case SystemTelemetryMessage::MessageType:
								receivedMessage = SystemTelemetryMessage::buildMessageSt(messagePayload);
							break;
							default:
								//unrecognized message type.
								status = -2;
							break;
						}
					}else
					{
						status = -3;
					}
				}
			}
		}
	}
	
	if (status != 0)
	{
		receivedMessage = NULL;
	}
	
	return status;
}
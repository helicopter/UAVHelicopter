/*
 * RadioInterface.cpp
 *
 * Created: 8/25/2013 6:11:13 PM
 *  Author: HP User
 */ 
#include <string.h>

#include "CommonHeader.h"
#include "GroundControlStationInterface.h"
#include "SystemTelemetryMessage.h"

using namespace helicopter::util;
using namespace helicopter::interfaces;

void GroundControlStationInterface::calculateChecksum (byte *msgPayload, int payloadSize, byte &checksumA, byte &checksumB)
{
	for (int i = 0; i < payloadSize; i++)
	{
		byte b = msgPayload[i];
		
		//These fields can overflow, but that is intentional.
		checksumA = (byte) (checksumA + b);
		checksumB = (byte) (checksumB + checksumA);
	}
}

int GroundControlStationInterface::transmit(Message *msgToSend)
{
	int status = 0;
	
	if (msgToSend != NULL)
	{
		byte *msgPayload = msgToSend->getBytes();
		
		int payloadSize = msgToSend->getMessageSize();
		
		int completeMsgSize = payloadSize + MsgHeaderFooterSize;;

		byte completeMsg[completeMsgSize];
		
		completeMsg[0] = GroundControlStationInterface::SyncByte1;
		completeMsg[1] = GroundControlStationInterface::SyncByte2;
		completeMsg[2] = GroundControlStationInterface::SyncByte3;
		
		memcpy(&completeMsg[3], msgPayload, payloadSize);
		
		byte checksumA = 0;
		byte checksumB = 0;
		
		calculateChecksum(msgPayload, payloadSize, checksumA, checksumB);
		
		completeMsg[completeMsgSize - 2] = checksumA;
		completeMsg[completeMsgSize - 1] = checksumB;
		
		if (enableTimeout)
		{
			timer->startTimer();
		}
		
		//iterate over the bytes and transmit them, unless there was an error.
		for (int i = 0; i < completeMsgSize && status == 0; i++)
		{
			status = serialDriver->transmitByte(completeMsg[i], timer);
		}
		
		timer->stopTimer();
		
		delete [] msgPayload;
		msgPayload = NULL;
	}
	
	return status;
}

/**
 * Note: While receiving a byte, the serial driver may timeout. However, if 
 * a lot of data is constantly received before that timeout, and none of the data
 * is a sync byte, then this method will never stop receiving bytes and will
 * lock up the rest of the system. 
 */
int GroundControlStationInterface::receive(Message * &receivedMessage)
{
	int status = 0;
	
    byte firstSyncByte = 0;
    byte secondSyncByte = 0;
    byte thirdSyncByte = 0;
	
	if (enableTimeout)
	{
		timer->startTimer();
	}

	//Read until the sync bytes are received or we time out.
	//Throw away any 'garbage' bytes.
	while(!(firstSyncByte == SyncByte1 && secondSyncByte == SyncByte2 && thirdSyncByte == SyncByte3) && status == 0)
	{
		firstSyncByte = secondSyncByte;
		secondSyncByte = thirdSyncByte;
		status = serialDriver->receiveByte(thirdSyncByte, timer);
	}
	
	if (status == 0)
	{
		//once we have found a valid message, get the message ID
		byte msgType = 0;
		
		status = serialDriver->receiveByte(msgType, timer);
		
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
					status = -3;
					break;
			}
			
			if (status == 0)
			{
				byte messagePayload[msgSize];
							
				messagePayload[0] = msgType;
							
				//Read the bytes in the payload of the message.
				//skip the first position since thats where the message type is located.
				for (int i = 1; i < msgSize && status == 0; i++)
				{
					status = serialDriver->receiveByte(messagePayload[i], timer);
				}
							
				if (status == 0)
				{
					//Read the two checksum bytes
					byte messageChecksumA = 0;
					byte messageChecksumB = 0;
					byte calculatedChecksumA = 0;
					byte calculatedChecksumB = 0;
								
					status = serialDriver->receiveByte(messageChecksumA, timer);
					status = serialDriver->receiveByte(messageChecksumB, timer);
								
					//Generate checksum for the message			
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
								status = -3;
							break;
						}
					}else
					{
						//checksum mismatch
						status = -4;
					}
				}
			}
		}
	}
	
	if (status != 0)
	{
		receivedMessage = NULL;
	}
	
	timer->stopTimer();
	
	return status;
}
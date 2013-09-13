/*
 * RadioInterface.cpp
 *
 * Created: 8/25/2013 6:11:13 PM
 *  Author: HP User
 */ 
#include "commonheader.h"
#include "RadioInterface.h"

using namespace helicopter::util::common;
using namespace helicopter::interfaces;

int RadioInterface::transmit(SystemTelemetryMessage *msgToSend)
{
	int status = 0;
	
	if (msgToSend != NULL)
	{
		byte *bytesToSend = msgToSend->getBytes();
		
		int numOfBytes = msgToSend->getNumOfBytes();
		
		//iterate over the bytes and transmit them, unless there was an error.
		for (int i = 0; i < numOfBytes && status == 0; i++)
		{
			status = serialDriver->transmitByte(bytesToSend[i]);
		}
		
		delete bytesToSend;
	}
	
	return status;
}

int RadioInterface::receive(SystemTelemetryMessage * &receivedMessage)
{
	byte msgType = 0;
	
	//Get the type of the message to build from the radio
	if (serialDriver->receiveByte(msgType) == 0)
	{
		//Get the number of bytes that should be contained in that message
		//int numOfBytesInMsg = msgBuilder->getNumOfBytesForMessage(msgType);
		int numOfBytesInMsg = 0;
		
		
		if (numOfBytesInMsg > 0)
		{
			//Reset the message builder so it's ready to construct the new message
//			msgBuilder->initialize(numOfBytesInMsg, msgType);
			
			//Start reading the rest of the message. The first byte of the 
			//message was already read (the message type).
			for (int i = 1; i < numOfBytesInMsg; i++)
			{
				byte receivedByte = 0;
				
				//Read the byte from the radio
				if (serialDriver->receiveByte(receivedByte) < 0)
				{
					//msg timed out
					return -1;
				}else
				{
//					msgBuilder->addByte(receivedByte);
				}
			}
			
//			receivedMessage = msgBuilder->buildMessage();
		}else
		{
			//Unknown message type
			return -2;
		}
	}else
	{
		//failed to receive the message type byte / timeout
		return -3;
	}
	
	return 0;
}
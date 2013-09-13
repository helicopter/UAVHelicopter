/*
 * SystemTelemetryMessage.cpp
 *
 * Created: 9/8/2013 7:22:48 PM
 *  Author: HP User
 */ 

#include "SystemTelemetryMessage.h"

#include <string.h>

using namespace helicopter::messages;

static SystemTelemetryMessage buildMessageSt(byte *message);

byte *SystemTelemetryMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	msgPtr[0] = this->msgType;
	
	msgPtr++;
	memcpy(msgPtr, &magX, sizeof(magX));
	
	msgPtr += sizeof(magX);
	memcpy(msgPtr, &magY, sizeof(magY));
	
	msgPtr += sizeof(magY);
	memcpy(msgPtr, &magZ, sizeof(magZ));
	
	return msg;
}

void SystemTelemetryMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		msgType = message[0];
		
		message++;
		memcpy(&magX, message, sizeof(magX));
		
		message += sizeof(magX);
		memcpy(&magY, message, sizeof(magY));
		
		message += sizeof(magY);
		memcpy(&magZ, message, sizeof(magZ));
	}
}

SystemTelemetryMessage* SystemTelemetryMessage::buildMessageSt(byte *message)
{
	SystemTelemetryMessage *msg = new SystemTelemetryMessage();
	msg->buildMessage(message);
	
	return msg;
}
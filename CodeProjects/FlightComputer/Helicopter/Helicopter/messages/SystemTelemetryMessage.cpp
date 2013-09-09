/*
 * SystemTelemetryMessage.cpp
 *
 * Created: 9/8/2013 7:22:48 PM
 *  Author: HP User
 */ 

#include "SystemTelemetryMessage.h"

#include <string.h>

using namespace helicopter::messages;

byte *SystemTelemetryMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	msg[0] = this->msgType;
	
	msg++;
	memcpy(msg, &magX, sizeof(magX));
	
	msg += sizeof(magX);
	memcpy(msg, &magY, sizeof(magY));
	
	msg += sizeof(magY);
	memcpy(msg, &magZ, sizeof(magZ));
	
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
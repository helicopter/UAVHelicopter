/*
 * SimTelemetryMessage.cpp
 *
 * Created: 9/7/2013 12:05:47 PM
 *  Author: HP User
 */ 
#include "SimTelemetryMessage.h"

using namespace helicopter::messages;

byte *SimTelemetryMessage::getBytes()
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
	
void SimTelemetryMessage::buildMessage(byte *message)
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
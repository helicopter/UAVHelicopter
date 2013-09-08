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
	memcpy(&msg[1], &heading, sizeof(heading));
		
	return msg;
}
	
int SimTelemetryMessage::getHeading()
{
	return heading;
}
	
void SimTelemetryMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		msgType = message[0];
			
		//what about the msg type?
		memcpy(&heading, &message[1], sizeof(heading));
	}
}
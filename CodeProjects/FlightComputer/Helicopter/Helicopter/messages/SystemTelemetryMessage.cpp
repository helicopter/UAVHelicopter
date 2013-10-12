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
	byte *msgPtr = msg;
	
	unsigned int temp = 0;
	
	encode (msgPtr, msgType);
	encode (msgPtr, magX);
	encode (msgPtr, magY);
	encode (msgPtr, magZ);
	
	temp = magYaw * 100;
	
	encode (msgPtr, temp);
	encode (msgPtr, timeouts);
	encode (msgPtr, unrecognizedMsgTypes);
	encode (msgPtr, checksumErrors);
				
	return msg;
}

void SystemTelemetryMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		unsigned int temp = 0;
		
		decode (message,msgType);
		decode (message,magX);
		decode (message,magY);
		decode (message,magZ);
		decode (message,temp);
		
		magYaw = temp / 100;
		
		decode (message,timeouts);
		decode (message,unrecognizedMsgTypes);
		decode (message,checksumErrors);
	}
}

SystemTelemetryMessage* SystemTelemetryMessage::buildMessageSt(byte *message)
{
	SystemTelemetryMessage *msg = new SystemTelemetryMessage();
	msg->buildMessage(message);
	
	return msg;
}
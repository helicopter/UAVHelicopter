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
	
	encode (msgPtr, msgType);
	encode (msgPtr, magX);
	encode (msgPtr, magY);
	encode (msgPtr, magZ);
	encode (msgPtr, magYaw);
	encode (msgPtr, yawVelocityDegreesPerSecond);
	encode (msgPtr, yawIntegral);
	encode (msgPtr, yawProportional);
	encode (msgPtr, yawDerivativeError);
	encode (msgPtr, timeouts);
	encode (msgPtr, unrecognizedMsgTypes);
	encode (msgPtr, checksumErrors);
				
	return msg;
}

void SystemTelemetryMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		decode (message,msgType);
		decode (message,magX);
		decode (message,magY);
		decode (message,magZ);
		decode (message,magYaw);
		decode (message,yawVelocityDegreesPerSecond);
		decode (message,yawIntegral);
		decode (message,yawProportional);
		decode (message,yawDerivativeError);						
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
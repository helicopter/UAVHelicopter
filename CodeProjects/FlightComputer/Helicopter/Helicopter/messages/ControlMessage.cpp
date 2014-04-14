/*
 * ControlMessage.cpp
 *
 * Created: 4/12/2014 1:04:54 PM
 *  Author: HP User
 */ 

#include "ControlMessage.h"

using namespace helicopter::messages;

byte *ControlMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	
	encode (msgPtr, msgType);
	encode (msgPtr, MainRotorCollectiveControl);
	encode (msgPtr, YawControl);
	encode (msgPtr, LongitudeControl);
	encode (msgPtr, LateralControl);
	
	
	return msg;
}

void ControlMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		decode (message,msgType);
		decode (message, MainRotorCollectiveControl);
		decode (message, YawControl);
		decode (message, LongitudeControl);
		decode (message, LateralControl);
	}
}

ControlMessage* ControlMessage::buildMessageSt(byte *message)
{
	ControlMessage *msg = new ControlMessage();
	msg->buildMessage(message);
	
	return msg;
}
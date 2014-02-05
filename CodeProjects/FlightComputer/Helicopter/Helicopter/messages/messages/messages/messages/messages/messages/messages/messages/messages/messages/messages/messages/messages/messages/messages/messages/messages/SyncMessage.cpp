/*
 * SyncMessage.cpp
 *
 * Created: 9/15/2013 2:37:16 PM
 *  Author: HP User
 */ 

#include "SyncMessage.h"

#include <string.h>

using namespace helicopter::messages;


byte *SyncMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	msgPtr[0] = this->msgType;
	
	return msg;
}

void SyncMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		msgType = message[0];
	}
}

SyncMessage* SyncMessage::buildMessageSt(byte *message)
{
	SyncMessage *msg = new SyncMessage();
	msg->buildMessage(message);
	
	return msg;
}
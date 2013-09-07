/*
 * MessageBuilder.cpp
 *
 * Created: 8/25/2013 11:59:23 PM
 *  Author: HP User
 */ 
#include "MessageBuilder.h"

using namespace helicopter::messages;

Message *MessageBuilder::buildMessage()
{
	//TODO IMPLEMENT
	//do a bunch of switch statements on which message to build.
	
	return NULL;
}

void MessageBuilder::initialize(int numOfBytesInMessage, byte messageType)
{
	if (internalMessage != NULL)
	{
		delete internalMessage;
		
		internalMessage = NULL;
	}
	
	this->messageType = messageType;
	
	internalMessage = new byte[numOfBytesInMessage];
	
	internalMessageCounter = 0;
	
	internalMessage[internalMessageCounter++] = messageType;
}

void MessageBuilder::addByte(byte byteToAdd)
{
	internalMessage[internalMessageCounter++] = byteToAdd;
}

int MessageBuilder::getNumOfBytesForMessage(byte messageType)
{
	return 0;
}
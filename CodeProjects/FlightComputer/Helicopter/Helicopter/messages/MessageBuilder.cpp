/*
 * MessageBuilder.cpp
 *
 * Created: 8/25/2013 11:59:23 PM
 *  Author: HP User
 */ 
#include "MessageBuilder.h"
#include "SimTelemetryMessage.h"

using namespace helicopter::messages;

Message *MessageBuilder::buildMessage()
{
	Message *message = NULL;
	
	if (messageType == SimTelemetryMessage::SimTelemetryMessageType)
	{
		message = new SimTelemetryMessage();
		
		message->buildMessage(internalMessage);
	}
	//TODO IMPLEMENT
	//do a bunch of switch statements on which message to build.
	
	return message;
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
	if (messageType == SimTelemetryMessage::SimTelemetryMessageType)
	{
		return SimTelemetryMessage::MessageSize;
	}
	return 0;
}
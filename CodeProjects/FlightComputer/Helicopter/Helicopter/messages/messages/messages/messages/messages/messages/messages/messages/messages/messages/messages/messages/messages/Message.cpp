/*
 * Message.cpp
 *
 * Created: 9/15/2013 1:42:54 PM
 *  Author: HP User
 */ 

#include "Message.h"

using namespace helicopter::messages;

/**
	* Returns the number of bytes in this message. This should
	* be the payload of the message, and not underlying protocol
	* information (sync bytes and checksum bytes)
	*/
int Message::getMessageSize()
{
	return msgSize;
}
				
/**
	* Returns the data in this message in raw byte format for transmission.
	*/
byte *Message::getBytes()
{
	return NULL;
}
			
/**
	* Builds a message object from the given bytes. Subclasses should implement this
	* for creating a message object given a stream of bytes.
	* @param message The bytes in the payload of the message which should be used
	* to construct a message object.
	*/
void Message::buildMessage(byte *message)
{
	//noop. subclass should implement this.
}
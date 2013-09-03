/*
 * Message.cpp
 *
 * Created: 8/26/2013 9:03:44 PM
 *  Author: HP User
 */ 

#include "Message.h"

using namespace helicopter::interfaces;

Message::Message (byte msgType, int numOfBytesInMessage):
msgType(msgType),
numBytesInMessage(numOfBytesInMessage)
{
}

/**
	* Returns the bytes in this message as an array of bytes where
	* the number of bytes is equal to whats returned by getNumOfBytes
	*/
byte* Message::getBytes()
{
	return NULL;	
}
				
/**
	* Parses the message's byte array and populates this
	* object with the parsed values
	* @param message the message in byte structure to be parsed
	* to populate this object with data. 
	*/
void Message::buildMessage(byte *message)
{
	
}
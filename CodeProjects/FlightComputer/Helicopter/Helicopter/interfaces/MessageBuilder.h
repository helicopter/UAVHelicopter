/*
 * MessageBuilder.h
 *
 * Created: 8/25/2013 9:24:57 PM
 *  Author: HP User
 */ 


#ifndef MESSAGEBUILDER_H_
#define MESSAGEBUILDER_H_

#include "commonheader.h"
#include "Message.h"

using namespace helicopter::util::common;

namespace helicopter
{
	namespace interfaces
	{
		class MessageBuilder
		{
			protected:
				// Byte structure holding the internal message being built
				byte *internalMessage; 
				
				// Counter to the current location in the byte structure where the next byte
				// will be placed. 
				int internalMessageCounter;
				
				// Type of the message being built.
				byte messageType;
				
			public:
			
				//TODO: Implement destructor. and delete internalMessage
				MessageBuilder(): internalMessage(NULL), internalMessageCounter(0) {}
					
				virtual ~MessageBuilder() {}
				
				/**
				 * TODO: fill in the possible message types and switch to enum
				 * Creates a Message object from the bytes received.
				 * The object that gets created is the MessageType
				 * that was passed in from the initialize method.
				 * 
				 * @return The created message.
				 */
				virtual Message *buildMessage();
				
				/**
				 * Given a message type id, gives the number of bytes
				 * that are in that message type.
				 * @param messageType identifier of the message type
				 */
				virtual int getNumOfBytesForMessage(byte messageType);
				
				/**
				 * TODO: all these message types should probably be switched to enums. 
				 * Resets the internal message structure of the message being built.
				 * Then recreates the internal message structure to the given size.
				 * This should be called in between messages being built. 
				 * I.e. before a message is started to be built, reset should be called.
				 * The first byte in the internal message will be the messageType byte passed
				 * in as a parameter.
				 */
				void initialize(int numOfBytesInMessage, byte messageType);
				
				/**
				 * Adds a byte to the internal message structure being built
				 *
				 * @param byteToAdd the byte to add to the message structure.
				 */
				void addByte(byte byteToAdd);
		};
	}
}



#endif /* MESSAGEBUILDER_H_ */
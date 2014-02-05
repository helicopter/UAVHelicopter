/*
 * Message.h
 *
 * Created: 9/15/2013 1:42:42 PM
 *  Author: HP User
 */ 


#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>

#include "CommonHeader.h"

/**
 * This file represents the a generic message object. Subclasses inherit this class to
 * implement specific messages for specific data requirements.
 */
namespace helicopter
{
	
	namespace messages
	{
		class Message
		{
			protected:
				byte msgType;
				
				int msgSize;
				
				/**
				 * Mem copies the value into the buffer pointer then increments
				 * the pointer location by the size of the value copied.
				 */
				void encode (byte *&buffPtr, long &val)
				{
					memcpy(buffPtr, &val, sizeof(val));
					buffPtr += sizeof(val);
				}
				
				void encode (byte *&buffPtr, float &val)
				{
					memcpy(buffPtr, &val, sizeof(val));
					buffPtr += sizeof(val);
				}				
				
				void encode (byte *&buffPtr, unsigned long &val)
				{
					memcpy(buffPtr, &val, sizeof(val));
					buffPtr += sizeof(val);
				}		
				
				void encode (byte *&buffPtr, byte &val)
				{
					*buffPtr = val;
					buffPtr += sizeof(val);
				}
				
				/**
				 * Copies the the value at the buff pointer location into the given val parameter and
				 * increments the buffer by the sizeof(val)
				 */
				void decode (byte *&buffPtr, byte &val)
				{
					val = *buffPtr;
					buffPtr += sizeof(val);
				}
				
				void decode (byte *&buffPtr, long &val)
				{
					memcpy(&val, buffPtr, sizeof(val));
					buffPtr += sizeof(val);
				}
				
				void decode (byte *&buffPtr, float &val)
				{
					memcpy(&val, buffPtr, sizeof(val));
					buffPtr += sizeof(val);
				}
				
				void decode (byte *&buffPtr, unsigned long &val)
				{
					memcpy(&val, buffPtr, sizeof(val));
					buffPtr += sizeof(val);
				}

			public:
			
				/**
				 * @param msgType The identifier of this message
				 * @param msgSize The size in bytes of this message
				 */
				Message (byte msgType, int msgSize):
					msgType(msgType), msgSize(msgSize)
				{
					
				}
				
				virtual ~Message()
				{
					
				}
			
				/**
				 * Returns the identifier of this message.
				 */
				byte getType()
				{
					return msgType;
				}
			
				/**
				 * Returns the number of bytes in this message. This should
				 * be the payload of the message, and not underlying protocol
				 * information (sync bytes and checksum bytes)
				 */
				int getMessageSize();
				
				/**
				 * Returns the data in this message in raw byte format for transmission.
				 */
				virtual byte *getBytes();
			
				/**
				 * Builds a message object from the given bytes. Subclasses should implement this
				 * for creating a message object given a stream of bytes.
				 * @param message The bytes in the payload of the message which should be used
				 * to construct a message object.
				 */
				virtual void buildMessage(byte *message);
		};
	}
}



#endif /* MESSAGE_H_ */
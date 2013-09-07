/*
 * Message.h
 *
 * Created: 8/25/2013 6:23:44 PM
 *  Author: HP User
 */ 


#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "commonheader.h"

using namespace helicopter::util::common;

namespace helicopter
{
	namespace messages
	{
		/**
		 * Class which represents messages to be transmitted.
		 */
		class Message
		{
			private:
			
			protected:
				byte msgType;
				int numBytesInMessage;
			
			public:
				Message (byte msgType, int numOfBytesInMessage);
					
				virtual ~Message() {}
					
				byte getType() 
				{
					return msgType;
				}
				
				/**
				 * Returns the bytes in this message as an array of bytes where
				 * the number of bytes is equal to whats returned by getNumOfBytes.
				 * The bytes will be on the heap, therefore the caller is responsible for freeing
				 * the data.
				 */
				virtual byte* getBytes();
				
				/**
				 * Parses the message's byte array and populates this
				 * object with the parsed values
				 * @param message the message in byte structure to be parsed
				 * to populate this object with data. 
				 */
				virtual void buildMessage(byte *message);
				
				/**
				 * Returns the number of bytes in the byte array format of this message.
				 */
				int getNumOfBytes()
				{
					return numBytesInMessage;
				}
		};
	}
}



#endif /* MESSAGE_H_ */
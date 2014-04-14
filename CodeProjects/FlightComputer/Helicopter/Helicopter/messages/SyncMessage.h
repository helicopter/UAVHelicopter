/*
 * SynchMessage.h
 *
 * Created: 9/15/2013 1:43:41 PM
 *  Author: HP User
 */ 


#ifndef SYNCMESSAGE_H_
#define SYNCMESSAGE_H_

#include "Message.h"
#include "CommonHeader.h"

/**
 * This message represents a message used to synchronize logic between the flight computer and the
 * ground control station. This can be used to indicate that the flight computer is ready
 * to receive information from the ground control station.
 */
namespace helicopter
{
	
	namespace messages
	{
		class SyncMessage : public Message
		{
			public: 
				static const byte MessageType = 3;
				
				byte RequestedMessage;
							
				static const byte MessageSize =
					sizeof(msgType) + sizeof(RequestedMessage);
							
				SyncMessage(byte requestedMessage): Message(MessageType,MessageSize), RequestedMessage(requestedMessage)
				{
								
				}
				
				~SyncMessage()
				{
					
				}
			
				byte *getBytes();
			
				void buildMessage(byte *message);
			
				/**
				* Static method which creates a message on the heap
				* and then calls buildMessage on that created object.
				*/
				static SyncMessage* buildMessageSt(byte *message);
		};
	}
}




#endif /* SYNCMESSAGE_H_ */
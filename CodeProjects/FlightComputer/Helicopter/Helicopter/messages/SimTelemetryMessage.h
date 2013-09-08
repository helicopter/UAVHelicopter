/*
 * SimTelemetryMessage.h
 *
 * Created: 9/7/2013 11:50:31 AM
 *  Author: HP User
 */ 


#ifndef SIMTELEMETRYMESSAGE_H_
#define SIMTELEMETRYMESSAGE_H_

#include <string.h>

#include "commonheader.h"
#include "Message.h"

using namespace helicopter::util::common;


namespace helicopter
{
	namespace messages
	{

		class SimTelemetryMessage : public Message
		{
			public:
			static const byte SimTelemetryMessageType = 122;
			
			private:
			int heading;
			
			static const byte MessageSize = sizeof(heading) + sizeof(msgType);
			
			public:
			SimTelemetryMessage(): Message(SimTelemetryMessageType, MessageSize)
			{
				
			}
	
			~SimTelemetryMessage() {}
			
			byte *getBytes();
			
			int getHeading();
			
			void buildMessage(byte *message);
			
		};
		
		
	}
	
}



#endif /* SIMTELEMETRYMESSAGE_H_ */
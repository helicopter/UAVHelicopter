/*
 * ControlMessage.h
 *
 * Created: 4/12/2014 1:04:42 PM
 *  Author: HP User
 */ 


#ifndef CONTROLMESSAGE_H_
#define CONTROLMESSAGE_H_



#include "Message.h"
#include "CommonHeader.h"
#include "SystemModel.h"

using namespace helicopter::model;

namespace helicopter
{
	
	namespace messages
	{
		/**
		 * This class represents flight telemetry data from the flight computer.
		 */
		class ControlMessage : public Message
		{
			public:
				static const byte MessageType = 5;

				float MainRotorCollectiveControl;
				float YawControl;
				float LongitudeControl;
				float LateralControl;
				
				
				static const int MessageSize =
				sizeof(msgType) +
				sizeof(MainRotorCollectiveControl) +
				sizeof(YawControl) +
				sizeof(LongitudeControl) +
				sizeof(LateralControl);															
				
				ControlMessage(): Message(MessageType,MessageSize),
					MainRotorCollectiveControl(0),
					YawControl(0),
					LongitudeControl(0),
					LateralControl(0)
				{
					
				}
				
				~ControlMessage() {}
			
				byte *getBytes();
				
				static ControlMessage* buildMessageSt(byte *message);
				
				void buildMessage(byte *message);
		};
	}
}




#endif /* CONTROLMESSAGE_H_ */
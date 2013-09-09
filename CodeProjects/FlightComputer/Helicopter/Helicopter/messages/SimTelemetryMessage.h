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
			private:
			int magX;

			int magY;

			int magZ;
			
			public:
			static const byte SimTelemetryMessageType = 1;
			
			static const byte MessageSize =
			sizeof(msgType) +
			sizeof(magX) +
			sizeof(magY) +
			sizeof(magZ);
						
			SimTelemetryMessage(): Message(SimTelemetryMessageType, MessageSize)
			{
				
			}
	
			~SimTelemetryMessage() {}
			
			byte *getBytes();
			
			void buildMessage(byte *message);
			
			int MagX() const { return magX; }
			void MagX(int val) { magX = val; }
				
			int MagY() const { return magY; }
			void MagY(int val) { magY = val; }
			
			int MagZ() const { return magZ; }
			void MagZ(int val) { magZ = val; }
			
		};
	}
}



#endif /* SIMTELEMETRYMESSAGE_H_ */
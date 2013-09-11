/*
 * SystemTelemetryMessage.h
 *
 * Created: 9/8/2013 7:19:19 PM
 *  Author: HP User
 */ 


#ifndef SYSTEMTELEMETRYMESSAGE_H_
#define SYSTEMTELEMETRYMESSAGE_H_

#include "Message.h"

using namespace helicopter::messages;

/**
 * This file represents the telemetry messages going from the flight computer to the ground station. These
 * are generated by the flight computer.
 */
namespace helicopter
{
	
	namespace messages
	{
		class SystemTelemetryMessage : public Message
		{
			private:
				int magX;

				int magY;

				int magZ;
			
			public:
				static const byte SystemTelemetryMessageType = 2;
			
				static const byte MessageSize =
				sizeof(msgType) +
				sizeof(magX) +
				sizeof(magY) +
				sizeof(magZ);
			
				SystemTelemetryMessage(): Message(SystemTelemetryMessageType, MessageSize),
					magX(0),
					magY(0),
					magZ(0)
				{
				
				}
			
				~SystemTelemetryMessage() {}
			
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


#endif /* SYSTEMTELEMETRYMESSAGE_H_ */
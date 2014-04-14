/*
 * SensorDataMessage.h
 *
 * Created: 4/11/2014 8:38:44 PM
 *  Author: HP User
 */ 


#ifndef SENSORDATAMESSAGE_H_
#define SENSORDATAMESSAGE_H_


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
		class SensorDataMessage : public Message
		{
			public:
				static const byte MessageType = 4;

				float XAccelFrdMss;
				float YAccelFrdMss;
				float ZAccelFrdMss;
				float YawAngularVelocityRadsPerSecond;
				float PitchAngularVelocityRadsPerSecond;
				float RollAngularVelocityRadsPerSecond;
				float XMagFrd;
				float YMagFrd;
				float ZMagFrd;
				long XEcefCm;
				long YEcefCm;
				long ZEcefCm;
				long XVEcefCms;
				long YVEcefCms;
				long ZVEcefCms;
				float PressureMillibars;
				
				
				static const int MessageSize =
				sizeof(msgType) +
				sizeof(XAccelFrdMss) +
				sizeof(YAccelFrdMss) +
				sizeof(ZAccelFrdMss) +
				sizeof(YawAngularVelocityRadsPerSecond) +
				sizeof(PitchAngularVelocityRadsPerSecond) +
				sizeof(RollAngularVelocityRadsPerSecond) +
				sizeof(XMagFrd) +
				sizeof(YMagFrd) +
				sizeof(ZMagFrd) +
				sizeof(XEcefCm) +
				sizeof(YEcefCm) +
				sizeof(ZEcefCm) +
				sizeof(XVEcefCms) +
				sizeof(YVEcefCms) +
				sizeof(ZVEcefCms) +	
				sizeof(PressureMillibars);															
				
				SensorDataMessage(): Message(MessageType,MessageSize),
					XAccelFrdMss(0),
					YAccelFrdMss(0),
					ZAccelFrdMss(0),
					YawAngularVelocityRadsPerSecond(0),
					PitchAngularVelocityRadsPerSecond(0),
					RollAngularVelocityRadsPerSecond(0),
					XMagFrd(0),
					YMagFrd(0),
					ZMagFrd(0),
					XEcefCm(0),
					YEcefCm(0),
					ZEcefCm(0),
					XVEcefCms(0),
					YVEcefCms(0),
					ZVEcefCms(0),
					PressureMillibars(0)
				{
					
				}
				
				~SensorDataMessage() {}
			
				byte *getBytes();
				
				static SensorDataMessage* buildMessageSt(byte *message);
				
				void buildMessage(byte *message);
		};
	}
}



#endif /* SENSORDATAMESSAGE_H_ */
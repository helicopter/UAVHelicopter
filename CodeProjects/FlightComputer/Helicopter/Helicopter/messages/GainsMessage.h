/*
 * GainsMessage.h
 *
 * Created: 5/29/2014 4:39:28 PM
 *  Author: HP User
 */ 


#ifndef GAINSMESSAGE_H_
#define GAINSMESSAGE_H_



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
		class GainsMessage : public Message
		{
			public:

				float YawIntegralGain;
				float YawDerivativeGain;
				float YawProportionalGain;
				float YawAntiWindupGain;

				
				float XIntegralGain;
				float XDerivativeGain;
				float XProportionalGain;
				float XAntiWindupGain;
				float LongitudeInnerLoopGain;
				float PitchAngularVelocityGain;				
				
				
				float YIntegralGain;
				float YDerivativeGain;
				float YProportionalGain;
				float YAntiWindupGain;
				float LateralInnerLoopGain;
				float RollAngularVelocityGain;				
				
				float ZIntegralGain;
				float ZDerivativeGain;
				float ZProportionalGain;
				float ZAntiWindupGain;		
				
				float XRefSetpoint;
				float YRefSetpoint;
				float YawRefSetpoint;		
				
				
				static const byte MessageType = 7;
			
				static const int MessageSize =
				sizeof(msgType) + 
				
				
				
				
				sizeof(YawIntegralGain) +
				sizeof(YawDerivativeGain) +
				sizeof(YawProportionalGain) +
				sizeof(YawAntiWindupGain) +
				
				
				sizeof(XIntegralGain) +
				sizeof(XDerivativeGain) +
				sizeof(XProportionalGain) +
				sizeof(XAntiWindupGain) +
				sizeof(LongitudeInnerLoopGain) +
				sizeof(PitchAngularVelocityGain) +				
				
				sizeof(YIntegralGain) +
				sizeof(YDerivativeGain) +
				sizeof(YProportionalGain) +
				sizeof(YAntiWindupGain) +
				sizeof(LateralInnerLoopGain) +
				sizeof(RollAngularVelocityGain) +				
				
				
				
				
				sizeof(ZIntegralGain) +
				sizeof(ZDerivativeGain) +
				sizeof(ZProportionalGain) +
				sizeof(ZAntiWindupGain) + 
				
				
				sizeof(XRefSetpoint)+
				sizeof(YRefSetpoint) + 
				sizeof(YawRefSetpoint);
			
				GainsMessage(): Message(MessageType, MessageSize),
				
					YawIntegralGain(0),
					YawDerivativeGain(0),
					YawProportionalGain(0),
					YawAntiWindupGain(0),
					
					
					XIntegralGain(0),
					XDerivativeGain(0),
					XProportionalGain(0),
					XAntiWindupGain(0),
					LongitudeInnerLoopGain(0),
					PitchAngularVelocityGain(0),
					
					
					YIntegralGain(0),
					YDerivativeGain(0),
					YProportionalGain(0),
					YAntiWindupGain(0),
					LateralInnerLoopGain(0),
					RollAngularVelocityGain(0),
					
					
					ZIntegralGain(0),
					ZDerivativeGain(0),
					ZProportionalGain(0),
					ZAntiWindupGain(0),
					
					XRefSetpoint(0),
					YRefSetpoint(0),
					YawRefSetpoint(0)
				{
				
				}
			
				~GainsMessage() {}
			
				byte *getBytes();
				
				void buildMessage(byte *message);
				
					
				//Static method which creates a telemetry message on the heap
				//and then calls buildMessage on that created object.
				static GainsMessage* buildMessageSt(byte *message);
				
		};
	}
}



#endif /* GAINSMESSAGE_H_ */
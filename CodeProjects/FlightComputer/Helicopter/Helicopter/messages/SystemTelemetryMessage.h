/*
 * SystemTelemetryMessage.h
 *
 * Created: 9/8/2013 7:19:19 PM
 *  Author: HP User
 */ 


#ifndef SYSTEMTELEMETRYMESSAGE_H_
#define SYSTEMTELEMETRYMESSAGE_H_

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
		class SystemTelemetryMessage : public Message
		{
			public:

				float MagYaw;
				float YawVelocityDegreesPerSecond;
				
				
				float YawIntegral;
				float YawProportional;
				float YawDerivativeError;
				float YawControl;
				float YawIntegralGain;
				float YawDerivativeGain;
				float YawProportionalGain;
				float YawAntiWindupGain;
				
				
				float XNEDBodyFrame;
				float XVelocityMetersPerSecond;
				float ThetaPitchDegrees;
				float XIntegral;
				float XProportional;
				float XDerivativeError;
				float LongitudeControl;
				float XIntegralGain;
				float XDerivativeGain;
				float XProportionalGain;
				float XAntiWindupGain;
				float LongitudeInnerLoopGain;
				float XLongitudeOuterLoopSetpoint;
				float PitchAngularVelocityRadsPerSecond;
				float PitchAngularVelocityGain;
				
				float YNEDBodyFrame;
				float YVelocityMetersPerSecond;
				float PhiRollDegrees;
				float YIntegral;
				float YProportional;
				float YDerivativeError;
				float LateralControl;
				float YIntegralGain;
				float YDerivativeGain;
				float YProportionalGain;
				float YAntiWindupGain;
				float LateralInnerLoopGain;
				float YLateralOuterLoopSetpoint;
				float RollAngularVelocityRadsPerSecond;
				float RollAngularVelocityGain;

				float AltitudeFeetAgl;
				float ZVelocityMetersPerSecond;
				float ZIntegral;
				float ZProportional;
				float ZDerivativeError;
				float MainRotorCollectiveControl;
				float ZIntegralGain;
				float ZDerivativeGain;
				float ZProportionalGain;
				float ZAntiWindupGain;
				float ZNEDBodyFrame;
										
				float LatitudeDegrees;
				float LongitudeDegrees;								
				
				/**
				 * Instrumentation fields
				 */
				long Timeouts;
				
				long UnrecognizedMsgTypes;
				
				long ChecksumErrors;
				
				long NumOfBlownFrames;
				
				

				
				
				
				static const byte MessageType = 2;
			
				static const byte MessageSize =
				sizeof(msgType) +
				sizeof(MagYaw) +
				sizeof(YawVelocityDegreesPerSecond) + 
				sizeof(YawIntegral) +
				sizeof(YawProportional) +
				sizeof(YawDerivativeError) +
				sizeof(YawControl) +
				sizeof(YawIntegralGain) +
				sizeof(YawDerivativeGain) +
				sizeof(YawProportionalGain) +
				sizeof(YawAntiWindupGain) +
				
				
				sizeof(XNEDBodyFrame) +
				sizeof(XVelocityMetersPerSecond) +
				sizeof(ThetaPitchDegrees) +
				sizeof(XIntegral) +
				sizeof(XProportional) +
				sizeof(XDerivativeError) +
				sizeof(LongitudeControl) +
				sizeof(XIntegralGain) +
				sizeof(XDerivativeGain) +
				sizeof(XProportionalGain) +
				sizeof(XAntiWindupGain) +
				sizeof(LongitudeInnerLoopGain) +
				sizeof(XLongitudeOuterLoopSetpoint) +
				sizeof(PitchAngularVelocityRadsPerSecond) +
				sizeof(PitchAngularVelocityGain) +
				
				
				sizeof(YNEDBodyFrame) +
				sizeof(YVelocityMetersPerSecond) +
				sizeof(PhiRollDegrees) +
				sizeof(YIntegral) +
				sizeof(YProportional) +
				sizeof(YDerivativeError) +
				sizeof(LateralControl) +
				sizeof(YIntegralGain) +
				sizeof(YDerivativeGain) +
				sizeof(YProportionalGain) +
				sizeof(YAntiWindupGain) +
				sizeof(LateralInnerLoopGain) +
				sizeof(YLateralOuterLoopSetpoint)+
				sizeof(RollAngularVelocityRadsPerSecond) +
				sizeof(RollAngularVelocityGain) +
								
				sizeof(AltitudeFeetAgl) +
				sizeof(ZVelocityMetersPerSecond) +
				sizeof(ZIntegral) +
				sizeof(ZProportional) +
				sizeof(ZDerivativeError) +
				sizeof(MainRotorCollectiveControl) +
				sizeof(ZIntegralGain) +
				sizeof(ZDerivativeGain) +
				sizeof(ZProportionalGain) +
				sizeof(ZAntiWindupGain) +
				sizeof(ZNEDBodyFrame) + 
				
				sizeof(LatitudeDegrees) +
				sizeof(LongitudeDegrees) +
				
				sizeof(Timeouts) + 
				sizeof(UnrecognizedMsgTypes) + 
				sizeof(ChecksumErrors) + 
				sizeof(NumOfBlownFrames);

			
				SystemTelemetryMessage(): Message(MessageType,MessageSize),
				
				
					MagYaw(0),
					YawVelocityDegreesPerSecond(0),
					YawIntegral(0),
					YawProportional(0),
					YawDerivativeError(0),
					YawControl(0),	
					YawIntegralGain(0),
					YawDerivativeGain(0),
					YawProportionalGain(0),
					YawAntiWindupGain(0),
					
					
					XNEDBodyFrame(0),
					XVelocityMetersPerSecond(0),
					ThetaPitchDegrees(0),
					XIntegral(0),
					XProportional(0),
					XDerivativeError(0),
					LongitudeControl(0),
					XIntegralGain(0),
					XDerivativeGain(0),
					XProportionalGain(0),
					XAntiWindupGain(0),
					LongitudeInnerLoopGain(0),
					XLongitudeOuterLoopSetpoint(0),
					PitchAngularVelocityRadsPerSecond(0),
					PitchAngularVelocityGain(0),
					
					YNEDBodyFrame(0),
					YVelocityMetersPerSecond(0),
					PhiRollDegrees(0),
					YIntegral(0),
					YProportional(0),
					YDerivativeError(0),
					LateralControl(0),
					YIntegralGain(0),
					YDerivativeGain(0),
					YProportionalGain(0),
					YAntiWindupGain(0),
					LateralInnerLoopGain(0),
					YLateralOuterLoopSetpoint(0),
					RollAngularVelocityRadsPerSecond(0),
					RollAngularVelocityGain(0),
					
					AltitudeFeetAgl(0),
					ZVelocityMetersPerSecond(0),
					ZIntegral(0),
					ZProportional(0),
					ZDerivativeError(0),
					MainRotorCollectiveControl(0),
					ZIntegralGain(0),
					ZDerivativeGain(0),
					ZProportionalGain(0),
					ZAntiWindupGain(0),
					ZNEDBodyFrame(0),
					
					LatitudeDegrees(0),
					LongitudeDegrees(0),
										
														
					Timeouts(0),
					UnrecognizedMsgTypes(0),
					ChecksumErrors(0),
					NumOfBlownFrames(0)
				{
				
				}
			
				~SystemTelemetryMessage() {}
			
				byte *getBytes();
				
				void buildMessage(byte *message);
				
				/**
				 * Updates the given model using the values in this telemetry message
				 */
				void updateModelFromMessage (SystemModel *model);
				
				/**
				 * Updates the given model using the values in this telemetry message
				 * which was received from the simulator. This method will only update
				 * the relevant fields that were transmitted by the simulator, and not
				 * all the fields. 
				 */
				void updateModelFromMessageFromSimulator (SystemModel *model);
					
				//Static method which creates a telemetry message on the heap
				//and then calls buildMessage on that created object.
				static SystemTelemetryMessage* buildMessageSt(byte *message);
				
				/**
				 * Updates this telemetry message from the model.
				 */
				static SystemTelemetryMessage* buildMessageFromModel( SystemModel *model );
		};
	}
}


#endif /* SYSTEMTELEMETRYMESSAGE_H_ */
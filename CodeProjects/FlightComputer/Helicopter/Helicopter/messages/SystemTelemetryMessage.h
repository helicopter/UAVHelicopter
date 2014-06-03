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

				float YawIntegral;
				float YawProportional;
				float YawDerivativeError;
				float YawControl;
				float YawIntegralGain;
				float YawDerivativeGain;
				float YawProportionalGain;
				float YawAntiWindupGain;
				
				
				float XNEDLocalFrame;
				float XVelocityFRDCms;
				float PitchRads;
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
				float PitchAngularVelocityGain;
				
				float YNEDLocalFrame;
				float YVelocityFRDCms;
				float RollRads;
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
				float RollAngularVelocityGain;




				float AltitudeMetersAgl;
				float ZVelocityFRDCms;
				float YawRads;
				float ZIntegral;
				float ZProportional;
				float ZDerivativeError;
				float MainRotorCollectiveControl;
				float ZIntegralGain;
				float ZDerivativeGain;
				float ZProportionalGain;
				float ZAntiWindupGain;
				float ZNEDLocalFrame;
										
				float LatitudeDegrees;
				float LongitudeDegrees;	
				
				
				
				/**
				 * Sensor readings
				 */
				
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
				

				float XRefSetpoint;
				float YRefSetpoint;
				float YawRefSetpoint;
				
																		
				
				/**
				 * Instrumentation fields
				 */
				long Timeouts;
				
				long UnrecognizedMsgTypes;
				
				long ChecksumErrors;
				
				long NumOfBlownFrames;
				
				long SerialCommunicationBufferOverruns;

				
				
				
				static const byte MessageType = 2;
			
				static const int MessageSize =
				sizeof(msgType) + 
				sizeof(YawIntegral) +
				sizeof(YawProportional) +
				sizeof(YawDerivativeError) +
				sizeof(YawControl) +
				sizeof(YawIntegralGain) +
				sizeof(YawDerivativeGain) +
				sizeof(YawProportionalGain) +
				sizeof(YawAntiWindupGain) +
				
				
				sizeof(XNEDLocalFrame) +
				sizeof(XVelocityFRDCms) +
				sizeof(PitchRads) +
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
				sizeof(PitchAngularVelocityGain) +
				
				
				sizeof(YNEDLocalFrame) +
				sizeof(YVelocityFRDCms) +
				sizeof(RollRads) +
				sizeof(YIntegral) +
				sizeof(YProportional) +
				sizeof(YDerivativeError) +
				sizeof(LateralControl) +
				sizeof(YIntegralGain) +
				sizeof(YDerivativeGain) +
				sizeof(YProportionalGain) +
				sizeof(YAntiWindupGain) +
				sizeof(LateralInnerLoopGain) +
				sizeof(YLateralOuterLoopSetpoint) +
				sizeof(RollAngularVelocityGain) +
								
				sizeof(AltitudeMetersAgl) +
				sizeof(ZVelocityFRDCms) +
				sizeof(YawRads) +
				sizeof(ZIntegral) +
				sizeof(ZProportional) +
				sizeof(ZDerivativeError) +
				sizeof(MainRotorCollectiveControl) +
				sizeof(ZIntegralGain) +
				sizeof(ZDerivativeGain) +
				sizeof(ZProportionalGain) +
				sizeof(ZAntiWindupGain) +
				sizeof(ZNEDLocalFrame) + 
				
				sizeof(LatitudeDegrees) +
				sizeof(LongitudeDegrees) +
				
				
				
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
				sizeof(PressureMillibars) +		


				sizeof(XRefSetpoint)+
				sizeof(YRefSetpoint) +
				sizeof(YawRefSetpoint) +				
				
				sizeof(Timeouts) + 
				sizeof(UnrecognizedMsgTypes) + 
				sizeof(ChecksumErrors) + 
				sizeof(NumOfBlownFrames) + 
				sizeof(SerialCommunicationBufferOverruns);

			
				SystemTelemetryMessage(): Message(MessageType,MessageSize),
				
					YawIntegral(0),
					YawProportional(0),
					YawDerivativeError(0),
					YawControl(0),	
					YawIntegralGain(0),
					YawDerivativeGain(0),
					YawProportionalGain(0),
					YawAntiWindupGain(0),
					
					
					XNEDLocalFrame(0),
					XVelocityFRDCms(0),
					PitchRads(0),
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
					PitchAngularVelocityGain(0),
					
					YNEDLocalFrame(0),
					YVelocityFRDCms(0),
					RollRads(0),
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
					RollAngularVelocityGain(0),
					
					AltitudeMetersAgl(0),
					ZVelocityFRDCms(0),
					YawRads(0),
					ZIntegral(0),
					ZProportional(0),
					ZDerivativeError(0),
					MainRotorCollectiveControl(0),
					ZIntegralGain(0),
					ZDerivativeGain(0),
					ZProportionalGain(0),
					ZAntiWindupGain(0),
					ZNEDLocalFrame(0),
					
					LatitudeDegrees(0),
					LongitudeDegrees(0),
							
							
							
							
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
					PressureMillibars(0),							
							
							
							
							
						XRefSetpoint(0),
						YRefSetpoint(0),
						YawRefSetpoint(0),						
							
							
										
														
					Timeouts(0),
					UnrecognizedMsgTypes(0),
					ChecksumErrors(0),
					NumOfBlownFrames(0), 
					SerialCommunicationBufferOverruns(0)
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
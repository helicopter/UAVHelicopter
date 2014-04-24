/*
 * SimpleTelemetry.h
 *
 * Created: 4/19/2014 8:05:40 PM
 *  Author: HP User
 */ 


#ifndef SIMPLETELEMETRYMESSAGE_H_
#define SIMPLETELEMETRYMESSAGE_H_



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
		class SimpleTelemetryMessage : public Message
		{
			public:

				float XNEDLocalFrame;
				float XVelocityFRDCms;
				float PitchRads;
				
				
				float YNEDLocalFrame;
				float YVelocityFRDCms;
				float RollRads;
				

				float ZVelocityFRDCms;
				float YawRads;
				float ZNEDLocalFrame;
									
				
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
				
														
				
				/**
				 * Instrumentation fields
				 */
				long Timeouts;
				
				long UnrecognizedMsgTypes;
				
				long ChecksumErrors;
				
				long NumOfBlownFrames;
				
				long SerialCommunicationBufferOverruns;

				
				
				
				static const byte MessageType = 6;
			
				static const int MessageSize =
				sizeof(msgType) + 
				
				
				
				
				sizeof(XNEDLocalFrame) +
				sizeof(XVelocityFRDCms) +
				sizeof(PitchRads) +
				
				
				
				sizeof(YNEDLocalFrame) +
				sizeof(YVelocityFRDCms) +
				sizeof(RollRads) +
				
								
				sizeof(ZVelocityFRDCms) +
				sizeof(YawRads) +
				sizeof(ZNEDLocalFrame) +
				
				
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
				
				
				sizeof(Timeouts) + 
				sizeof(UnrecognizedMsgTypes) + 
				sizeof(ChecksumErrors) + 
				sizeof(NumOfBlownFrames) + 
				sizeof(SerialCommunicationBufferOverruns);

			
				SimpleTelemetryMessage(): Message(MessageType,MessageSize),
				
					XNEDLocalFrame(0),
					XVelocityFRDCms(0),
					PitchRads(0),
					
					
					YNEDLocalFrame(0),
					YVelocityFRDCms(0),
					RollRads(0),
					
					
					ZVelocityFRDCms(0),
					YawRads(0),
					ZNEDLocalFrame(0),
					
					
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
							
							
							
							
							
							
							
										
														
					Timeouts(0),
					UnrecognizedMsgTypes(0),
					ChecksumErrors(0),
					NumOfBlownFrames(0), 
					SerialCommunicationBufferOverruns(0)
				{
				
				}
			
				~SimpleTelemetryMessage() {}
			
				byte *getBytes();
				
				void buildMessage(byte *message);
				
					
				//Static method which creates a telemetry message on the heap
				//and then calls buildMessage on that created object.
				static SimpleTelemetryMessage* buildMessageSt(byte *message);
				
		};
	}
}



#endif /* SIMPLETELEMETRYMESSAGE_H_ */
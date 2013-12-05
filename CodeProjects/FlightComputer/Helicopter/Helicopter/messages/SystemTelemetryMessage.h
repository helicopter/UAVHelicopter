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

				unsigned long MagYaw;
				
				long YawVelocityDegreesPerSecond;
				
				/**
				 * Control output fields
				 */
				
				long YawIntegral;
				
				long YawProportional;
				
				long YawDerivativeError;
				
				long YawControl;
				
				/**
				 * Gains used for PID controller
				 */
				long YawIntegralGain;
				long YawDerivativeGain;
				long YawProportionalGain;
				long YawAntiWindupGain;
				
				
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
				sizeof(Timeouts) + 
				sizeof(UnrecognizedMsgTypes) + 
				sizeof(ChecksumErrors) + 
				sizeof(NumOfBlownFrames);
;
			
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
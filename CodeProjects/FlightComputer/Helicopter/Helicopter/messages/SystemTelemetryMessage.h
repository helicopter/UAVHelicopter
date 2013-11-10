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

namespace helicopter
{
	
	namespace messages
	{
		/**
		 * This class represents flight telemetry data from the flight computer.
		 */
		class SystemTelemetryMessage : public Message
		{
			private:
				int magX;

				int magY;

				int magZ;
				
				unsigned int magYaw;
				
				int yawVelocityDegreesPerSecond;
				
				int yawIntegral;
				
				int yawProportional;
				
				int yawDerivativeError;
				
				/**
				 * Instrumentation fields
				 */
				int timeouts;
				
				int unrecognizedMsgTypes;
				
				int checksumErrors;
				
			public:
				static const byte MessageType = 2;
			
				static const byte MessageSize =
				sizeof(msgType) +
				sizeof(magX) +
				sizeof(magY) +
				sizeof(magZ) + 
				sizeof(magYaw) +
				sizeof(yawVelocityDegreesPerSecond) + 
				sizeof(yawIntegral) +
				sizeof(yawProportional) +
				sizeof(yawDerivativeError) +
				sizeof(timeouts) + 
				sizeof(unrecognizedMsgTypes) + 
				sizeof(checksumErrors);
			
				SystemTelemetryMessage(): Message(MessageType,MessageSize),
					magX(0),
					magY(0),
					magZ(0),
					magYaw(0),
					yawVelocityDegreesPerSecond(0),
					yawIntegral(0),
					yawProportional(0),
					yawDerivativeError(0),					
					timeouts(0),
					unrecognizedMsgTypes(0),
					checksumErrors(0)
				{
				
				}
			
				~SystemTelemetryMessage() {}
			
				/**
				 * Magnetic heading variables in the x y and z directions.
				 */
				
				int MagX() const { return magX; }
				void MagX(int val) { magX = val; }
			
				int MagY() const { return magY; }
				void MagY(int val) { magY = val; }
			
				int MagZ() const { return magZ; }
				void MagZ(int val) { magZ = val; }
					
				unsigned int MagYaw() const { return magYaw; }
				void MagYaw(unsigned int val) { magYaw = val; }
					
				int YawVelocityDegreesPerSecond() const { return yawVelocityDegreesPerSecond; }
				void YawVelocityDegreesPerSecond(int val) { yawVelocityDegreesPerSecond = val; }
					
				int YawIntegral() const {return yawIntegral;}
				void YawIntegral(int val) { yawIntegral = val;}
					
				int YawProportional() const {return yawProportional;}
				void YawProportional(int val) { yawProportional = val;}

				int YawDerivativeError() const {return yawDerivativeError;}
				void YawDerivativeError(int val) { yawDerivativeError = val;}	
					
									
					
				int Timeouts() const {return timeouts; }
				void Timeouts(int val) { timeouts = val; }
				
				int UnrecognizedMsgTypes() const {return unrecognizedMsgTypes; }
				void UnrecognizedMsgTypes(int val) { unrecognizedMsgTypes = val; }
				
				int ChecksumErrors() const {return checksumErrors; }
				void ChecksumErrors(int val) { checksumErrors = val; }
					
					
				byte *getBytes();
				
				void buildMessage(byte *message);
					
				//Static method which creates a telemetry message on the heap
				//and then calls buildMessage on that created object.
				static SystemTelemetryMessage* buildMessageSt(byte *message);
		};
	}
}


#endif /* SYSTEMTELEMETRYMESSAGE_H_ */
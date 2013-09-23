/*
 * SystemModel.h
 *
 * Created: 9/8/2013 4:57:07 PM
 *  Author: HP User
 */ 


#ifndef SYSTEMMODEL_H_
#define SYSTEMMODEL_H_

#include "SystemTelemetryMessage.h"

using namespace helicopter::messages;

namespace helicopter
{
	namespace model
	{
		
		/**
		 * This class represents the model of the flight computer. This contains the 
		 * current representation of various points of data used throughout the system.
		 */
		class SystemModel
		{
			private:
			int rawMagX;
			
			int rawMagY;
			
			int rawMagZ;
			
			int magYaw;
			
			// Instrumentation fields
			int timeouts;
			
			int unrecognizedMsgTypes;
			
			int checksumErrors;
			
			public:
			
			SystemModel():
			rawMagX(0),
			rawMagY(0),
			rawMagZ(0),
			magYaw(0),
			timeouts(0),
			unrecognizedMsgTypes(0),
			checksumErrors(0)
			{
				
			}
			
			/**
			 * RawMag values represents the raw magnetic heading values received from
			 * the sensors in the x y and z axis, in the following frame of reference:
			 * X axis is left out of the FC
			 * Y axis is pointing behind the FC
			 * Z axis is pointing up out of the FC
			 */
			
			int RawMagX() const { return rawMagX; }
			void RawMagX(int val) { rawMagX = val; }
			
			int RawMagY() const { return rawMagY; }
			void RawMagY(int val) { rawMagY = val; }
			
			int RawMagZ() const { return rawMagZ; }
			void RawMagZ(int val) { rawMagZ = val; }
				

			int MagYaw() const { return magYaw; }
			void MagYaw(int val) { magYaw = val; }				
						
			int Timeouts() const {return timeouts; }
			void Timeouts(int val) { timeouts = val; }
			
			int UnrecognizedMsgTypes() const {return unrecognizedMsgTypes; }
			void UnrecognizedMsgTypes(int val) { unrecognizedMsgTypes = val; }
			
			int ChecksumErrors() const {return checksumErrors; }
			void ChecksumErrors(int val) { checksumErrors = val; }
				
			SystemTelemetryMessage *CreateTelemetryMessage();
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */
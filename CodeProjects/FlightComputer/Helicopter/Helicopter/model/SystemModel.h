/*
 * SystemModel.h
 *
 * Created: 9/8/2013 4:57:07 PM
 *  Author: HP User
 */ 


#ifndef SYSTEMMODEL_H_
#define SYSTEMMODEL_H_


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
			
			double magYawDegrees;
			
			double referenceMagYawDegrees;
			
			double yawControl;
			
			//This variable is used for calculating integral anti-windup.
			double yawControlBeforeServoLimitsAdjustment;
			
			double yawVelocityDegreesPerSecond;
			
			double referenceYawVelocityDegreesPerSecond;
			
			double yawIntegral;
			
			double yawProportional;
			
			double yawDerivativeError;
			
			
			
			// Instrumentation fields
			long timeouts;
			
			long unrecognizedMsgTypes;
			
			long checksumErrors;
			
			long numOfFramesBlown;
			
			public:
			
			SystemModel():
			magYawDegrees(0),
			referenceMagYawDegrees(0),
			yawControl(0),
			yawControlBeforeServoLimitsAdjustment(0),
			yawVelocityDegreesPerSecond(0),
			referenceYawVelocityDegreesPerSecond(0),
			yawIntegral(0),
			yawProportional(0),
			yawDerivativeError(0),
			timeouts(0),
			unrecognizedMsgTypes(0),
			checksumErrors(0),
			numOfFramesBlown(0)
			{
				
			}
			
			/**
			 * RawMag values represents the raw magnetic heading values received from
			 * the sensors in the x y and z axis, in the following frame of reference:
			 * X axis is left out of the FC
			 * Y axis is pointing behind the FC
			 * Z axis is pointing up out of the FC
			 */
			

			double MagYawDegrees() const { return magYawDegrees; }
			void MagYawDegrees(double val) { magYawDegrees = val; }	
				
				
				
			double ReferenceMagYawDegrees() const {return referenceMagYawDegrees;}
			void ReferenceMagYawDegrees(double val) { referenceMagYawDegrees = val;}
				
				
			double YawControl() const {return yawControl;}
			void YawControl(double val) { yawControl = val;}
				
			double YawControlBeforeServoLimitsAdjustment() const {return yawControlBeforeServoLimitsAdjustment;}
			void YawControlBeforeServoLimitsAdjustment(double val) { yawControlBeforeServoLimitsAdjustment = val;}
				
			double YawVelocityDegreesPerSecond() const {return yawVelocityDegreesPerSecond;}
			void YawVelocityDegreesPerSecond(double val) { yawVelocityDegreesPerSecond = val;}
				
			double ReferenceYawVelocityDegreesPerSecond() const {return referenceYawVelocityDegreesPerSecond;}
			void ReferenceYawVelocityDegreesPerSecond(double val) { referenceYawVelocityDegreesPerSecond = val;}
				
			double YawIntegral() const {return yawIntegral;}
			void YawIntegral(double val) { yawIntegral = val;}

			double YawProportional() const {return yawProportional;}
			void YawProportional(double val) { yawProportional = val;}

			double YawDerivativeError() const {return yawDerivativeError;}
			void YawDerivativeError(double val) { yawDerivativeError = val;}								
																							
						
			long Timeouts() const {return timeouts; }
			void Timeouts(long val) { timeouts = val; }
			
			long UnrecognizedMsgTypes() const {return unrecognizedMsgTypes; }
			void UnrecognizedMsgTypes(long val) { unrecognizedMsgTypes = val; }
			
			long ChecksumErrors() const {return checksumErrors; }
			void ChecksumErrors(long val) { checksumErrors = val; }
			
			long  BlownFrames() const {return numOfFramesBlown;}
			void BlownFrames( long val ) { numOfFramesBlown = val;}
			
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */
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
			
			/**
			 * These variables are used for controlling Yaw action (tail rotor)
			 */
			double magYawDegrees;
			double referenceMagYawDegrees;
			double yawControl;
			double yawControlBeforeServoLimitsAdjustment;
			double yawVelocityDegreesPerSecond;
			double referenceYawVelocityDegreesPerSecond;
			double yawIntegral;
			double yawProportional;
			double yawDerivativeError;
			
			/**
			 * These variables are used for controlling the longitudinal action (cyclic pitch motion)
			 */
			double xNEDBodyFrame;
			double referenceXNEDBodyFrame;
			double longitudeControlBeforeServoLimitsAdjustment;
			double longitudeControl;
			double xVelocityMetersPerSecond;
			double referenceXVelocityMetersPerSecond;
			double xLongitudinalOuterLoopControl;
			double xProportional;
			double xIntegral;
			double xDerivativeError;
			double thetaPitchDegrees;
			
			/**
			 * These variables are used for controlling the lateral action (cyclic roll motion)
			 */
			double yNEDBodyFrame;
			double referenceYNEDBodyFrame;
			double lateralControlBeforeServoLimitsAdjustment;
			double lateralControl;
			double yVelocityMetersPerSecond;
			double referenceYVelocityMetersPerSecond;
			double yLateralOuterLoopControl;
			double yProportional;
			double yIntegral;
			double yDerivativeError;
			double phiRollDegrees;
			
			/**
			 * These variables are used for controlling heave action (collective altitude motion)
			 */
			double altitudeFeet;
			double zNEDBodyFrame;
			double referenceAltitudeFeet;
			double mainRotorControl;
			double mainRotorControlBeforeServoLimitsAdjustment;
			double zVelocityFeetPerSecond;
			double referenceZVelocityFeetPerSecond;
			double zIntegral;
			double zProportional;
			double zDerivativeError;
			
			/**
			 * These variables represent the position of the aircraft in curvilinear coordinates
			 */
			double latitudeDegrees;
			double longitudeDegrees;
			
			/**
			 * Instrumentation fields
			 */
			long timeouts;
			
			long unrecognizedMsgTypes;
			
			long checksumErrors;
			
			long numOfFramesBlown;
			
			
			/**
			 * Initial position
			 */
			double initialXPositionEcef;
			double initialYPositionEcef;
			double initialZPositionEcef;
			
			public:
			
			double EcefToLocalNEDRotationMatrix[3][3];
			
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
			
				xNEDBodyFrame(0),
				referenceXNEDBodyFrame(0),
				longitudeControlBeforeServoLimitsAdjustment(0),
				longitudeControl(0),
				xVelocityMetersPerSecond(0),
				referenceXVelocityMetersPerSecond(0),
				xLongitudinalOuterLoopControl(0),
				xProportional(0),
				xIntegral(0),
				xDerivativeError(0),
				thetaPitchDegrees(0),
			
				yNEDBodyFrame(0),
				referenceYNEDBodyFrame(0),
				lateralControlBeforeServoLimitsAdjustment(0),
				lateralControl(0),
				yVelocityMetersPerSecond(0),
				referenceYVelocityMetersPerSecond(0),
				yLateralOuterLoopControl(0),
				yProportional(0),
				yIntegral(0),
				yDerivativeError(0),
				phiRollDegrees(0),
			
			
				altitudeFeet(0),
				zNEDBodyFrame(0),
				referenceAltitudeFeet(0),
				mainRotorControl(0),
				mainRotorControlBeforeServoLimitsAdjustment(0),
				zVelocityFeetPerSecond(0),
				referenceZVelocityFeetPerSecond(0),
				zIntegral(0),
				zProportional(0),
				zDerivativeError(0),
			
				latitudeDegrees(0),
				longitudeDegrees(0),
			
				timeouts(0),
				unrecognizedMsgTypes(0),
				checksumErrors(0),
				numOfFramesBlown(0),
				
				initialXPositionEcef(0),
				initialYPositionEcef(0),
				initialZPositionEcef(0)
			{
				EcefToLocalNEDRotationMatrix[0][0] = 0;
				EcefToLocalNEDRotationMatrix[0][1] = 0;
				EcefToLocalNEDRotationMatrix[0][2] = 0;
				EcefToLocalNEDRotationMatrix[1][0] = 0;
				EcefToLocalNEDRotationMatrix[1][1] = 0;
				EcefToLocalNEDRotationMatrix[1][2] = 0;
				EcefToLocalNEDRotationMatrix[2][0] = 0;
				EcefToLocalNEDRotationMatrix[2][1] = 0;
				EcefToLocalNEDRotationMatrix[2][2] = 0;								
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
																							
		
		
		
			double XNEDBodyFrame() const {return xNEDBodyFrame;}
			void XNEDBodyFrame(double val) { xNEDBodyFrame = val;}
						
			double ReferenceXNEDBodyFrame() const {return referenceXNEDBodyFrame;}
			void ReferenceXNEDBodyFrame(double val) { referenceXNEDBodyFrame = val;}

			double LongitudeControlBeforeServoLimitsAdjustment() const {return longitudeControlBeforeServoLimitsAdjustment;}
			void LongitudeControlBeforeServoLimitsAdjustment(double val) { longitudeControlBeforeServoLimitsAdjustment = val;}

			double LongitudeControl() const {return longitudeControl;}
			void LongitudeControl(double val) { longitudeControl = val;}
				
			double XVelocityMetersPerSecond() const {return xVelocityMetersPerSecond;}
			void XVelocityMetersPerSecond(double val) { xVelocityMetersPerSecond = val;}
				
			double ReferenceXVelocityMetersPerSecond() const {return referenceXVelocityMetersPerSecond;}
			void ReferenceXVelocityMetersPerSecond(double val) { referenceXVelocityMetersPerSecond = val;}
				
			double XLongitudinalOuterLoopControl() const {return xLongitudinalOuterLoopControl;}
			void XLongitudinalOuterLoopControl(double val) { xLongitudinalOuterLoopControl = val;}
				
			double XProportional() const {return xProportional;}
			void XProportional(double val) { xProportional = val;}
				
			double XIntegral() const {return xIntegral;}
			void XIntegral(double val) { xIntegral = val;}
				
			double XDerivativeError() const {return xDerivativeError;}
			void XDerivativeError(double val) { xDerivativeError = val;}
				
		
			double ThetaPitchDegrees() const {return thetaPitchDegrees;}
			void ThetaPitchDegrees(double val) {thetaPitchDegrees = val;}
		
		
		
						
						
						
						
						


			double YNEDBodyFrame() const {return yNEDBodyFrame;}
			void YNEDBodyFrame(double val) { yNEDBodyFrame = val;}

			double ReferenceYNEDBodyFrame() const {return referenceYNEDBodyFrame;}
			void ReferenceYNEDBodyFrame(double val) { referenceYNEDBodyFrame = val;}

			double LateralControlBeforeServoLimitsAdjustment() const {return lateralControlBeforeServoLimitsAdjustment;}
			void LateralControlBeforeServoLimitsAdjustment(double val) { lateralControlBeforeServoLimitsAdjustment = val;}

			double LateralControl() const {return lateralControl;}
			void LateralControl(double val) { lateralControl = val;}

			double YVelocityMetersPerSecond() const {return yVelocityMetersPerSecond;}
			void YVelocityMetersPerSecond(double val) { yVelocityMetersPerSecond = val;}

			double ReferenceYVelocityMetersPerSecond() const {return referenceYVelocityMetersPerSecond;}
			void ReferenceYVelocityMetersPerSecond(double val) { referenceYVelocityMetersPerSecond = val;}

			double YLateralOuterLoopControl() const {return yLateralOuterLoopControl;}
			void YLateralOuterLoopControl(double val) { yLateralOuterLoopControl = val;}

			double YProportional() const {return yProportional;}
			void YProportional(double val) { yProportional = val;}

			double YIntegral() const {return yIntegral;}
			void YIntegral(double val) { yIntegral = val;}

			double YDerivativeError() const {return yDerivativeError;}
			void YDerivativeError(double val) { yDerivativeError = val;}


			double PhiRollDegrees() const {return phiRollDegrees;}
			void PhiRollDegrees(double val) {phiRollDegrees = val;}


						
						
						
						

			double AltitudeFeet() const { return altitudeFeet; }
			void AltitudeFeet(double val) { altitudeFeet = val; }

			double ZNEDBodyFrame() const { return zNEDBodyFrame; }
			void ZNEDBodyFrame(double val) { zNEDBodyFrame = val; }


			double ReferenceAltitudeFeet() const {return referenceAltitudeFeet;}
			void ReferenceAltitudeFeet(double val) { referenceAltitudeFeet = val;}


			double MainRotorCollectiveControl() const {return mainRotorControl;}
			void MainRotorCollectiveControl(double val) { mainRotorControl = val;}

			double MainRotorControlBeforeServoLimitsAdjustment() const {return mainRotorControlBeforeServoLimitsAdjustment;}
			void MainRotorControlBeforeServoLimitsAdjustment(double val) { mainRotorControlBeforeServoLimitsAdjustment = val;}

			double ZVelocityFeetPerSecond() const {return zVelocityFeetPerSecond;}
			void ZVelocityFeetPerSecond(double val) { zVelocityFeetPerSecond = val;}

			double ReferenceZVelocityFeetPerSecond() const {return referenceZVelocityFeetPerSecond;}
			void ReferenceZVelocityFeetPerSecond(double val) { referenceZVelocityFeetPerSecond = val;}

			double ZIntegral() const {return zIntegral;}
			void ZIntegral(double val) { zIntegral = val;}

			double ZProportional() const {return zProportional;}
			void ZProportional(double val) { zProportional = val;}

			double ZDerivativeError() const {return zDerivativeError;}
			void ZDerivativeError(double val) { zDerivativeError = val;}						
						
						
			double LatitudeDegrees() const {return latitudeDegrees;}
			void LatitudeDegrees(double val) { latitudeDegrees = val;}						

			double LongitudeDegrees() const {return longitudeDegrees;}
			void LongitudeDegrees(double val) { longitudeDegrees = val;}						
						
						
			long Timeouts() const {return timeouts; }
			void Timeouts(long val) { timeouts = val; }
			
			long UnrecognizedMsgTypes() const {return unrecognizedMsgTypes; }
			void UnrecognizedMsgTypes(long val) { unrecognizedMsgTypes = val; }
			
			long ChecksumErrors() const {return checksumErrors; }
			void ChecksumErrors(long val) { checksumErrors = val; }
			
			long  BlownFrames() const {return numOfFramesBlown;}
			void BlownFrames( long val ) { numOfFramesBlown = val;}

			long  InitialXPositionEcef() const {return initialXPositionEcef;}
			void InitialXPositionEcef( long val ) { initialXPositionEcef = val;}			

			long  InitialYPositionEcef() const {return initialYPositionEcef;}
			void InitialYPositionEcef( long val ) { initialYPositionEcef = val;}

			long  InitialZPositionEcef() const {return initialZPositionEcef;}
			void InitialZPositionEcef( long val ) { initialZPositionEcef = val;}							
			/*
			double **  EcefToLocalNEDRotationMatrix() const {return ecefToLocalNEDRotationMatrix;}
			void EcefToLocalNEDRotationMatrix( long val ) { ecefToLocalNEDRotationMatrix = val;}*/
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */
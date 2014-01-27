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
			public: 
			
			/**
			 * Enum representing whether the helicopter is being controlled
			 * by a radio transmitter (ManualControl) or by the PID algorithm (AutoPilot).
			 */
			enum OperatingState {ManualControl, AutoPilot};
				
			private:
			OperatingState operationalState;
			
			/**
			 * These variables are used for controlling Yaw action (tail rotor)
			 */
			float magYawDegrees;
			float referenceMagYawDegrees;
			float yawControl;
			float yawControlBeforeServoLimitsAdjustment;
			float yawVelocityDegreesPerSecond;
			float referenceYawVelocityDegreesPerSecond;
			float yawIntegral;
			float yawProportional;
			float yawDerivativeError;
			
			/**
			 * These variables are used for controlling the longitudinal action (cyclic pitch motion)
			 */
			float xNEDBodyFrame;
			float referenceXNEDBodyFrame;
			float longitudeControlBeforeServoLimitsAdjustment;
			float longitudeControl;
			float xVelocityMetersPerSecond;
			float referenceXVelocityMetersPerSecond;
			float xLongitudeOuterLoopSetpoint;
			float xProportional;
			float xIntegral;
			float xDerivativeError;
			float thetaPitchDegrees;
			float pitchAngularVelocityRadsPerSecond;
			
			/**
			 * These variables are used for controlling the lateral action (cyclic roll motion)
			 */
			float yNEDBodyFrame;
			float referenceYNEDBodyFrame;
			float lateralControlBeforeServoLimitsAdjustment;
			float lateralControl;
			float yVelocityMetersPerSecond;
			float referenceYVelocityMetersPerSecond;
			float yLateralOuterLoopSetpoint;
			float yProportional;
			float yIntegral;
			float yDerivativeError;
			float phiRollDegrees;
			float rollAngularVelocityRadsPerSecond;
			
			/**
			 * These variables are used for controlling heave action (collective altitude motion)
			 */
			float altitudeFeetAgl;
			float zNEDBodyFrame;
			float referenceAltitudeFeet;
			float mainRotorControl;
			float mainRotorControlBeforeServoLimitsAdjustment;
			float zVelocityFeetPerSecond;
			float referenceZVelocityFeetPerSecond;
			float zIntegral;
			float zProportional;
			float zDerivativeError;
			
			/**
			 * These variables represent the position of the aircraft in curvilinear coordinates
			 */
			float latitudeDegrees;
			float longitudeDegrees;
			
			/**
			 * Instrumentation fields
			 */
			long timeouts;
			
			long unrecognizedMsgTypes;
			
			long checksumErrors;
			
			long numOfFramesBlown;
			
			long serialCommunicationBufferOverruns;
			
			/**
			 * Initial position
			 */
			float initialXPositionEcef;
			float initialYPositionEcef;
			float initialZPositionEcef;
			
			public:
			
			float EcefToLocalNEDRotationMatrix[3][3];
			
			SystemModel():
				operationalState(ManualControl),
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
				xLongitudeOuterLoopSetpoint(0),
				xProportional(0),
				xIntegral(0),
				xDerivativeError(0),
				thetaPitchDegrees(0),
				pitchAngularVelocityRadsPerSecond(0),
			
				yNEDBodyFrame(0),
				referenceYNEDBodyFrame(0),
				lateralControlBeforeServoLimitsAdjustment(0),
				lateralControl(0),
				yVelocityMetersPerSecond(0),
				referenceYVelocityMetersPerSecond(0),
				yLateralOuterLoopSetpoint(0),
				yProportional(0),
				yIntegral(0),
				yDerivativeError(0),
				phiRollDegrees(0),
				rollAngularVelocityRadsPerSecond(0),
			
			
				altitudeFeetAgl(0),
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
				serialCommunicationBufferOverruns(0),
				
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
			

			float MagYawDegrees() const { return magYawDegrees; }
			void MagYawDegrees(float val) { magYawDegrees = val; }	
				
				
				
			float ReferenceMagYawDegrees() const {return referenceMagYawDegrees;}
			void ReferenceMagYawDegrees(float val) { referenceMagYawDegrees = val;}
				
				
			float YawControl() const {return yawControl;}
			void YawControl(float val) { yawControl = val;}
				
			float YawControlBeforeServoLimitsAdjustment() const {return yawControlBeforeServoLimitsAdjustment;}
			void YawControlBeforeServoLimitsAdjustment(float val) { yawControlBeforeServoLimitsAdjustment = val;}
				
			float YawVelocityDegreesPerSecond() const {return yawVelocityDegreesPerSecond;}
			void YawVelocityDegreesPerSecond(float val) { yawVelocityDegreesPerSecond = val;}
				
			float ReferenceYawVelocityDegreesPerSecond() const {return referenceYawVelocityDegreesPerSecond;}
			void ReferenceYawVelocityDegreesPerSecond(float val) { referenceYawVelocityDegreesPerSecond = val;}
				
			float YawIntegral() const {return yawIntegral;}
			void YawIntegral(float val) { yawIntegral = val;}

			float YawProportional() const {return yawProportional;}
			void YawProportional(float val) { yawProportional = val;}

			float YawDerivativeError() const {return yawDerivativeError;}
			void YawDerivativeError(float val) { yawDerivativeError = val;}								
																							
		
		
		
			float XNEDBodyFrame() const {return xNEDBodyFrame;}
			void XNEDBodyFrame(float val) { xNEDBodyFrame = val;}
						
			float ReferenceXNEDBodyFrame() const {return referenceXNEDBodyFrame;}
			void ReferenceXNEDBodyFrame(float val) { referenceXNEDBodyFrame = val;}

			float LongitudeControlBeforeServoLimitsAdjustment() const {return longitudeControlBeforeServoLimitsAdjustment;}
			void LongitudeControlBeforeServoLimitsAdjustment(float val) { longitudeControlBeforeServoLimitsAdjustment = val;}

			float LongitudeControl() const {return longitudeControl;}
			void LongitudeControl(float val) { longitudeControl = val;}
				
			float XVelocityMetersPerSecond() const {return xVelocityMetersPerSecond;}
			void XVelocityMetersPerSecond(float val) { xVelocityMetersPerSecond = val;}
				
			float ReferenceXVelocityMetersPerSecond() const {return referenceXVelocityMetersPerSecond;}
			void ReferenceXVelocityMetersPerSecond(float val) { referenceXVelocityMetersPerSecond = val;}
				
			float XLongitudeOuterLoopSetpoint() const {return xLongitudeOuterLoopSetpoint;}
			void XLongitudeOuterLoopSetpoint(float val) { xLongitudeOuterLoopSetpoint = val;}
				
			float XProportional() const {return xProportional;}
			void XProportional(float val) { xProportional = val;}
				
			float XIntegral() const {return xIntegral;}
			void XIntegral(float val) { xIntegral = val;}
				
			float XDerivativeError() const {return xDerivativeError;}
			void XDerivativeError(float val) { xDerivativeError = val;}
				
		
			float ThetaPitchDegrees() const {return thetaPitchDegrees;}
			void ThetaPitchDegrees(float val) {thetaPitchDegrees = val;}
		
			float PitchAngularVelocityRadsPerSecond() const {return pitchAngularVelocityRadsPerSecond;}
			void PitchAngularVelocityRadsPerSecond(float val) {pitchAngularVelocityRadsPerSecond = val;}
		
						
						
						
						
						


			float YNEDBodyFrame() const {return yNEDBodyFrame;}
			void YNEDBodyFrame(float val) { yNEDBodyFrame = val;}

			float ReferenceYNEDBodyFrame() const {return referenceYNEDBodyFrame;}
			void ReferenceYNEDBodyFrame(float val) { referenceYNEDBodyFrame = val;}

			float LateralControlBeforeServoLimitsAdjustment() const {return lateralControlBeforeServoLimitsAdjustment;}
			void LateralControlBeforeServoLimitsAdjustment(float val) { lateralControlBeforeServoLimitsAdjustment = val;}

			float LateralControl() const {return lateralControl;}
			void LateralControl(float val) { lateralControl = val;}

			float YVelocityMetersPerSecond() const {return yVelocityMetersPerSecond;}
			void YVelocityMetersPerSecond(float val) { yVelocityMetersPerSecond = val;}

			float ReferenceYVelocityMetersPerSecond() const {return referenceYVelocityMetersPerSecond;}
			void ReferenceYVelocityMetersPerSecond(float val) { referenceYVelocityMetersPerSecond = val;}

			float YLateralOuterLoopSetpoint() const {return yLateralOuterLoopSetpoint;}
			void YLateralOuterLoopSetpoint(float val) { yLateralOuterLoopSetpoint = val;}

			float YProportional() const {return yProportional;}
			void YProportional(float val) { yProportional = val;}

			float YIntegral() const {return yIntegral;}
			void YIntegral(float val) { yIntegral = val;}

			float YDerivativeError() const {return yDerivativeError;}
			void YDerivativeError(float val) { yDerivativeError = val;}


			float PhiRollDegrees() const {return phiRollDegrees;}
			void PhiRollDegrees(float val) {phiRollDegrees = val;}

			float RollAngularVelocityRadsPerSecond() const {return rollAngularVelocityRadsPerSecond;}
			void RollAngularVelocityRadsPerSecond(float val) {rollAngularVelocityRadsPerSecond = val;}

						
						
						
						

			float AltitudeFeetAgl() const { return altitudeFeetAgl; }
			void AltitudeFeetAgl(float val) { altitudeFeetAgl = val; }

			float ZNEDBodyFrame() const { return zNEDBodyFrame; }
			void ZNEDBodyFrame(float val) { zNEDBodyFrame = val; }


			float ReferenceZNEDBodyFrameFeet() const {return referenceAltitudeFeet;}
			void ReferenceZNEDBodyFrameFeet(float val) { referenceAltitudeFeet = val;}


			float MainRotorCollectiveControl() const {return mainRotorControl;}
			void MainRotorCollectiveControl(float val) { mainRotorControl = val;}

			float MainRotorControlBeforeServoLimitsAdjustment() const {return mainRotorControlBeforeServoLimitsAdjustment;}
			void MainRotorControlBeforeServoLimitsAdjustment(float val) { mainRotorControlBeforeServoLimitsAdjustment = val;}

			float ZVelocityFeetPerSecond() const {return zVelocityFeetPerSecond;}
			void ZVelocityFeetPerSecond(float val) { zVelocityFeetPerSecond = val;}

			float ReferenceZVelocityFeetPerSecond() const {return referenceZVelocityFeetPerSecond;}
			void ReferenceZVelocityFeetPerSecond(float val) { referenceZVelocityFeetPerSecond = val;}

			float ZIntegral() const {return zIntegral;}
			void ZIntegral(float val) { zIntegral = val;}

			float ZProportional() const {return zProportional;}
			void ZProportional(float val) { zProportional = val;}

			float ZDerivativeError() const {return zDerivativeError;}
			void ZDerivativeError(float val) { zDerivativeError = val;}						
						
						
			float LatitudeDegrees() const {return latitudeDegrees;}
			void LatitudeDegrees(float val) { latitudeDegrees = val;}						

			float LongitudeDegrees() const {return longitudeDegrees;}
			void LongitudeDegrees(float val) { longitudeDegrees = val;}						
						
						
			long Timeouts() const {return timeouts; }
			void Timeouts(long val) { timeouts = val; }
			
			long UnrecognizedMsgTypes() const {return unrecognizedMsgTypes; }
			void UnrecognizedMsgTypes(long val) { unrecognizedMsgTypes = val; }
			
			long ChecksumErrors() const {return checksumErrors; }
			void ChecksumErrors(long val) { checksumErrors = val; }
			
			long BlownFrames() const {return numOfFramesBlown;}
			void BlownFrames( long val ) { numOfFramesBlown = val;}
				
			long SerialCommunicationBufferOverruns() const {return serialCommunicationBufferOverruns;}
			void SerialCommunicationBufferOverruns( long val ) { serialCommunicationBufferOverruns = val;}				

			long  InitialXPositionEcef() const {return initialXPositionEcef;}
			void InitialXPositionEcef( long val ) { initialXPositionEcef = val;}			

			long  InitialYPositionEcef() const {return initialYPositionEcef;}
			void InitialYPositionEcef( long val ) { initialYPositionEcef = val;}

			long  InitialZPositionEcef() const {return initialZPositionEcef;}
			void InitialZPositionEcef( long val ) { initialZPositionEcef = val;}			
			/*
			float **  EcefToLocalNEDRotationMatrix() const {return ecefToLocalNEDRotationMatrix;}
			void EcefToLocalNEDRotationMatrix( long val ) { ecefToLocalNEDRotationMatrix = val;}*/
				
			OperatingState  OperationalState() const {return operationalState;}
			void OperationalState( OperatingState val ) { operationalState = val;}
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */
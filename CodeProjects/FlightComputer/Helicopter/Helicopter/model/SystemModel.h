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
			
			float referenceMagYawRads;
			float yawControl;
			float yawControlBeforeServoLimitsAdjustment;
			float referenceYawVelocityRadsPerSecond;
			float yawIntegral;
			float yawProportional;
			float yawDerivativeError;
			
			/**
			 * These variables are used for controlling the longitudinal action (cyclic pitch motion)
			 */
			float xNEDLocalFrame;
			float referenceXNEDLocalFrame;
			float longitudeControlBeforeServoLimitsAdjustment;
			float longitudeControl;
			float xVelocityMetersPerSecond;
			float referenceXVelocityMetersPerSecond;
			float xLongitudeOuterLoopSetpoint;
			float xProportional;
			float xIntegral;
			float xDerivativeError;
			float thetaPitchDegrees;

			
			/**
			 * These variables are used for controlling the lateral action (cyclic roll motion)
			 */
			float yNEDLocalFrame;
			float referenceYNEDLocalFrame;
			float lateralControlBeforeServoLimitsAdjustment;
			float lateralControl;
			float yVelocityMetersPerSecond;
			float referenceYVelocityMetersPerSecond;
			float yLateralOuterLoopSetpoint;
			float yProportional;
			float yIntegral;
			float yDerivativeError;
			float phiRollDegrees;
			
			/**
			 * These variables are used for controlling heave action (collective altitude motion)
			 */
			float altitudeMetersAgl;
			float zNEDLocalFrame;
			float referenceAltitudeMeters;
			float mainRotorControl;
			float mainRotorControlBeforeServoLimitsAdjustment;
			float zVelocityMetersPerSecond;
			float referenceZVelocityMetersPerSecond;
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
			float initialAltitudeMeters;
			
			
			float rollRads;
			float pitchRads;
			float yawRads;
			
			/**
			 * Sensor Readings
			 */
			float pitchAngularVelocityRs;
			float rollAngularVelocityRs;
			float yawAngularVelocityRs;
			float xAccelFrdMss;
			float yAccelFrdMss;
			float zAccelFrdMss;
			float xMagFrd;
			float yMagFrd;
			float zMagFrd;
			long xEcefCm;
			long yEcefCm;
			long zEcefCm;		
			long xVEcefCms;
			long yVEcefCms;
			long zVEcefCms;
			float pressureMillibars;
			
			public:
			
			float EcefToLocalNEDRotationMatrix[3][3];
			
			SystemModel():
				operationalState(ManualControl),
				referenceMagYawRads(0),
				yawControl(0),
				yawControlBeforeServoLimitsAdjustment(0),
				referenceYawVelocityRadsPerSecond(0),
				yawIntegral(0),
				yawProportional(0),
				yawDerivativeError(0),
			
				xNEDLocalFrame(0),
				referenceXNEDLocalFrame(0),
				longitudeControlBeforeServoLimitsAdjustment(0),
				longitudeControl(0),
				xVelocityMetersPerSecond(0),
				referenceXVelocityMetersPerSecond(0),
				xLongitudeOuterLoopSetpoint(0),
				xProportional(0),
				xIntegral(0),
				xDerivativeError(0),
				thetaPitchDegrees(0),
			
				yNEDLocalFrame(0),
				referenceYNEDLocalFrame(0),
				lateralControlBeforeServoLimitsAdjustment(0),
				lateralControl(0),
				yVelocityMetersPerSecond(0),
				referenceYVelocityMetersPerSecond(0),
				yLateralOuterLoopSetpoint(0),
				yProportional(0),
				yIntegral(0),
				yDerivativeError(0),
				phiRollDegrees(0),
			
			
				altitudeMetersAgl(0),
				zNEDLocalFrame(0),
				referenceAltitudeMeters(0),
				mainRotorControl(0),
				mainRotorControlBeforeServoLimitsAdjustment(0),
				zVelocityMetersPerSecond(0),
				referenceZVelocityMetersPerSecond(0),
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
				initialZPositionEcef(0),
				rollRads(0),
				pitchRads(0),
				yawRads(0),
				pitchAngularVelocityRs(0),
				rollAngularVelocityRs(0),
				yawAngularVelocityRs(0),
				xAccelFrdMss(0),
				yAccelFrdMss(0),
				zAccelFrdMss(0),
				xMagFrd(0),
				yMagFrd(0),
				zMagFrd(0),
				xVEcefCms(0),
				yVEcefCms(0),
				zVEcefCms(0),
				pressureMillibars(0)						
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
				
			float ReferenceMagYawRads() const {return referenceMagYawRads;}
			void ReferenceMagYawRads(float val) { referenceMagYawRads = val;}
				
				
			float YawControl() const {return yawControl;}
			void YawControl(float val) { yawControl = val;}
				
			float YawControlBeforeServoLimitsAdjustment() const {return yawControlBeforeServoLimitsAdjustment;}
			void YawControlBeforeServoLimitsAdjustment(float val) { yawControlBeforeServoLimitsAdjustment = val;}
				
			float ReferenceYawVelocityRadsPerSecond() const {return referenceYawVelocityRadsPerSecond;}
			void ReferenceYawVelocityRadsPerSecond(float val) { referenceYawVelocityRadsPerSecond = val;}
				
			float YawIntegral() const {return yawIntegral;}
			void YawIntegral(float val) { yawIntegral = val;}

			float YawProportional() const {return yawProportional;}
			void YawProportional(float val) { yawProportional = val;}

			float YawDerivativeError() const {return yawDerivativeError;}
			void YawDerivativeError(float val) { yawDerivativeError = val;}								
																							
		
		
		
			float XNEDLocalFrame() const {return xNEDLocalFrame;}
			void XNEDLocalFrame(float val) { xNEDLocalFrame = val;}
						
			float ReferenceXNEDLocalFrame() const {return referenceXNEDLocalFrame;}
			void ReferenceXNEDLocalFrame(float val) { referenceXNEDLocalFrame = val;}

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
				
						
						
						


			float YNEDLocalFrame() const {return yNEDLocalFrame;}
			void YNEDLocalFrame(float val) { yNEDLocalFrame = val;}

			float ReferenceYNEDLocalFrame() const {return referenceYNEDLocalFrame;}
			void ReferenceYNEDLocalFrame(float val) { referenceYNEDLocalFrame = val;}

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
						
						
						
						

			float AltitudeMetersAgl() const { return altitudeMetersAgl; }
			void AltitudeMetersAgl(float val) { altitudeMetersAgl = val; }

			float ZNEDLocalFrame() const { return zNEDLocalFrame; }
			void ZNEDLocalFrame(float val) { zNEDLocalFrame = val; }


			float ReferenceZNEDLocalFrameMeters() const {return referenceAltitudeMeters;}
			void ReferenceZNEDLocalFrameMeters(float val) { referenceAltitudeMeters = val;}


			float MainRotorCollectiveControl() const {return mainRotorControl;}
			void MainRotorCollectiveControl(float val) { mainRotorControl = val;}

			float MainRotorControlBeforeServoLimitsAdjustment() const {return mainRotorControlBeforeServoLimitsAdjustment;}
			void MainRotorControlBeforeServoLimitsAdjustment(float val) { mainRotorControlBeforeServoLimitsAdjustment = val;}

			float ZVelocityMetersPerSecond() const {return zVelocityMetersPerSecond;}
			void ZVelocityMetersPerSecond(float val) { zVelocityMetersPerSecond = val;}

			float ReferenceZVelocityMetersPerSecond() const {return referenceZVelocityMetersPerSecond;}
			void ReferenceZVelocityMetersPerSecond(float val) { referenceZVelocityMetersPerSecond = val;}

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
				
				
				
				
				
				
			float YawAngularVelocityRadsPerSecond() const {return yawAngularVelocityRs;}
			void YawAngularVelocityRadsPerSecond(float val) {yawAngularVelocityRs = val;}
				
			float PitchAngularVelocityRadsPerSecond() const {return pitchAngularVelocityRs;}
			void PitchAngularVelocityRadsPerSecond(float val) {pitchAngularVelocityRs = val;}

			float RollAngularVelocityRadsPerSecond() const {return rollAngularVelocityRs;}
			void RollAngularVelocityRadsPerSecond(float val) {rollAngularVelocityRs = val;}		
				
				
			float XAccelFrdMss() const {return xAccelFrdMss;}
			void XAccelFrdMss(float val) {xAccelFrdMss = val;}

			float YAccelFrdMss() const {return yAccelFrdMss;}
			void YAccelFrdMss(float val) {yAccelFrdMss = val;}
				
			float ZAccelFrdMss() const {return zAccelFrdMss;}
			void ZAccelFrdMss(float val) {zAccelFrdMss = val;}	
				
			float XMagFrd() const {return xMagFrd;}
			void XMagFrd(float val) {xMagFrd = val;}

			float YMagFrd() const {return yMagFrd;}
			void YMagFrd(float val) {yMagFrd = val;}

			float ZMagFrd() const {return zMagFrd;}
			void ZMagFrd(float val) {zMagFrd = val;}	
				
			float YawRads() const {return yawRads;}
			void YawRads(float val) {yawRads = val;}	
				
			float PitchRads() const {return pitchRads;}
			void PitchRads(float val) {pitchRads = val;}
				
			float RollRads() const {return rollRads;}
			void RollRads(float val) {rollRads = val;}	

			long XEcefCm() const {return xEcefCm;}
			void XEcefCm(long val) {xEcefCm = val;}
			
			long YEcefCm() const {return yEcefCm;}
			void YEcefCm(long val) {yEcefCm = val;}
				
			long ZEcefCm() const {return zEcefCm;}
			void ZEcefCm(long val) {zEcefCm = val;}				
				
			long XVEcefCms() const {return xVEcefCms;}
			void XVEcefCms(long val) {xVEcefCms = val;}	
				
			long YVEcefCms() const {return yVEcefCms;}
			void YVEcefCms(long val) {yVEcefCms = val;}
				
			long ZVEcefCms() const {return zVEcefCms;}
			void ZVEcefCms(long val) {zVEcefCms = val;}		
				
			float PressureMillibars() const {return pressureMillibars;}
			void PressureMillibars(float val) {pressureMillibars = val;}
								
			float InitialAltitudeMeters() const {return initialAltitudeMeters;}
			void InitialAltitudeMeters(float val) {initialAltitudeMeters = val;}													
													
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */
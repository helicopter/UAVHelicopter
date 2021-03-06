/*
 * SystemModel.h
 *
 * Created: 9/8/2013 4:57:07 PM
 *  Author: HP User
 */ 


#ifndef SYSTEMMODEL_H_
#define SYSTEMMODEL_H_

#include "AHRS.h"

using namespace helicopter::navigation;

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
			
			static SystemModel *publicModel;
			
			/**
			 * Enum representing whether the helicopter is being controlled
			 * by a radio transmitter (ManualControl) or by the PID algorithm (AutoPilot).
			 */
			enum OperatingStates {ManualControl, AutoPilot};
				
			enum FlightModes {SimulatedFlight, RealFlightTest, HardwareInLoopSimulatedFlight, RealFlight};
				
			enum SensorInputs {SimulatedSensors, RealSensors};
				
			enum CommunicationMethods {USB, Radio};
				
			
				
			private:
			
			AHRS *ahrs;
			
			OperatingStates operationalState;
			FlightModes flightMode;
			SensorInputs sensorInput;
			CommunicationMethods communicationMethod;
			
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
			float xVelocityFRDCms;
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
			float yVelocityFRDCms;
			float referenceYVelocityMetersPerSecondBodyFrame;
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
			float referenceZVelocityCms;
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
			float initialAltitudeCm;
			
			
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
			
			float auxChannelValue;
			
			public:
			
			float EcefToLocalNEDRotationMatrix[3][3];
			
			bool HasNewPressureReading;
			
			SystemModel():
				ahrs(NULL),
				operationalState(ManualControl),
				flightMode(SimulatedFlight),
				sensorInput(SimulatedSensors),
				communicationMethod(USB),
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
				xVelocityFRDCms(0),
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
				yVelocityFRDCms(0),
				referenceYVelocityMetersPerSecondBodyFrame(0),
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
				referenceZVelocityCms(0),
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
				xEcefCm(0),
				yEcefCm(0),
				zEcefCm(0),
				xVEcefCms(0),
				yVEcefCms(0),
				zVEcefCms(0),
				pressureMillibars(0),
				auxChannelValue(0),
				HasNewPressureReading(false)				
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
																							
		
		
		
			float XNEDLocalFrameCm() const {return xNEDLocalFrame;}
			void XNEDLocalFrameCm(float val) { xNEDLocalFrame = val;}
						
			float ReferenceXNEDLocalFrameCm() const {return referenceXNEDLocalFrame;}
			void ReferenceXNEDLocalFrameCm(float val) { referenceXNEDLocalFrame = val;}

			float LongitudeControlBeforeServoLimitsAdjustment() const {return longitudeControlBeforeServoLimitsAdjustment;}
			void LongitudeControlBeforeServoLimitsAdjustment(float val) { longitudeControlBeforeServoLimitsAdjustment = val;}

			float LongitudeControl() const {return longitudeControl;}
			void LongitudeControl(float val) { longitudeControl = val;}
				
			float XVelocityFRDCms() const {return xVelocityFRDCms;}
			void XVelocityFRDCms(float val) { xVelocityFRDCms = val;}
				
			float ReferenceXVelocityCms() const {return referenceXVelocityMetersPerSecond;}
			void ReferenceXVelocityCms(float val) { referenceXVelocityMetersPerSecond = val;}
				
			float XLongitudeOuterLoopSetpoint() const {return xLongitudeOuterLoopSetpoint;}
			void XLongitudeOuterLoopSetpoint(float val) { xLongitudeOuterLoopSetpoint = val;}
				
			float XProportional() const {return xProportional;}
			void XProportional(float val) { xProportional = val;}
				
			float XIntegral() const {return xIntegral;}
			void XIntegral(float val) { xIntegral = val;}
				
			float XDerivativeError() const {return xDerivativeError;}
			void XDerivativeError(float val) { xDerivativeError = val;}
				
						
						
						


			float YNEDLocalFrameCm() const {return yNEDLocalFrame;}
			void YNEDLocalFrameCm(float val) { yNEDLocalFrame = val;}

			float ReferenceYNEDLocalFrameCm() const {return referenceYNEDLocalFrame;}
			void ReferenceYNEDLocalFrameCm(float val) { referenceYNEDLocalFrame = val;}

			float LateralControlBeforeServoLimitsAdjustment() const {return lateralControlBeforeServoLimitsAdjustment;}
			void LateralControlBeforeServoLimitsAdjustment(float val) { lateralControlBeforeServoLimitsAdjustment = val;}

			float LateralControl() const {return lateralControl;}
			void LateralControl(float val) { lateralControl = val;}

			float YVelocityFRDCms() const {return yVelocityFRDCms;}
			void YVelocityFRDCms(float val) { yVelocityFRDCms = val;}

			float ReferenceYVelocityCms() const {return referenceYVelocityMetersPerSecondBodyFrame;}
			void ReferenceYVelocityCms(float val) { referenceYVelocityMetersPerSecondBodyFrame = val;}

			float YLateralOuterLoopSetpoint() const {return yLateralOuterLoopSetpoint;}
			void YLateralOuterLoopSetpoint(float val) { yLateralOuterLoopSetpoint = val;}

			float YProportional() const {return yProportional;}
			void YProportional(float val) { yProportional = val;}

			float YIntegral() const {return yIntegral;}
			void YIntegral(float val) { yIntegral = val;}

			float YDerivativeError() const {return yDerivativeError;}
			void YDerivativeError(float val) { yDerivativeError = val;}
						
						
						
						
			/*
			float AltitudeMetersAgl() const { return altitudeMetersAgl; }
			void AltitudeMetersAgl(float val) { altitudeMetersAgl = val; }
			*/

			float ZNEDLocalFrameCm() const { return zNEDLocalFrame; }
			void ZNEDLocalFrameCm(float val) { zNEDLocalFrame = val; }


			float ReferenceZNEDLocalFrameCm() const {return referenceAltitudeMeters;}
			void ReferenceZNEDLocalFrameCm(float val) { referenceAltitudeMeters = val;}


			float MainRotorCollectiveControl() const {return mainRotorControl;}
			void MainRotorCollectiveControl(float val) { mainRotorControl = val;}

			float MainRotorControlBeforeServoLimitsAdjustment() const {return mainRotorControlBeforeServoLimitsAdjustment;}
			void MainRotorControlBeforeServoLimitsAdjustment(float val) { mainRotorControlBeforeServoLimitsAdjustment = val;}

			float ZVelocityFRDCms() const {return zVelocityMetersPerSecond;}
			void ZVelocityFRDCms(float val) { zVelocityMetersPerSecond = val;}

			float ReferenceZVelocityCms() const {return referenceZVelocityCms;}
			void ReferenceZVelocityCms(float val) { referenceZVelocityCms = val;}

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
				
			OperatingStates  OperationalState() const {return operationalState;}
			void OperationalState( OperatingStates val ) { operationalState = val;}

			FlightModes  FlightMode() const {return flightMode;}
			void FlightMode( FlightModes val ) { flightMode = val;}
				
			SensorInputs  SensorInput() const {return sensorInput;}
			void SensorInput( SensorInputs val ) { sensorInput = val;}								
				
			CommunicationMethods  CommunicationMethod() const {return communicationMethod;}
			void CommunicationMethod( CommunicationMethods val ) { communicationMethod = val;}				
				
				
				
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
								
			float InitialAltitudeCm() const {return initialAltitudeCm;}
			void InitialAltitudeCm(float val) {initialAltitudeCm = val;}		
															
			float AuxChannelValue() const {return auxChannelValue;}
			void AuxChannelValue(float val) {auxChannelValue = val;}

			AHRS* Ahrs() const {return ahrs;}
			void Ahrs(AHRS *val) {ahrs = val;}







float cal1f;
float off1() const {return cal1f;}
void off1(float val) {cal1f = val;}
float cal2f;
float off2() const {return cal2f;}
void off2(float val) {cal2f = val;}
float cal3f;
float off3() const {return cal3f;}
void off3(float val) {cal3f = val;}





													
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */
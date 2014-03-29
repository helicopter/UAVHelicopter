/*
 * SystemTelemetryMessage.cpp
 *
 * Created: 9/8/2013 7:22:48 PM
 *  Author: HP User
 */ 

#include "SystemTelemetryMessage.h"

#include <string.h>

using namespace helicopter::messages;


byte *SystemTelemetryMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	
	encode (msgPtr, msgType);
	encode (msgPtr, YawIntegral);
	encode (msgPtr, YawProportional);
	encode (msgPtr, YawDerivativeError);
	encode (msgPtr, YawControl);
	encode (msgPtr, YawIntegralGain);
	encode (msgPtr, YawDerivativeGain);
	encode (msgPtr, YawProportionalGain);
	encode (msgPtr, YawAntiWindupGain);
	


	encode (msgPtr, XNEDLocalFrame);
	encode (msgPtr, XVelocityMetersPerSecond);
	encode (msgPtr, PitchRads);	
	encode (msgPtr, XIntegral);
	encode (msgPtr, XProportional);
	encode (msgPtr, XDerivativeError);
	encode (msgPtr, LongitudeControl);
	encode (msgPtr, XIntegralGain);
	encode (msgPtr, XDerivativeGain);
	encode (msgPtr, XProportionalGain);
	encode (msgPtr, XAntiWindupGain);	
	encode (msgPtr, LongitudeInnerLoopGain);	
	encode (msgPtr, XLongitudeOuterLoopSetpoint);
	encode (msgPtr, PitchAngularVelocityGain);
	
	encode (msgPtr, YNEDLocalFrame);
	encode (msgPtr, YVelocityMetersPerSecond);
	encode (msgPtr, RollRads);
	encode (msgPtr, YIntegral);
	encode (msgPtr, YProportional);
	encode (msgPtr, YDerivativeError);
	encode (msgPtr, LateralControl);
	encode (msgPtr, YIntegralGain);
	encode (msgPtr, YDerivativeGain);
	encode (msgPtr, YProportionalGain);
	encode (msgPtr, YAntiWindupGain);
	encode (msgPtr, LateralInnerLoopGain);	
	encode (msgPtr, YLateralOuterLoopSetpoint);
	encode (msgPtr, RollAngularVelocityGain);
	

	encode (msgPtr, AltitudeMetersAgl);
	encode (msgPtr, ZVelocityMetersPerSecond);
	encode (msgPtr, YawRads);
	encode (msgPtr, ZIntegral);
	encode (msgPtr, ZProportional);
	encode (msgPtr, ZDerivativeError);
	encode (msgPtr, MainRotorCollectiveControl);
	encode (msgPtr, ZIntegralGain);
	encode (msgPtr, ZDerivativeGain);
	encode (msgPtr, ZProportionalGain);
	encode (msgPtr, ZAntiWindupGain);
	encode (msgPtr, ZNEDLocalFrame);
		
	
	encode(msgPtr, LatitudeDegrees);
	encode(msgPtr, LongitudeDegrees);
	
	
	encode (msgPtr, XAccelFrdMss);
	encode (msgPtr, YAccelFrdMss);
	encode (msgPtr, ZAccelFrdMss);
	encode (msgPtr, YawAngularVelocityRadsPerSecond);
	encode (msgPtr, PitchAngularVelocityRadsPerSecond);
	encode (msgPtr, RollAngularVelocityRadsPerSecond);
	encode (msgPtr, XMagFrd);
	encode (msgPtr, YMagFrd);
	encode (msgPtr, ZMagFrd);
	encode (msgPtr, XEcefCm);
	encode (msgPtr, YEcefCm);
	encode (msgPtr, ZEcefCm);	
	encode (msgPtr, XVEcefCms);
	encode (msgPtr, YVEcefCms);
	encode (msgPtr, ZVEcefCms);
	encode (msgPtr, PressureMillibars);	
	
	
	
	
	
	
	encode (msgPtr, Timeouts);
	encode (msgPtr, UnrecognizedMsgTypes);
	encode (msgPtr, ChecksumErrors);
	encode (msgPtr, NumOfBlownFrames);
	encode (msgPtr, SerialCommunicationBufferOverruns);
				
	return msg;
}

void SystemTelemetryMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		decode (message,msgType);
		decode (message, YawIntegral);
		decode (message, YawProportional);
		decode (message, YawDerivativeError);	
		decode (message, YawControl);	
		decode (message, YawIntegralGain);
		decode (message, YawDerivativeGain);
		decode (message, YawProportionalGain);
		decode (message, YawAntiWindupGain);
		
		
		

		decode (message, XNEDLocalFrame);
		decode (message, XVelocityMetersPerSecond);
		decode (message, PitchRads);
		decode (message, XIntegral);
		decode (message, XProportional);
		decode (message, XDerivativeError);
		decode (message, LongitudeControl);
		decode (message, XIntegralGain);
		decode (message, XDerivativeGain);
		decode (message, XProportionalGain);
		decode (message, XAntiWindupGain);
		decode (message, LongitudeInnerLoopGain);
		decode (message, XLongitudeOuterLoopSetpoint);
		decode (message, PitchAngularVelocityGain);

		decode (message, YNEDLocalFrame);
		decode (message, YVelocityMetersPerSecond);
		decode (message, RollRads);
		decode (message, YIntegral);
		decode (message, YProportional);
		decode (message, YDerivativeError);
		decode (message, LateralControl);
		decode (message, YIntegralGain);
		decode (message, YDerivativeGain);
		decode (message, YProportionalGain);
		decode (message, YAntiWindupGain);
		decode (message, LateralInnerLoopGain);
		decode (message, YLateralOuterLoopSetpoint);
		decode (message, RollAngularVelocityGain);
		

		decode (message, AltitudeMetersAgl);
		decode (message, ZVelocityMetersPerSecond);
		decode (message, YawRads);
		decode (message, ZIntegral);
		decode (message, ZProportional);
		decode (message, ZDerivativeError);
		decode (message, MainRotorCollectiveControl);
		decode (message, ZIntegralGain);
		decode (message, ZDerivativeGain);
		decode (message, ZProportionalGain);
		decode (message, ZAntiWindupGain);
		decode (message, ZNEDLocalFrame);
		
	
		decode (message, LatitudeDegrees);
		decode (message, LongitudeDegrees);
		
		
		decode (message, XAccelFrdMss);
		decode (message, YAccelFrdMss);
		decode (message, ZAccelFrdMss);
		decode (message, YawAngularVelocityRadsPerSecond);
		decode (message, PitchAngularVelocityRadsPerSecond);
		decode (message, RollAngularVelocityRadsPerSecond);
		decode (message, XMagFrd);
		decode (message, YMagFrd);
		decode (message, ZMagFrd);
		decode (message, XEcefCm);
		decode (message, YEcefCm);	
		decode (message, ZEcefCm);				
		decode (message, XVEcefCms);
		decode (message, YVEcefCms);
		decode (message, ZVEcefCms);
		decode (message, PressureMillibars);		

		
		
						
		decode (message, Timeouts);
		decode (message, UnrecognizedMsgTypes);
		decode (message, ChecksumErrors);
		decode (message, NumOfBlownFrames);
		decode (message, SerialCommunicationBufferOverruns);
	}
}

SystemTelemetryMessage* SystemTelemetryMessage::buildMessageSt(byte *message)
{
	SystemTelemetryMessage *msg = new SystemTelemetryMessage();
	msg->buildMessage(message);
	
	return msg;
}

SystemTelemetryMessage * SystemTelemetryMessage::buildMessageFromModel(SystemModel *model)
{
	SystemTelemetryMessage *message = new SystemTelemetryMessage();
	
	message->YawDerivativeError = model->YawDerivativeError();
	message->YawIntegral = model->YawIntegral();
	message->YawProportional = model->YawProportional();
	message->YawControl = model->YawControl();
	

	message->PitchRads = model->PitchRads();
	message->XNEDLocalFrame = model->XNEDLocalFrame();
	message->XDerivativeError = model->XDerivativeError();
	message->XIntegral = model->XIntegral();
	message->XProportional = model->XProportional();
	message->XVelocityMetersPerSecond = model->XVelocityMetersPerSecond();
	message->LongitudeControl = model->LongitudeControl();
	message->XLongitudeOuterLoopSetpoint = model->XLongitudeOuterLoopSetpoint();
	message->PitchAngularVelocityRadsPerSecond = model->PitchAngularVelocityRadsPerSecond();

	
	message->RollRads = model->RollRads();
	message->YNEDLocalFrame = model->YNEDLocalFrame();
	message->YDerivativeError = model->YDerivativeError();
	message->YIntegral = model->YIntegral();
	message->YProportional = model->YProportional();
	message->YVelocityMetersPerSecond = model->YVelocityMetersPerSecond();
	message->LateralControl = model->LateralControl();
	message->YLateralOuterLoopSetpoint = model->YLateralOuterLoopSetpoint();
	message->RollAngularVelocityRadsPerSecond = model->RollAngularVelocityRadsPerSecond();	
	
	
	message->AltitudeMetersAgl = model->AltitudeMetersAgl();
	message->ZDerivativeError = model->ZDerivativeError();
	message->YawRads = model->YawRads();
	message->ZIntegral = model->ZIntegral();
	message->ZProportional = model->ZProportional();
	message->ZVelocityMetersPerSecond = (model->ZVelocityMetersPerSecond());
	message->MainRotorCollectiveControl = model->MainRotorCollectiveControl();
	message->ZNEDLocalFrame = model->ZNEDLocalFrame();
	
	
	
	message->XAccelFrdMss = model->XAccelFrdMss();
	message->YAccelFrdMss = model->YAccelFrdMss();
	message->ZAccelFrdMss = model->ZAccelFrdMss();
	message->YawAngularVelocityRadsPerSecond = model->YawAngularVelocityRadsPerSecond();
	message->PitchAngularVelocityRadsPerSecond = model->PitchAngularVelocityRadsPerSecond();
	message->RollAngularVelocityRadsPerSecond = model->RollAngularVelocityRadsPerSecond();
	message->XMagFrd = model->XMagFrd();
	message->YMagFrd = model->YMagFrd();
	message->ZMagFrd = model->ZMagFrd();

	message->XEcefCm = model->XEcefCm();
	message->YEcefCm = model->YEcefCm();
	message->ZEcefCm = model->ZEcefCm();	
	message->XVEcefCms = model->XVEcefCms();
	message->YVEcefCms = model->YVEcefCms();
	message->ZVEcefCms = model->ZVEcefCms();
	message->PressureMillibars = model->PressureMillibars();
	
	
	
	message->ChecksumErrors = model->ChecksumErrors();
	message->Timeouts = model->Timeouts();
	message->UnrecognizedMsgTypes = model->UnrecognizedMsgTypes();
	message->NumOfBlownFrames = model->BlownFrames();
	message->SerialCommunicationBufferOverruns = model->SerialCommunicationBufferOverruns();
	
	message->LatitudeDegrees = model->LatitudeDegrees();
	message->LongitudeDegrees = model->LongitudeDegrees();
	
	return message;
}


//TODO delete this method, its not used.
void SystemTelemetryMessage::updateModelFromMessage (SystemModel *model)
{
	model->YawAngularVelocityRadsPerSecond(this->YawAngularVelocityRadsPerSecond);
	model->YawIntegral(this->YawIntegral);
	model->YawProportional(this->YawProportional);
	model->YawDerivativeError(this->YawDerivativeError);
	model->YawControl(this->YawControl);
	model->Timeouts(this->Timeouts);
	model->UnrecognizedMsgTypes(this->UnrecognizedMsgTypes);
	model->ChecksumErrors(this->ChecksumErrors);
	model->BlownFrames(this->NumOfBlownFrames);
}

void SystemTelemetryMessage::updateModelFromMessageFromSimulator (SystemModel *model)
{
	//These values are sensor readings from the simulator that are used for flight
	//control calculations. The real helicopter algorithms will read these values
	//from actual sensors, but when running off of the simulator, these sensor
	//readings come from the simulator itself.
	
	//model->XNEDBodyFrame((double) this->XNEDBodyFrame / 100);//not supposed to be here since this is a calculated value, not a sensor reading.
	
	//model->ThetaPitchDegrees(this->ThetaPitchDegrees);
	
	
	//model->YNEDBodyFrame((double) this->YNEDBodyFrame / 100);
	
	//model->PhiRollDegrees(this->PhiRollDegrees);
	
	
	//model->AltitudeMetersAgl(this->AltitudeMetersAgl);
	
	//Convert from meters per sec, to foot per sec.
	
	
	
	//Copy fake gyroscope data
	model->PitchAngularVelocityRadsPerSecond(this->PitchAngularVelocityRadsPerSecond);
	model->YawAngularVelocityRadsPerSecond(this->YawAngularVelocityRadsPerSecond);
	model->RollAngularVelocityRadsPerSecond(this->RollAngularVelocityRadsPerSecond);
	
	
	//Copy fake GPS data
	model->XVelocityMetersPerSecond(this->XVelocityMetersPerSecond);
	model->YVelocityMetersPerSecond(this->YVelocityMetersPerSecond);
	//ZVelocity is derived from pressure changes. //model->ZVelocityMetersPerSecond(this->ZVelocityMetersPerSecond); 
	model->LatitudeDegrees(this->LatitudeDegrees);
	model->LongitudeDegrees(this->LongitudeDegrees);
	model->XEcefCm(this->XEcefCm);
	model->YEcefCm(this->YEcefCm);
	model->ZEcefCm(this->ZEcefCm);
		
	//Copy fake accelerometer data
	model->XAccelFrdMss(this->XAccelFrdMss);
	model->YAccelFrdMss(this->YAccelFrdMss);
	model->ZAccelFrdMss(this->ZAccelFrdMss);
	model->XVEcefCms(this->XVEcefCms);
	model->YVEcefCms(this->YVEcefCms);

	//copy fake mag data
	model->XMagFrd(this->XMagFrd);
	model->YMagFrd(this->YMagFrd);
	model->ZMagFrd(this->ZMagFrd);
	
	//Copy fake barometer data.
	model->PressureMillibars(this->PressureMillibars);	

}
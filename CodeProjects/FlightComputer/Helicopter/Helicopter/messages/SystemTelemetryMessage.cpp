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
	encode (msgPtr, MagYaw);
	encode (msgPtr, YawVelocityDegreesPerSecond);
	encode (msgPtr, YawIntegral);
	encode (msgPtr, YawProportional);
	encode (msgPtr, YawDerivativeError);
	encode (msgPtr, YawControl);
	encode (msgPtr, YawIntegralGain);
	encode (msgPtr, YawDerivativeGain);
	encode (msgPtr, YawProportionalGain);
	encode (msgPtr, YawAntiWindupGain);
	


	encode (msgPtr, XNEDBodyFrame);
	encode (msgPtr, XVelocityMetersPerSecond);
	encode (msgPtr, ThetaPitchDegrees);	
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
	encode (msgPtr, PitchAngularVelocityRadsPerSecond);
	encode (msgPtr, PitchAngularVelocityGain);
	
	encode (msgPtr, YNEDBodyFrame);
	encode (msgPtr, YVelocityMetersPerSecond);
	encode (msgPtr, PhiRollDegrees);
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
	encode (msgPtr, RollAngularVelocityRadsPerSecond);
	encode (msgPtr, RollAngularVelocityGain);
	

	encode (msgPtr, AltitudeFeetAgl);
	encode (msgPtr, ZVelocityMetersPerSecond);
	encode (msgPtr, ZIntegral);
	encode (msgPtr, ZProportional);
	encode (msgPtr, ZDerivativeError);
	encode (msgPtr, MainRotorCollectiveControl);
	encode (msgPtr, ZIntegralGain);
	encode (msgPtr, ZDerivativeGain);
	encode (msgPtr, ZProportionalGain);
	encode (msgPtr, ZAntiWindupGain);
	encode (msgPtr, ZNEDBodyFrame);
		
	
	encode(msgPtr, LatitudeDegrees);
	encode(msgPtr, LongitudeDegrees);
	
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
		decode (message,MagYaw);
		decode (message, YawVelocityDegreesPerSecond);
		decode (message, YawIntegral);
		decode (message, YawProportional);
		decode (message, YawDerivativeError);	
		decode (message, YawControl);	
		decode (message, YawIntegralGain);
		decode (message, YawDerivativeGain);
		decode (message, YawProportionalGain);
		decode (message, YawAntiWindupGain);
		
		
		

		decode (message, XNEDBodyFrame);
		decode (message, XVelocityMetersPerSecond);
		decode (message, ThetaPitchDegrees);
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
		decode (message, PitchAngularVelocityRadsPerSecond);
		decode (message, PitchAngularVelocityGain);

		decode (message, YNEDBodyFrame);
		decode (message, YVelocityMetersPerSecond);
		decode (message, PhiRollDegrees);
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
		decode (message, RollAngularVelocityRadsPerSecond);
		decode (message, RollAngularVelocityGain);
		

		decode (message, AltitudeFeetAgl);
		decode (message, ZVelocityMetersPerSecond);
		decode (message, ZIntegral);
		decode (message, ZProportional);
		decode (message, ZDerivativeError);
		decode (message, MainRotorCollectiveControl);
		decode (message, ZIntegralGain);
		decode (message, ZDerivativeGain);
		decode (message, ZProportionalGain);
		decode (message, ZAntiWindupGain);
		decode (message, ZNEDBodyFrame);
		
	
		decode (message, LatitudeDegrees);
		decode (message, LongitudeDegrees);
						
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
	
	message->MagYaw = model->MagYawDegrees();
	message->YawDerivativeError = model->YawDerivativeError();
	message->YawIntegral = model->YawIntegral();
	message->YawProportional = model->YawProportional();
	message->YawVelocityDegreesPerSecond = model->YawVelocityDegreesPerSecond();
	message->YawControl = model->YawControl();
	

	message->ThetaPitchDegrees = model->ThetaPitchDegrees();
	message->XNEDBodyFrame = model->XNEDLocalFrame();
	message->XDerivativeError = model->XDerivativeError();
	message->XIntegral = model->XIntegral();
	message->XProportional = model->XProportional();
	message->XVelocityMetersPerSecond = model->XVelocityMetersPerSecond();
	message->LongitudeControl = model->LongitudeControl();
	message->XLongitudeOuterLoopSetpoint = model->XLongitudeOuterLoopSetpoint();
	message->PitchAngularVelocityRadsPerSecond = model->PitchAngularVelocityRadsPerSecond();

	
	message->PhiRollDegrees = model->PhiRollDegrees();
	message->YNEDBodyFrame = model->YNEDLocalFrame();
	message->YDerivativeError = model->YDerivativeError();
	message->YIntegral = model->YIntegral();
	message->YProportional = model->YProportional();
	message->YVelocityMetersPerSecond = model->YVelocityMetersPerSecond();
	message->LateralControl = model->LateralControl();
	message->YLateralOuterLoopSetpoint = model->YLateralOuterLoopSetpoint();
	message->RollAngularVelocityRadsPerSecond = model->RollAngularVelocityRadsPerSecond();	
	
	
	message->AltitudeFeetAgl = model->AltitudeFeetAgl();
	message->ZDerivativeError = model->ZDerivativeError();
	message->ZIntegral = model->ZIntegral();
	message->ZProportional = model->ZProportional();
	message->ZVelocityMetersPerSecond = (model->ZVelocityFeetPerSecond() / 3.2804f);
	message->MainRotorCollectiveControl = model->MainRotorCollectiveControl();
	message->ZNEDBodyFrame = model->ZNEDLocalFrame();
	
	
	
	message->ChecksumErrors = model->ChecksumErrors();
	message->Timeouts = model->Timeouts();
	message->UnrecognizedMsgTypes = model->UnrecognizedMsgTypes();
	message->NumOfBlownFrames = model->BlownFrames();
	message->SerialCommunicationBufferOverruns = model->SerialCommunicationBufferOverruns();
	
	return message;
}


//TODO delete this method, its not used.
void SystemTelemetryMessage::updateModelFromMessage (SystemModel *model)
{
	model->MagYawDegrees(this->MagYaw);
	model->YawVelocityDegreesPerSecond(this->YawVelocityDegreesPerSecond);
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
	
	model->MagYawDegrees(this->MagYaw);
	model->YawVelocityDegreesPerSecond(this->YawVelocityDegreesPerSecond);
	
	//model->XNEDBodyFrame((double) this->XNEDBodyFrame / 100);//not supposed to be here since this is a calculated value, not a sensor reading.
	model->XVelocityMetersPerSecond(this->XVelocityMetersPerSecond);
	model->ThetaPitchDegrees(this->ThetaPitchDegrees);
	model->PitchAngularVelocityRadsPerSecond(this->PitchAngularVelocityRadsPerSecond);
	
	//model->YNEDBodyFrame((double) this->YNEDBodyFrame / 100);
	model->YVelocityMetersPerSecond(this->YVelocityMetersPerSecond);
	model->PhiRollDegrees(this->PhiRollDegrees);
	model->RollAngularVelocityRadsPerSecond(this->RollAngularVelocityRadsPerSecond);
	
	model->AltitudeFeetAgl(this->AltitudeFeetAgl);
	
	//Convert from meters per sec, to foot per sec.
	
	model->ZVelocityFeetPerSecond(this->ZVelocityMetersPerSecond * 3.28084d); 
	
	model->LatitudeDegrees(this->LatitudeDegrees);
	model->LongitudeDegrees(this->LongitudeDegrees);
}
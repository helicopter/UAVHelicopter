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
		
	
	encode(msgPtr, LatitudeDegrees);
	encode(msgPtr, LongitudeDegrees);
	
	encode (msgPtr, Timeouts);
	encode (msgPtr, UnrecognizedMsgTypes);
	encode (msgPtr, ChecksumErrors);
	encode (msgPtr, NumOfBlownFrames);
				
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
		
	
		decode (message, LatitudeDegrees);
		decode (message, LongitudeDegrees);
						
		decode (message,Timeouts);
		decode (message,UnrecognizedMsgTypes);
		decode (message,ChecksumErrors);
		decode (message,NumOfBlownFrames);
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
	//throw std::exception("The method or operation is not implemented.");
	SystemTelemetryMessage *message = new SystemTelemetryMessage();
	
	message->MagYaw = model->MagYawDegrees() * 100;
	message->YawDerivativeError = model->YawDerivativeError() * 100;
	message->YawIntegral = model->YawIntegral() * 100;
	message->YawProportional = model->YawProportional() * 100;
	message->YawVelocityDegreesPerSecond = model->YawVelocityDegreesPerSecond() * 100;
	message->YawControl = model->YawControl() * 100;
	

	message->XNEDBodyFrame = model->XNEDBodyFrame() * 100;
	message->XDerivativeError = model->XDerivativeError() * 100;
	message->XIntegral = model->XIntegral() * 100;
	message->XProportional = model->XProportional() * 100;
	message->XVelocityMetersPerSecond = model->XVelocityMetersPerSecond() * 100;
	message->LongitudeControl = model->LongitudeControl() * 100;
	
	
	message->YNEDBodyFrame = model->YNEDBodyFrame() * 100;
	message->YDerivativeError = model->YDerivativeError() * 100;
	message->YIntegral = model->YIntegral() * 100;
	message->YProportional = model->YProportional() * 100;
	message->YVelocityMetersPerSecond = model->YVelocityMetersPerSecond() * 100;
	message->LateralControl = model->LateralControl() * 100;
	
	
	
	message->AltitudeFeetAgl = model->AltitudeFeet() * 100;
	message->ZDerivativeError = model->ZDerivativeError() * 100;
	message->ZIntegral = model->ZIntegral() * 100;
	message->ZProportional = model->ZProportional() * 100;
	message->ZVelocityMetersPerSecond = (model->ZVelocityFeetPerSecond() / 3.2804d) * 100;
	message->MainRotorCollectiveControl = model->MainRotorCollectiveControl() * 100;
	
	
	message->ChecksumErrors = model->ChecksumErrors();
	message->Timeouts = model->Timeouts();
	message->UnrecognizedMsgTypes = model->UnrecognizedMsgTypes();
	message->NumOfBlownFrames = model->BlownFrames();
	
	return message;
}


//TODO delete this method, its not used.
void SystemTelemetryMessage::updateModelFromMessage (SystemModel *model)
{
	model->MagYawDegrees((double) this->MagYaw / 100);
	model->YawVelocityDegreesPerSecond((double) this->YawVelocityDegreesPerSecond / 100);
	model->YawIntegral((double) this->YawIntegral / 100);
	model->YawProportional((double) this->YawProportional / 100);
	model->YawDerivativeError((double) this->YawDerivativeError / 100);
	model->YawControl((double) this->YawControl / 100);
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
	
	model->MagYawDegrees((double) this->MagYaw / 100);
	model->YawVelocityDegreesPerSecond((double) this->YawVelocityDegreesPerSecond / 100);
	
	//model->XNEDBodyFrame((double) this->XNEDBodyFrame / 100);//not supposed to be here since this is a calculated value, not a sensor reading.
	model->XVelocityMetersPerSecond((double) this->XVelocityMetersPerSecond / 100);
	model->ThetaPitchDegrees((double) this->ThetaPitchDegrees / 100);
	
	//model->YNEDBodyFrame((double) this->YNEDBodyFrame / 100);
	model->YVelocityMetersPerSecond((double) this->YVelocityMetersPerSecond / 100);
	model->PhiRollDegrees((double) this->PhiRollDegrees / 100);
	
	model->AltitudeFeet((double) this->AltitudeFeetAgl / 100);
	
	//Convert from meters per sec, to foot per sec.
	
	model->ZVelocityFeetPerSecond(((double) this->ZVelocityMetersPerSecond / 100) * 3.28084d); 
	
	model->LatitudeDegrees((double) this->LatitudeDegrees / 100);
	model->LongitudeDegrees((double) this->LongitudeDegrees / 100);
}
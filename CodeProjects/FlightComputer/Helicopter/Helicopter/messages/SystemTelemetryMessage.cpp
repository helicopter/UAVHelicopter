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
		decode (message,YawVelocityDegreesPerSecond);
		decode (message,YawIntegral);
		decode (message,YawProportional);
		decode (message,YawDerivativeError);	
		decode (message, YawControl);					
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
	message->ChecksumErrors = model->ChecksumErrors();
	message->Timeouts = model->Timeouts();
	message->UnrecognizedMsgTypes = model->UnrecognizedMsgTypes();
	message->NumOfBlownFrames = model->BlownFrames();
	
	return message;
}


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
	model->MagYawDegrees((double) this->MagYaw / 100);
	model->YawVelocityDegreesPerSecond((double) this->YawVelocityDegreesPerSecond / 100);
}
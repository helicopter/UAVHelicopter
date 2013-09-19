/*
 * Model.cpp
 *
 * Created: 9/8/2013 7:17:08 PM
 *  Author: HP User
 */ 

#include "SystemModel.h"
#include "SystemTelemetryMessage.h"

using namespace helicopter::model;


SystemTelemetryMessage *SystemModel::CreateTelemetryMessage()
{
	SystemTelemetryMessage *message = new SystemTelemetryMessage();
	
	message->MagX(RawMagX());
	message->MagY(RawMagY());
	message->MagZ(RawMagZ());
	message->ChecksumErrors(ChecksumErrors());
	message->Timeouts(Timeouts());
	message->UnrecognizedMsgTypes(UnrecognizedMsgTypes());
	
	return message;
}

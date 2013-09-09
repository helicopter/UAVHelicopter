/*
 * Model.cpp
 *
 * Created: 9/8/2013 7:17:08 PM
 *  Author: HP User
 */ 

#include "SystemModel.h"
#include "SystemTelemetryMessage.h"

using namespace helicopter::model;


Message *SystemModel::CreateTelemetryMessage()
{
	SystemTelemetryMessage *message = new SystemTelemetryMessage();
	
	message->MagX(RawMagX());
	message->MagY(RawMagY());
	message->MagZ(RawMagZ());
	
	return message;
}
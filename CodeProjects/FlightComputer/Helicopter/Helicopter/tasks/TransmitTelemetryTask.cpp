/*
 * TransmitTelemetryTask.cpp
 *
 * Created: 9/8/2013 7:08:56 PM
 *  Author: HP User
 */ 
#include "TransmitTelemetryTask.h"
#include "SystemTelemetryMessage.h"

using namespace helicopter::tasks;

TransmitTelemetryTask::TransmitTelemetryTask(GroundControlStationInterface *radioInterface, SystemModel *model, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model)
{
	
}


void TransmitTelemetryTask::runTaskImpl()
{
	SystemTelemetryMessage *message = model->CreateTelemetryMessage();
	
	//TODO add some error handling in here
	radioInterface->transmit(message);
	
	delete message;
}
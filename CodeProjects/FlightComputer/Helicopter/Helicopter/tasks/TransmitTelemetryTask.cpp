/*
 * TransmitTelemetryTask.cpp
 *
 * Created: 9/8/2013 7:08:56 PM
 *  Author: HP User
 */ 
#include "TransmitTelemetryTask.h"
#include "SystemTelemetryMessage.h"
#include "ControlMessage.h"

using namespace helicopter::tasks;

TransmitTelemetryTask::TransmitTelemetryTask(GroundControlStationInterface *radioInterface, SystemModel *model, DATATOSEND dataToSend, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model),
	dataToSend(dataToSend)
{
	
}


void TransmitTelemetryTask::runTaskImpl()
{
	if (dataToSend == ALLDATA)
	{
		//SystemTelemetryMessage *message = model->CreateTelemetryMessage();
		SystemTelemetryMessage *message = SystemTelemetryMessage::buildMessageFromModel(model);
	
		//TODO add some error handling in here
		radioInterface->transmit(message);
	
		delete message;
	}else if (dataToSend == CONTROLDATA)
	{
		ControlMessage *message = new ControlMessage();
		message->LateralControl = model->LateralControl();
		message->LongitudeControl = model->LongitudeControl();
		message->MainRotorCollectiveControl = model->MainRotorCollectiveControl();
		message->YawControl = model->YawControl();
		
		//TODO add some error handling in here
		radioInterface->transmit(message);
		
		delete message;		
	}
}
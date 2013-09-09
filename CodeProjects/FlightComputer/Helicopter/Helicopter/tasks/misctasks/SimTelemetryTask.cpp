/*
 * SimTelemetryTask.cpp
 *
 * Created: 9/8/2013 3:49:12 PM
 *  Author: HP User
 */ 
#include "SimTelemetryTask.h"
#include "SimTelemetryMessage.h"

using namespace helicopter::tasks::misctasks;
using namespace helicopter::messages;

SimTelemetryTask::SimTelemetryTask(RadioInterface *radioInterface, SystemModel *model, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model)
{
	
}


void SimTelemetryTask::runTaskImpl() 
{
	//Use the radio interface to get the telemetry message from the simulator
	Message *message = NULL;
	
	//TODO: keep in mind that the message received might not have been received in the
	//expected order. I.e. this message might actually be a different type of message.
	//so we should use more of a pub sub model. This works for now though since this is the
	//only message.
	if (radioInterface->receive(message) == 0 && message != NULL)
	{
		//If it's a sim telemetry message, then process it. 
		if (message->getType() == SimTelemetryMessage::SimTelemetryMessageType)
		{
			SimTelemetryMessage *simMsg = (SimTelemetryMessage*) message;
			
			//Update the model using the new data received from the simulator.
			model->RawMagX(simMsg->MagX());
			model->RawMagY(simMsg->MagY());
			model->RawMagZ(simMsg->MagZ());
		}
		
		delete message;
	}
	
}
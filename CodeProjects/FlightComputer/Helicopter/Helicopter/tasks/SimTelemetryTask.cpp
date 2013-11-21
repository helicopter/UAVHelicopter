/*
 * SimTelemetryTask.cpp
 *
 * Created: 9/8/2013 3:49:12 PM
 *  Author: HP User
 */ 
#include "SimTelemetryTask.h"
#include "SystemTelemetryMessage.h"
#include "SyncMessage.h"

using namespace helicopter::tasks;
using namespace helicopter::messages;

SimTelemetryTask::SimTelemetryTask(GroundControlStationInterface *radioInterface, SystemModel *model, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model)
{
	
}

void SimTelemetryTask::runTaskImpl() 
{
	
	//Send sync message to indicate to the ground control station that we are ready to receive data.
	//Without this sync message, the GCS could send data while the FC is asleep, which would cause the data
	//to overflow the usart buffer.
	SyncMessage syncMsg;
	
	int status = radioInterface->transmit(&syncMsg);
	
	if (status == 0)
	{
	
		//Use the radio interface to get the telemetry message from the simulator
		Message *message = NULL;
	
		status = radioInterface->receive(message);
	
		if (status == 0 && message != NULL)
		{
			SystemTelemetryMessage *telemMsg = (SystemTelemetryMessage*) message;
			
			//Update the model using the new data received from the simulator.
			telemMsg->updateModelFromMessageFromSimulator(model);
		
			delete message;
		}
	}
	
	//Update instrumentation fields
	switch (status)
	{
		case 0:
			break;
		case -1:
			model->Timeouts(model->Timeouts() + 1);
			break;
		case -2:
			model->UnrecognizedMsgTypes(model->UnrecognizedMsgTypes() + 1);
			break;
		case -3:
			model->ChecksumErrors(model->ChecksumErrors() + 1);
		default:
			break;
		
	}
}

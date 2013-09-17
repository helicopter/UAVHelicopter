/*
 * SimTelemetryTask.cpp
 *
 * Created: 9/8/2013 3:49:12 PM
 *  Author: HP User
 */ 
#include "SimTelemetryTask.h"
#include "SystemTelemetryMessage.h"
#include "SyncMessage.h"

using namespace helicopter::tasks::misctasks;
using namespace helicopter::messages;

SimTelemetryTask::SimTelemetryTask(RadioInterface *radioInterface, SystemModel *model, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model)
{
	
}

bool isOn = false;
void SimTelemetryTask::runTaskImpl() 
{
	if (isOn)
	{
		PORTA |= (1<<PA5);
		isOn = false;
	}
				
				
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
	
		//TODO: keep in mind that the message received might not have been received in the
		//expected order. I.e. this message might actually be a different type of message.
		//so we should use more of a pub sub model. This works for now though since this is the
		//only message.
		if (status == 0 && message != NULL)
		{
			SystemTelemetryMessage *telemMsg = (SystemTelemetryMessage*) message;
			
			//Update the model using the new data received from the simulator.
			model->RawMagX(telemMsg->MagX());
			model->RawMagY(telemMsg->MagY());
			model->RawMagZ(telemMsg->MagZ());
		
			delete message;
		}else if (status == -4 && message != NULL)
		{
			
			SystemTelemetryMessage *telemMsg = (SystemTelemetryMessage*) message;
			model->RawMagX(telemMsg->MagX());
			model->RawMagY(telemMsg->MagY());
			model->RawMagZ(-4);
			delete telemMsg;
		}else if (status == -4 && message == NULL)
		{
			model->RawMagX(-4);
			model->RawMagY(-4);
			model->RawMagZ(-4);
			
		}
		else if (status == -2)
		{
			model->RawMagX(-2);
			model->RawMagY(-2);
			model->RawMagZ(-2);
		}else if (status == -1)
		{
			model->RawMagX(-1);
			model->RawMagY(-1);
			model->RawMagZ(-1);
		}else
		{
			model->RawMagX(-10);
			model->RawMagY(-10);
			model->RawMagZ(status);
		}
		
		if (status != 0)
		{
			PORTA &= ~(1<<PA5);
			isOn = true;
			

		}
	}
}

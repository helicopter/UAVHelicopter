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

SimTelemetryTask::SimTelemetryTask(GroundControlStationInterface *radioInterface, SystemModel *model, PIDController *pidController, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model),
	pidController(pidController)
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
			
			pidController->setYawIntegralGain((double) telemMsg->YawIntegralGain / 100);
			pidController->setYawDerivativeGain((double) telemMsg->YawDerivativeGain / 100);
			pidController->setYawProportionalGain((double) telemMsg->YawProportionalGain / 100);
			pidController->setYawAntiWindupGain((double) telemMsg->YawAntiWindupGain / 100);
		
			pidController->setXIntegralGain((double) telemMsg->XIntegralGain / 100);
			pidController->setXDerivativeGain((double) telemMsg->XDerivativeGain / 100);
			pidController->setXProportionalGain((double) telemMsg->XProportionalGain / 100);
			pidController->setXAntiWindupGain((double) telemMsg->XAntiWindupGain / 100);
			pidController->setLongitudeInnerLoopGain((double) telemMsg->LongitudeInnerLoopGain / 100);
				
			pidController->setYIntegralGain((double) telemMsg->YIntegralGain / 100);
			pidController->setYDerivativeGain((double) telemMsg->YDerivativeGain / 100);
			pidController->setYProportionalGain((double) telemMsg->YProportionalGain / 100);
			pidController->setYAntiWindupGain((double) telemMsg->YAntiWindupGain / 100);
			pidController->setLateralInnerLoopGain((double) telemMsg->LateralInnerLoopGain / 100);
			
			pidController->setZIntegralGain((double) telemMsg->ZIntegralGain / 100);
			pidController->setZDerivativeGain((double) telemMsg->ZDerivativeGain / 100);
			pidController->setZProportionalGain((double) telemMsg->ZProportionalGain / 100);
			pidController->setZAntiWindupGain((double) telemMsg->ZAntiWindupGain / 100);															
		
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

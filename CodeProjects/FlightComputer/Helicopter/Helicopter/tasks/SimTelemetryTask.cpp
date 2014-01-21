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
			
			pidController->setYawIntegralGain(telemMsg->YawIntegralGain);
			pidController->setYawDerivativeGain(telemMsg->YawDerivativeGain);
			pidController->setYawProportionalGain(telemMsg->YawProportionalGain);
			pidController->setYawAntiWindupGain(telemMsg->YawAntiWindupGain);
		
			pidController->setXIntegralGain(telemMsg->XIntegralGain);
			pidController->setXDerivativeGain(telemMsg->XDerivativeGain);
			pidController->setXProportionalGain(telemMsg->XProportionalGain);
			pidController->setXAntiWindupGain(telemMsg->XAntiWindupGain);
			pidController->setLongitudeInnerLoopGain(telemMsg->LongitudeInnerLoopGain);
			pidController->setPitchAngularVelocityGain(telemMsg->PitchAngularVelocityGain);
				
			pidController->setYIntegralGain(telemMsg->YIntegralGain);
			pidController->setYDerivativeGain(telemMsg->YDerivativeGain);
			pidController->setYProportionalGain(telemMsg->YProportionalGain);
			pidController->setYAntiWindupGain(telemMsg->YAntiWindupGain);
			pidController->setLateralInnerLoopGain(telemMsg->LateralInnerLoopGain);
			pidController->setRollAngularVelocityGain(telemMsg->RollAngularVelocityGain);
			
			pidController->setZIntegralGain(telemMsg->ZIntegralGain);
			pidController->setZDerivativeGain(telemMsg->ZDerivativeGain);
			pidController->setZProportionalGain(telemMsg->ZProportionalGain);
			pidController->setZAntiWindupGain(telemMsg->ZAntiWindupGain);															
		
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
			model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns() + 1);
			break;
		case -3:
			model->UnrecognizedMsgTypes(model->UnrecognizedMsgTypes() + 1);
			break;
		case -4:
			model->ChecksumErrors(model->ChecksumErrors() + 1);
		default:
			break;
		
	}
}

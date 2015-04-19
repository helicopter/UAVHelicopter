/*
 * SimTelemetryTask.cpp
 *
 * Created: 9/8/2013 3:49:12 PM
 *  Author: HP User
 */ 
#include "SimTelemetryTask.h"
#include "SystemTelemetryMessage.h"
#include "SyncMessage.h"
#include "SensorDataMessage.h"
#include "GainsMessage.h"

using namespace helicopter::tasks;
using namespace helicopter::messages;

SimTelemetryTask::SimTelemetryTask(GroundControlStationInterface *radioInterface, SystemModel *model, PIDController *pidController,  DATATORECEIVE dataToReceive, int delay, int period) :
	Task(delay, period),
	radioInterface(radioInterface),
	model(model),
	pidController(pidController),
	dataToReceive(dataToReceive)
{
	
}
//
//void SimTelemetryTask::runTaskImpl() 
//{
//
	////Send sync message to indicate to the ground control station that we are ready to receive data.
	////Without this sync message, the GCS could send data while the FC is asleep, which would cause the data
	////to overflow the usart buffer.
	//SyncMessage syncMsg(dataToReceive);
	//
	////Since this is now asynchronous, this will actually be receiving data from the previous request. 
	//int status = radioInterface->transmit(&syncMsg);
	//
	//if (status == 0)
	//{
	//
		////Use the radio interface to get the telemetry message from the simulator
		//Message *message = NULL;
	//
		//status = radioInterface->receive(message);
	//
		//if (status == 0 && message != NULL)
		//{
			//if (message->getType() == SystemTelemetryMessage::MessageType)
			//{
				//SystemTelemetryMessage *telemMsg = (SystemTelemetryMessage*) message;
			//
				////Update the model using the new data received from the simulator.
				//telemMsg->updateModelFromMessageFromSimulator(model);
			//
				//pidController->setYawIntegralGain(telemMsg->YawIntegralGain);
				//pidController->setYawDerivativeGain(telemMsg->YawDerivativeGain);
				//pidController->setYawProportionalGain(telemMsg->YawProportionalGain);
				//pidController->setYawAntiWindupGain(telemMsg->YawAntiWindupGain);
		//
				//pidController->setXIntegralGain(telemMsg->XIntegralGain);
				//pidController->setXDerivativeGain(telemMsg->XDerivativeGain);
				//pidController->setXProportionalGain(telemMsg->XProportionalGain);
				//pidController->setXAntiWindupGain(telemMsg->XAntiWindupGain);
				//pidController->setLongitudeInnerLoopGain(telemMsg->LongitudeInnerLoopGain);
				//pidController->setPitchAngularVelocityGain(telemMsg->PitchAngularVelocityGain);
				//
				//pidController->setYIntegralGain(telemMsg->YIntegralGain);
				//pidController->setYDerivativeGain(telemMsg->YDerivativeGain);
				//pidController->setYProportionalGain(telemMsg->YProportionalGain);
				//pidController->setYAntiWindupGain(telemMsg->YAntiWindupGain);
				//pidController->setLateralInnerLoopGain(telemMsg->LateralInnerLoopGain);
				//pidController->setRollAngularVelocityGain(telemMsg->RollAngularVelocityGain);
			//
				//pidController->setZIntegralGain(telemMsg->ZIntegralGain);
				//pidController->setZDerivativeGain(telemMsg->ZDerivativeGain);
				//pidController->setZProportionalGain(telemMsg->ZProportionalGain);
				//pidController->setZAntiWindupGain(telemMsg->ZAntiWindupGain);																	
			//}else if (message->getType() == SensorDataMessage::MessageType)
			//{
				//SensorDataMessage *sensorMsg = (SensorDataMessage*) message;
				//model->XAccelFrdMss(sensorMsg->XAccelFrdMss);
				//model->YAccelFrdMss(sensorMsg->YAccelFrdMss);
				//model->ZAccelFrdMss(sensorMsg->ZAccelFrdMss);
				//model->YawAngularVelocityRadsPerSecond(sensorMsg->YawAngularVelocityRadsPerSecond);
				//model->PitchAngularVelocityRadsPerSecond(sensorMsg->PitchAngularVelocityRadsPerSecond);
				//model->RollAngularVelocityRadsPerSecond(sensorMsg->RollAngularVelocityRadsPerSecond);
				//model->XMagFrd(sensorMsg->XMagFrd);
				//model->YMagFrd(sensorMsg->YMagFrd);
				//model->ZMagFrd(sensorMsg->ZMagFrd);
				//model->XEcefCm(sensorMsg->XEcefCm);
				//model->YEcefCm(sensorMsg->YEcefCm);
				//model->ZEcefCm(sensorMsg->ZEcefCm);
				//model->XVEcefCms(sensorMsg->XVEcefCms);
				//model->YVEcefCms(sensorMsg->YVEcefCms);
				//model->ZVEcefCms(sensorMsg->ZVEcefCms);
				//model->PressureMillibars(sensorMsg->PressureMillibars);
				//
			//}else if (message->getType() == GainsMessage::MessageType)
			//{
				//GainsMessage *telemMsg = (GainsMessage*) message;
			//
				//pidController->setYawIntegralGain(telemMsg->YawIntegralGain);
				//pidController->setYawDerivativeGain(telemMsg->YawDerivativeGain);
				//pidController->setYawProportionalGain(telemMsg->YawProportionalGain);
				//pidController->setYawAntiWindupGain(telemMsg->YawAntiWindupGain);
			//
				//pidController->setXIntegralGain(telemMsg->XIntegralGain);
				//pidController->setXDerivativeGain(telemMsg->XDerivativeGain);
				//pidController->setXProportionalGain(telemMsg->XProportionalGain);
				//pidController->setXAntiWindupGain(telemMsg->XAntiWindupGain);
				//pidController->setLongitudeInnerLoopGain(telemMsg->LongitudeInnerLoopGain);
				//pidController->setPitchAngularVelocityGain(telemMsg->PitchAngularVelocityGain);
			//
				//pidController->setYIntegralGain(telemMsg->YIntegralGain);
				//pidController->setYDerivativeGain(telemMsg->YDerivativeGain);
				//pidController->setYProportionalGain(telemMsg->YProportionalGain);
				//pidController->setYAntiWindupGain(telemMsg->YAntiWindupGain);
				//pidController->setLateralInnerLoopGain(telemMsg->LateralInnerLoopGain);
				//pidController->setRollAngularVelocityGain(telemMsg->RollAngularVelocityGain);
			//
				//pidController->setZIntegralGain(telemMsg->ZIntegralGain);
				//pidController->setZDerivativeGain(telemMsg->ZDerivativeGain);
				//pidController->setZProportionalGain(telemMsg->ZProportionalGain);
				//pidController->setZAntiWindupGain(telemMsg->ZAntiWindupGain);
				//
				//model->ReferenceMagYawRads(telemMsg->YawRefSetpoint);
				//model->ReferenceXNEDLocalFrameCm(telemMsg->XRefSetpoint);
				//model->ReferenceYNEDLocalFrameCm(telemMsg->YRefSetpoint);
				//model->ReferenceZNEDLocalFrameCm(telemMsg->ZRefSetpoint);
			//}
			//
		//}
		//
		//if (message != NULL)
		//{
			//delete message;
			//message = NULL;
		//}
	//}
	//
	////Update instrumentation fields
	//switch (status)
	//{
		//case 0:
			//break;
		//case -1:
			//model->Timeouts(model->Timeouts() + 1);
			//break;
		//case -2:
			//model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns() + 1);
			//break;
		//case -3:
			//model->UnrecognizedMsgTypes(model->UnrecognizedMsgTypes() + 1);
			//break;
		//case -4:
			//model->ChecksumErrors(model->ChecksumErrors() + 1);
		//default:
			//break;
		//
	//}
//}
//


void SimTelemetryTask::runTaskImpl()
{
	int status = 0;

	//Use the radio interface to get the telemetry message from the simulator
	Message *message = NULL;
		
	status = radioInterface->receive(message);
		
	if (status == 0 && message != NULL)
	{
		if (message->getType() == SystemTelemetryMessage::MessageType)
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
			
			model->HasNewPressureReading = true;
		}else if (message->getType() == SensorDataMessage::MessageType)
		{
			SensorDataMessage *sensorMsg = (SensorDataMessage*) message;
			model->XAccelFrdMss(sensorMsg->XAccelFrdMss);
			model->YAccelFrdMss(sensorMsg->YAccelFrdMss);
			model->ZAccelFrdMss(sensorMsg->ZAccelFrdMss);
			model->YawAngularVelocityRadsPerSecond(sensorMsg->YawAngularVelocityRadsPerSecond);
			model->PitchAngularVelocityRadsPerSecond(sensorMsg->PitchAngularVelocityRadsPerSecond);
			model->RollAngularVelocityRadsPerSecond(sensorMsg->RollAngularVelocityRadsPerSecond);
			model->XMagFrd(sensorMsg->XMagFrd);
			model->YMagFrd(sensorMsg->YMagFrd);
			model->ZMagFrd(sensorMsg->ZMagFrd);
			model->XEcefCm(sensorMsg->XEcefCm);
			model->YEcefCm(sensorMsg->YEcefCm);
			model->ZEcefCm(sensorMsg->ZEcefCm);
			model->XVEcefCms(sensorMsg->XVEcefCms);
			model->YVEcefCms(sensorMsg->YVEcefCms);
			model->ZVEcefCms(sensorMsg->ZVEcefCms);
			model->PressureMillibars(sensorMsg->PressureMillibars);
			
			model->HasNewPressureReading = true;
				
		}else if (message->getType() == GainsMessage::MessageType)
		{
			GainsMessage *telemMsg = (GainsMessage*) message;
				
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
				
			model->ReferenceMagYawRads(telemMsg->YawRefSetpoint);
			model->ReferenceXNEDLocalFrameCm(telemMsg->XRefSetpoint);
			model->ReferenceYNEDLocalFrameCm(telemMsg->YRefSetpoint);
			model->ReferenceZNEDLocalFrameCm(telemMsg->ZRefSetpoint);
		}
			
	}
		
	if (message != NULL)
	{
		delete message;
		message = NULL;
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
	
	
	//Send sync message to indicate to the ground control station that we are ready to receive data.
	//Without this sync message, the GCS could send data while the FC is asleep, which would cause the data
	//to overflow the usart buffer.
	SyncMessage syncMsg(dataToReceive);
	
	//Send this out after receiving data so if there is asynchronous processing, the system doesn't try to
	//read a partial message that might have actually been transmitted just after sending the transmit message.
	//radioInterface->transmit(&syncMsg);	
	if (radioInterface->transmit(&syncMsg) != 0)
	{
		model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns()+1);
	}
}

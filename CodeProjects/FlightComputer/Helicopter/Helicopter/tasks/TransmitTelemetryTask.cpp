/*
 * TransmitTelemetryTask.cpp
 *
 * Created: 9/8/2013 7:08:56 PM
 *  Author: HP User
 */ 
#include "TransmitTelemetryTask.h"
#include "SystemTelemetryMessage.h"
#include "ControlMessage.h"
#include "SimpleTelemetryMessage.h"

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
		}else if (dataToSend == SIMPLEDATA)
		{
			SimpleTelemetryMessage *message = new SimpleTelemetryMessage();
			
			message->PitchRads = model->PitchRads();
			message->XNEDLocalFrame = model->XNEDLocalFrameCm();
			message->XVelocityFRDCms = model->XVelocityFRDCms();

			
			message->RollRads = model->RollRads();
			message->YNEDLocalFrame = model->YNEDLocalFrameCm();
			message->YVelocityFRDCms = model->YVelocityFRDCms();
			
			
			
			message->YawRads = model->YawRads();
			message->ZVelocityFRDCms = (model->ZVelocityFRDCms());
			message->ZNEDLocalFrame = model->ZNEDLocalFrameCm();
			
			
			
			message->XAccelFrdMss = model->XAccelFrdMss();
			message->YAccelFrdMss = model->YAccelFrdMss();
			message->ZAccelFrdMss = model->ZAccelFrdMss();
			message->YawAngularVelocityRadsPerSecond = model->YawAngularVelocityRadsPerSecond();
			message->PitchAngularVelocityRadsPerSecond = model->PitchAngularVelocityRadsPerSecond();
			message->RollAngularVelocityRadsPerSecond = model->RollAngularVelocityRadsPerSecond();
			message->XMagFrd = model->XMagFrd();
			message->YMagFrd = model->YMagFrd();
			message->ZMagFrd = model->ZMagFrd();

			message->XEcefCm = model->XEcefCm();
			message->YEcefCm = model->YEcefCm();
			message->ZEcefCm = model->ZEcefCm();
			message->XVEcefCms = model->XVEcefCms();
			message->YVEcefCms = model->YVEcefCms();
			message->ZVEcefCms = model->ZVEcefCms();
			message->PressureMillibars = model->PressureMillibars();
			
			
			
			message->ChecksumErrors = model->ChecksumErrors();
			message->Timeouts = model->Timeouts();
			message->UnrecognizedMsgTypes = model->UnrecognizedMsgTypes();
			message->NumOfBlownFrames = model->BlownFrames();
			message->SerialCommunicationBufferOverruns = model->SerialCommunicationBufferOverruns();
			
			radioInterface->transmit(message);
	
			delete message;
		}
}
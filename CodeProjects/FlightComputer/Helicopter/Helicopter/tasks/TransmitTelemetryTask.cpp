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

#include <math.h>
#include <avr/io.h>
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
	/*
	if (dataToSend == ALLDATA || dataToSend == SIMPLEDATA)
	{
		
		
		float yawRads = atan2(model->Ahrs()->dcm[1][0], model->Ahrs()->dcm[0][0]);
		if (yawRads < 0)
		{
			yawRads = 2.0f * M_PI + yawRads;
		}
		model->YawRads(yawRads);
		model->PitchRads(-asin(model->Ahrs()->dcm[2][0]));
		model->RollRads(atan2(model->Ahrs()->dcm[2][1], model->Ahrs()->dcm[2][2]));
	}*/
	
	if (dataToSend == ALLDATA)
	{
		//SystemTelemetryMessage *message = model->CreateTelemetryMessage();
		SystemTelemetryMessage *message = SystemTelemetryMessage::buildMessageFromModel(model);
	
	/*
message->XMagFrd = model->off1();
message->YMagFrd = model->off2();
message->ZMagFrd = model->off3();
*/	
	
		//TODO add some error handling in here
		if (radioInterface->transmit(message) != 0)
		{
			model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns()+1);
		}
	
		delete message;
	}else if (dataToSend == CONTROLDATA)
	{
		ControlMessage *message = new ControlMessage();
		message->LateralControl = model->LateralControl();
		message->LongitudeControl = model->LongitudeControl();
		message->MainRotorCollectiveControl = model->MainRotorCollectiveControl();
		message->YawControl = model->YawControl();
		
		//TODO add some error handling in here
		if (radioInterface->transmit(message) != 0)
		{
			model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns()+1);
		}
		
		delete message;		
		}else if (dataToSend == SIMPLEDATA)
		{
			SimpleTelemetryMessage *message = new SimpleTelemetryMessage();
			
			message->PitchRads = model->PitchRads();
			message->XNEDLocalFrame = model->XNEDLocalFrameCm();
//message->XNEDLocalFrame = model->LateralControl();			
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
			
/*
message->XMagFrd = model->off1();
message->YMagFrd = model->off2();
message->ZMagFrd = model->off3();			
*/
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
			
			if (radioInterface->transmit(message) != 0)
			{
				model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns()+1);
			}
	
			delete message;
		}
}
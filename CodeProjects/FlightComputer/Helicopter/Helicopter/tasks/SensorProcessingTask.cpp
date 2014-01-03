/*
 * SensorProcessingTask.cpp
 *
 * Created: 12/20/2013 10:06:56 PM
 *  Author: HP User
 */ 
#include "SensorProcessingTask.h"
#include "CoordinateUtil.h"

using namespace helicopter::tasks;
using namespace helicopter::util;

SensorProcessingTask::SensorProcessingTask (SystemModel *model, int delay, int period) :
	Task(delay, period),
	model(model)
{
	
}

void SensorProcessingTask::runTaskImpl()
{
	float localNEDX = 0;
	float localNEDY = 0;
	float localNEDZ = 0;
	
	CoordinateUtil::ConvertFromGeodedicToLocalNED(model->LatitudeDegrees(), model->LongitudeDegrees(), model->AltitudeFeetAgl(), 
		model->InitialXPositionEcef(), model->InitialYPositionEcef(), model->InitialZPositionEcef(),model->EcefToLocalNEDRotationMatrix,
		localNEDX,localNEDY,localNEDZ);
		
		
	model->XNEDBodyFrame(localNEDX);
	model->YNEDBodyFrame(localNEDY);
	model->ZNEDBodyFrame(localNEDZ);
}

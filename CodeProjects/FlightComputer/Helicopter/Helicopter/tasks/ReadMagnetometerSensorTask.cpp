/*
 * ReadMagnetometerSensorTask.cpp
 *
 * Created: 4/8/2014 8:31:55 PM
 *  Author: HP User
 */ 
#include "ReadMagnetometerSensorTask.h"
#include <avr/io.h>

using namespace helicopter::tasks;

ReadMagnetometerSensorTask::ReadMagnetometerSensorTask (SystemModel *model, MagnetometerSensor *magnetometerSensor, int delay, int period) :
Task(delay, period),
model(model),
magnetometerSensor(magnetometerSensor)
{
	
}


void ReadMagnetometerSensorTask::runTaskImpl()
{


	if (magnetometerSensor->readSensor() != 0)
	{
				DDRA |= (1<<PA4);
				PORTA &= ~(1<<PA4);
	}
	
	if(model->SensorInput() == SystemModel::RealSensors)
	{
		model->XMagFrd(magnetometerSensor->getFRDX());
		model->YMagFrd(magnetometerSensor->getFRDY());
		model->ZMagFrd(magnetometerSensor->getFRDZ());
	}
	
}


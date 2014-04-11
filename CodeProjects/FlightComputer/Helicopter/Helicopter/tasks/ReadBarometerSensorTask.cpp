/*
 * ReadBarometerSensorTask.cpp
 *
 * Created: 4/8/2014 8:32:51 PM
 *  Author: HP User
 */ 
#include "ReadBarometerSensorTask.h"

using namespace helicopter::tasks;

ReadBarometerSensorTask::ReadBarometerSensorTask (SystemModel *model, BarometerSensor *barometerSensor, int delay, int period) :
Task(delay, period),
model(model),
barometerSensor(barometerSensor)
{
	
}


void ReadBarometerSensorTask::runTaskImpl()
{
	barometerSensor->readSensor();
	
//	model->PressureMillibars(barometerSensor->getPressureMillibars());
}

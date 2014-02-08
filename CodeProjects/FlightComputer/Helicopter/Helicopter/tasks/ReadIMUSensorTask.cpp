#include "ReadIMUSensorTask.h"

using namespace helicopter::tasks;

ReadIMUSensorTask::ReadIMUSensorTask (SystemModel *model, IMUSensor *imuSensor, int delay, int period) :
Task(delay, period),
model(model),
imuSensor(imuSensor)
{
	
}


void ReadIMUSensorTask::runTaskImpl()
{
	//Read the sensor values from the IMU Sensor.
	imuSensor->readSensor();
	
	model->XNEDBodyFrame(imuSensor->getFRDAccX());
	model->YNEDBodyFrame(imuSensor->getFRDAccY());
	model->ZNEDBodyFrame(imuSensor->getFRDAccZ());
	
	model->YawProportional(imuSensor->getFRDGyroX());
	model->YawIntegral(imuSensor->getFRDGyroY());
	model->YawDerivativeError(imuSensor->getFRDGyroZ());
	
	/*
	imuSensor->getFRDAccX();
	imuSensor->getFRDAccY();
	imuSensor->getFRDAccZ();
	
	imuSensor->getFRDGyroX();
	imuSensor->getFRDGyroY();
	imuSensor->getFRDGyroZ();
	*/
}

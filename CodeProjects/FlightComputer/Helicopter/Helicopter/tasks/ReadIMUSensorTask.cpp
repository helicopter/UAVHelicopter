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
	
	model->XNEDLocalFrameCm(imuSensor->getFRDAccXMss());
	model->YNEDLocalFrameCm(imuSensor->getFRDAccYMss());
	model->ZNEDLocalFrameCm(imuSensor->getFRDAccZMss());
	
	model->YawProportional(imuSensor->getFRDGyroXRs());
	model->YawIntegral(imuSensor->getFRDGyroYRs());
	model->YawDerivativeError(imuSensor->getFRDGyroZRs());
	
	/*
	imuSensor->getFRDAccX();
	imuSensor->getFRDAccY();
	imuSensor->getFRDAccZ();
	
	imuSensor->getFRDGyroX();
	imuSensor->getFRDGyroY();
	imuSensor->getFRDGyroZ();
	*/
}

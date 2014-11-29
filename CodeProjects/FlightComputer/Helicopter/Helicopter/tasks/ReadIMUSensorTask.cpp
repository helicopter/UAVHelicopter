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
	
	if(imuSensor->missedReading())
	{
		model->SerialCommunicationBufferOverruns(model->SerialCommunicationBufferOverruns() + 1);
	}
	
	//Read the sensor values from the IMU Sensor.
	imuSensor->readSensor();
	
	if(model->SensorInput() == SystemModel::RealSensors)
	{
		model->XAccelFrdMss(imuSensor->getFRDAccXMss());
		model->YAccelFrdMss(imuSensor->getFRDAccYMss());
		model->ZAccelFrdMss(imuSensor->getFRDAccZMss());
		
		model->RollAngularVelocityRadsPerSecond(imuSensor->getFRDGyroXRs());
		model->PitchAngularVelocityRadsPerSecond(imuSensor->getFRDGyroYRs());
		model->YawAngularVelocityRadsPerSecond(imuSensor->getFRDGyroZRs());		
	}
}

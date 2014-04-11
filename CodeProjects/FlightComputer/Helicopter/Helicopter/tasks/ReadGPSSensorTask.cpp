/*
 * ReadGPSSensorTask.cpp
 *
 * Created: 4/8/2014 8:00:26 PM
 *  Author: HP User
 */ 
#include "ReadGPSSensorTask.h"

using namespace helicopter::tasks;

ReadGPSSensorTask::ReadGPSSensorTask (SystemModel *model, GPSSensor *gpsSensor, int delay, int period) :
Task(delay, period),
model(model),
gpsSensor(gpsSensor),
sendReadCommand(true)
{
	
}


void ReadGPSSensorTask::runTaskImpl()
{
	//Read the sensor values from the GPS Sensor.
	gpsSensor->readSensorSolution();
	
	/*if (sendReadCommand)
	{
		sendReadCommand = false;
		gpsSensor->readSensorSolutionSendCommand();
	}else
	{
		sendReadCommand = true;
		gpsSensor->readSensorSolutionReadData();
	}*/
	
	//model->XEcefCm(), model->YEcefCm(), model->ZEcefCm()
	model->XEcefCm(gpsSensor->getXEcefCm());
	model->YEcefCm(gpsSensor->getYEcefCm());
	model->ZEcefCm(gpsSensor->getZEcefCm());
	
	model->XVEcefCms(gpsSensor->getXVEcefCms());
	model->YVEcefCms(gpsSensor->getYVEcefCms());
	model->ZVEcefCms(gpsSensor->getZVEcefCms());
	
}

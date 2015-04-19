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
barometerSensor(barometerSensor),
step(0)
{
	
}


void ReadBarometerSensorTask::runTaskImpl()
{
	//barometerSensor->readSensor();
	
	switch(step++)
	{
		
		case 0:
			barometerSensor->sendD1Command();
		break;
		case 1:
			barometerSensor->readD1Data();
			barometerSensor->sendD2Command();
		break;
		case 2:
			barometerSensor->readD2Data();
			barometerSensor->processBaroData();
			
			if (model->SensorInput() == SystemModel::RealSensors)
			{
				model->PressureMillibars(barometerSensor->getPressureMillibars() / 100.0f);
				model->HasNewPressureReading = true;
			}
			
			
			step = 0;
		break;
		default:
			step = 0;
		break;
		/*
		case 0:
			barometerSensor->sendD1Command();
		break;
		case 1:
			barometerSensor->readD1Data();
		break;
		case 2:
			barometerSensor->sendD2Command();
		break;
		case 3:
			barometerSensor->readD2Data();
		break;
		case 4:
			barometerSensor->processBaroData();
			model->PressureMillibars(barometerSensor->getPressureMillibars() / 100.0f);
			step = 0;
			break;
		default:
			step = 0;
		break;
		*/
	}
	
	
	//model->PressureMillibars(barometerSensor->getPressureMillibars() / 100.0f);
}

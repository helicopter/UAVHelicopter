/*
 * ServoControlTask.cpp
 *
 * Created: 4/16/2014 4:44:25 PM
 *  Author: HP User
 */ 
#include "ServoControlTask.h"

using namespace helicopter::tasks;

ServoControlTask::ServoControlTask (SystemModel *model, RadioControllerInterface *radioControllerInterface,int delay, int period) :
Task(delay, period),
model(model),
radioControllerInterface(radioControllerInterface)
{
	
}


void ServoControlTask::runTaskImpl()
{

	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		//Take the control values and set the servos to those control values. 
		
		
		
		
	}
}

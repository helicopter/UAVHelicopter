/*
 * PIDInnerLoopTask.cpp
 *
 * Created: 12/12/2013 7:51:58 PM
 *  Author: HP User
 */ 
#include "PIDInnerLoopTask.h"

using namespace helicopter::tasks;

PIDInnerLoopTask::PIDInnerLoopTask (PIDController *pidController, int delay, int period) :
Task(delay, period),
pidController(pidController)
{
	
}

void PIDInnerLoopTask::runTaskImpl()
{
	//pidController->cyclicLateralInnerLoopUpdate();
	pidController->cyclicLongitudeInnerLoopUpdate();
}

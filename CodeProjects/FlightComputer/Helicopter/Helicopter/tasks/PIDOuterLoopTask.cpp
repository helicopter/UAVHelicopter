/*
 * PIDOuterLoopTask.cpp
 *
 * Created: 10/13/2013 12:31:19 PM
 *  Author: HP User
 */ 
#include "PIDOuterLoopTask.h"

using namespace helicopter::tasks;

PIDOuterLoopTask::PIDOuterLoopTask (PIDController *pidController, int delay, int period) :
Task(delay, period),
pidController(pidController)
{
	
}

void PIDOuterLoopTask::runTaskImpl()
{
	pidController->tailRotorCollectiveOuterLoopUpdate();
}
	
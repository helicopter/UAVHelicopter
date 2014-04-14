/*
 * Task.cpp
 *
 * Created: 3/3/2013 8:25:24 PM
 *  Author: HP User
 */ 
#include <avr/interrupt.h>
#include "Task.h"

using namespace helicopter::tasks;

Task::Task(int delay, int period)
{
	this->delay = delay;
	this->period = period;
	this->isReadyToRun = false;
}

void Task::runTask()
{
	//Disable interrupts 
	//cli(); //removed because gps serial driver will miss a lot of bytes notifications if a task runs for a long time.
	
	//run task
	this->runTaskImpl();
	
	//Re-enable interrupts
	//sei();
}
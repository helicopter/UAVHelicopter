/*
 * Helicopter.cpp
 *
 * Created: 2/11/2013 10:33:54 PM
 *  Author: HP User
 */ 


#include <avr/io.h>

#include "FlashLEDTask.h"
#include "Scheduler.h"
#include "commonheader.h"

using namespace helicopter::tasks::misctasks;
using namespace helicopter::scheduler;

int main(void)
{
	bool isOn = false;
	
	while (1)
	{
		DDRA |= (1<<PA3);
		
		
		if (isOn)
		{
			PORTA &= ~(1<<PA3);
			isOn = false;
		}else
		{
			PORTA |= (1<<PA3);
			isOn = true;
		}
		
		_delay_ms(2000);
	}
	
	FlashLEDTask *flashTask = new FlashLEDTask(0, TIMER_FREQUENCY_HZ);//starting at tick 1, execute every 10 ticks.
	
	Scheduler *scheduler = Scheduler::getScheduler();
	
	scheduler->addTask(flashTask);
	
	scheduler->init(); //Sets up the timer registers, inits all tasks,
	
	scheduler->start();
	
	
	while(1)
	{
		scheduler->dispatch(); //ensure it includes processor sleeping.
	}
	
	return 0;	
}
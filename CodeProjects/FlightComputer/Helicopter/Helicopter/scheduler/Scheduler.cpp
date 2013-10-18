/*
 * Scheduler.cpp
 *
 * Created: 3/3/2013 8:30:31 PM
 *  Author: HP User
 */ 
#include <avr/sleep.h>
#include "Scheduler.h"


//set your clock speed
//#define F_CPU 1000000UL

using namespace helicopter::scheduler;

Scheduler* Scheduler::scheduler = NULL;

Scheduler* Scheduler::getScheduler()
{
	if (scheduler == NULL)
	{
		scheduler = new Scheduler(F_CPU, PRESCALE_BY_SIXTYFOUR, SCHEDULER_TICK_FREQUENCY_HZ);
	}

	return scheduler;
}

Scheduler::Scheduler(unsigned long cpuSpeed, PRESCALER prescaler, int schedulerTickFrequencyHz)
{
	targetTimerCount =  ((cpuSpeed / prescaler) / schedulerTickFrequencyHz);
	
	//Calculate the timer value to achieve the desired frequency. 
	this->targetTimerCount = targetTimerCount;

	this->prescaler = prescaler;
	
	numOfTasks = 0;
}

Scheduler::~Scheduler()
{
	for (int i = 0; i < numOfTasks; i++)
	{
		delete tasks[i];
		tasks[i] = NULL;
	}
	
	numOfTasks = 0;
}

int Scheduler::addTask(Task *task)
{
	if (numOfTasks < MAXNUMOFTASKS)
	{
		tasks[numOfTasks] = task;
		numOfTasks++;
		
		return 0;
	}

	return -1;
}

void Scheduler::init()
{
	for (int i = 0; i < numOfTasks; i++)
	{
		tasks[i]->init();
	}
	
	//setup timer
	OCR1A = targetTimerCount; //Set Clear Timer on Compare (auto reset) (CTC)
		
	TCCR1B |= (1 << WGM12); //Configure timer 1 for ctc mode
		
	TIMSK1 |= (1 << OCIE1A); //enable ctc interrupt
		
	sei(); //Enable global interrupts
}

void Scheduler::dispatch()
{
	Task *task = NULL;
		
	for (int i = 0; i < numOfTasks; i++)
	{
		task = tasks[i];
		
		if (task->getIsReadyToRun())
		{
			task->setIsReadyToRun(false);
			task->runTask();
		}
	}
	
	//Detect if a frame overrun occurred. I.e. it took longer than the allowed time to process the tasks.
	/*if ((TIFR1 & (1<<OCF1A)) != 0)
	{
		model->SystemModel->
	}*/
	
	//tells the processor to goto sleep to conserve power since no more
	//tasks are scheduled to be run until after the next scheduler interrupt(tick)
	//which will then wake the processor back up.
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
}

void Scheduler::start()
{
	//Setting TCCR to a prescaler will start the timer.
	switch(this->prescaler)
	{
		case 1:
			TCCR1B |= NoPrescaling;
			break;
		case 8:
			TCCR1B |= PrescaleByEight;
			break;
		case 64:
			TCCR1B |= PrescaleBySixtyFour;
			break;
		case 256:
			TCCR1B |= PrescaleByTwofiftysix;
			break;
		case 1024:
			TCCR1B |= PrescaleByTentwentyfour;
			break;
		default:
			TCCR1B |= NoPrescaling;	
	}
}

/**
 * Interrupt service routine for determining when tasks are ready to execute.
 */
ISR(TIMER1_COMPA_vect)
{
	//TODO do we want to stop interrupts in this method?
	Scheduler *scheduler = Scheduler::getScheduler();
	
	Task *task = NULL;
	
	//iterate through all the tasks and decriment how many
	//'ticks' are left before the task is ready to execute.
	for (int i = 0; i < scheduler->getNumOfTasks(); i++)
	{
		task = scheduler->getTasks()[i];
		
		if (task->getDelay() == 0)
		{
			//Mark the task as ready to run
			task->setIsReadyToRun(true);
			task->setDelay(task->getPeriod());
		}else
		{
			task->reduceDelay();
		}
	}
}

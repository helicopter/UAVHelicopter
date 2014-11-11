/*
 * Scheduler.cpp
 *
 * Created: 3/3/2013 8:30:31 PM
 *  Author: HP User
 */ 
#include <avr/sleep.h>
#include "Scheduler.h"

using namespace helicopter::scheduler;

Scheduler* Scheduler::scheduler = NULL;

Scheduler* Scheduler::getScheduler()
{
	if (scheduler == NULL)
	{
		scheduler = new Scheduler(F_CPU, PRESCALE_BY_TENTWENTYFOUR, SCHEDULER_TICK_FREQUENCY_HZ);
	}

	return scheduler;
}

Scheduler::Scheduler(unsigned long cpuSpeed, PRESCALER prescaler, int schedulerTickFrequencyHz)
{
	//Calculate the timer value to achieve the desired frequency.
	targetTimerCount =  ((cpuSpeed / prescaler) / schedulerTickFrequencyHz);

	this->targetTimerCount = targetTimerCount;

	this->prescaler = prescaler;
	
	numOfTasks = 0;
	
	blowFrameDetected = false;
	
	completedDispatch = true;
}

Scheduler::~Scheduler()
{
	//Delete the tasks in the list.
	for (int i = 0; i < numOfTasks; i++)
	{
		if (tasks[i] != NULL)
		{
			delete tasks[i];
			tasks[i] = NULL;
		}
	}
	
	numOfTasks = 0;
	
	if (scheduler != NULL)
	{
		delete scheduler;
		scheduler = NULL;
	}
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
	cli();
	
	for (int i = 0; i < numOfTasks; i++)
	{
		tasks[i]->init();
	}
	
	//setup timer
	OCR2A = targetTimerCount; //Set Clear Timer on Compare (auto reset) (CTC)
	TCCR2A |= (1 << WGM21); //Configure timer 2 for ctc mode
	TIMSK2 |= (1 << OCIE2A); //enable ctc interrupt for OCR0A
		
	sei(); //Enable global interrupts
}

void Scheduler::dispatch()
{
	completedDispatch = false;
	blowFrameDetected = false;
	
	Task *task = NULL;
		

	//Iterate through the tasks and if it's ready to run, execute the task. 
	for (int i = 0; i < numOfTasks; i++)
	{
		task = tasks[i];
		
		if (task->getIsReadyToRun())
		{
			task->setIsReadyToRun(false);
			task->runTask();
		}
	}

	completedDispatch = true;
	
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
			TCCR2B |= NoPrescaling;
			break;
		case 8:
			TCCR2B |= PrescaleByEight;
			break;
		case 64:
			TCCR2B |= PrescaleBySixtyFour;
			break;
		case 256:
			TCCR2B |= PrescaleByTwofiftysix;
			break;
		case 1024:
			TCCR2B |= PrescaleByTentwentyfour;
			break;
		default:
			TCCR2B |= NoPrescaling;	
	}
}

/**
 * Interrupt service routine for determining when tasks are ready to execute.
 */
ISR(TIMER2_COMPA_vect)
{
	Scheduler *scheduler = Scheduler::getScheduler();
	
	//Detect if the scheduler blew a frame
	if (!scheduler->hasCompletedDispatch())
	{
		scheduler->hasBlownFrame(true);
	}
	
	Task *task = NULL;
	
	//iterate through all the tasks and decrement how many
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

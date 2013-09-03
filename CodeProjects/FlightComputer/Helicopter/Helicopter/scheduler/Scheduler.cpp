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
		//scheduler = new Scheduler(124, 8);
		scheduler = new Scheduler(TARGET_TIMER_COUNT, TIMER_PRESCALER);
	}
	
	return scheduler;
}

Scheduler::Scheduler(int targetTimerCount, PRESCALER prescale)
{
	//Calculate the timer value to achieve the desired frequency. 
	this->targetTimerCount = targetTimerCount;

	this->prescaler = prescale;
	
	numOfTasks = 0;
}

Scheduler::~Scheduler()
{
	
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
		task = scheduler->getTasks()[i];
		
		if (task->getIsReadyToRun())
		{
			task->setIsReadyToRun(false);
			task->runTask();
		}
	}
	
	//TODO: put the processor to sleep.	
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
}

void Scheduler::start()
{
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

ISR(TIMER1_COMPA_vect)
{
	Scheduler *scheduler = Scheduler::getScheduler();
	
	Task *task = NULL;
	
	for (int i = 0; i < scheduler->getNumOfTasks(); i++)
	{
		task = scheduler->getTasks()[i];
		
		if (task->getDelay() == 0)
		{
			task->setIsReadyToRun(true);
			task->setDelay(task->getPeriod());
		}else
		{
			task->reduceDelay();
		}
	}
}

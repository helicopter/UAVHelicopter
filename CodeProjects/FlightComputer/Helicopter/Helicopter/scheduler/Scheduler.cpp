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
		//scheduler = new Scheduler(F_CPU, PRESCALE_BY_SIXTYFOUR, SCHEDULER_TICK_FREQUENCY_HZ);
		//scheduler = new Scheduler(F_CPU, PRESCALE_BY_TENTWENTYFOUR, SCHEDULER_TICK_FREQUENCY_HZ);
		scheduler = new Scheduler(F_CPU, PRESCALE_BY_TENTWENTYFOUR, SCHEDULER_TICK_FREQUENCY_HZ);
	}

	return scheduler;
}

Scheduler::Scheduler(unsigned long cpuSpeed, PRESCALER prescaler, int schedulerTickFrequencyHz)
{
	targetTimerCount =  ((cpuSpeed / prescaler) / schedulerTickFrequencyHz);
//	targetTimerCount =  (cpuSpeed / (2.0f * prescaler * schedulerTickFrequencyHz)) - 1;
//	targetTimerCount =  78;
//	targetTimerCount =  156;
//	targetTimerCount =  5;
	
	//Calculate the timer value to achieve the desired frequency. 
	this->targetTimerCount = targetTimerCount;

	this->prescaler = prescaler;
	
	numOfTasks = 0;
	
	blowFrameDetected = false;
	
	completedDispatch = true;
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
	cli();
	
	for (int i = 0; i < numOfTasks; i++)
	{
		tasks[i]->init();
	}
	
	//setup timer
	OCR2A = targetTimerCount; //Set Clear Timer on Compare (auto reset) (CTC)
//OCR0A=1;
		
//	TCCR0B |= (1 << WGM02); //Configure timer 1 for ctc mode
	TCCR2A |= (1 << WGM21); //Configure timer 2 for ctc mode
		
	TIMSK2 |= (1 << OCIE2A); //enable ctc interrupt for OCR0A
		
	sei(); //Enable global interrupts
}

void Scheduler::dispatch()
{
	completedDispatch = false;
	blowFrameDetected = false;
	
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
//TCCR0B |= (1 << CS02);
//TCCR0B &= ~((1 << CS01) | (1 << CS00));
	
	//TCCR0B = (1 << CS02) | (1 << CS00);
}

/**
 * Interrupt service routine for determining when tasks are ready to execute.
 */
ISR(TIMER2_COMPA_vect)
{
	//TODO do we want to stop interrupts in this method?
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

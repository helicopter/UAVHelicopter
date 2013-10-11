/*
 * SchedulerTest.cpp
 *
 * Created: 3/3/2013 6:25:41 PM
 *  Author: HP User
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "UnitTestUtils.h"
#include "TestCase.h"
#include "CommonHeader.h"
#include "SchedulerTest.h"
#include "Task.h"
#include "Scheduler.h"


using namespace helicoptertestscommon::util::testframework;
using namespace helicopter::tasks;
using namespace helicopter::scheduler;


class CounterTask : public Task
{
public:
	int counter;

	CounterTask (int delay, int period) : Task(delay, period)
	{
		counter = 0;
	}
	
	void init()
	{

	}
	
	void runTaskImpl() {
		counter++;
	}
};


int schedulertask_test(TestCase *test)
{	
	CounterTask *counterTask1 = new CounterTask(1, 10);//starting at tick 1, execute every 10 ticks. 
	Task *counterTask2 = new CounterTask(5, 10);
	
	
	Scheduler *scheduler = Scheduler::getScheduler();
	//Scheduler scheduler(12, 8);
	
	scheduler->addTask(counterTask1);
	scheduler->addTask(counterTask2);
	
	scheduler->init(); //Sets up the timer registers, inits all tasks,
	
	AssertTrue2(counterTask1->counter == 0, 1);
	
	int previousCounter = 0;
	
	scheduler->start();
	
	
	for (int i = 0; i < 10; i++)
	{
		//_delay_ms(2);
		
		
		//TODO: Implement the dispatcher so that the tasks are actually called
		scheduler->dispatch(); //ensure it includes processor sleeping.
	
		//AssertTrue(counterTask1->counter  > previousCounter, 2);
		//previousCounter = counterTask1->counter;	
		
		//TODO:
		//Include some kind of timer test here to ensure that the timers
		//are actually being called. 
		
		//TODO: 
		//include a precise timing test to ensure that precise timing is being performed.
	}
	AssertTrue2(counterTask1->counter  > previousCounter, 2);
	
	return 0;
};


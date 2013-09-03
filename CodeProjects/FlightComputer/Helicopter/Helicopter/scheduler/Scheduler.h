/*
 * Scheduler.h
 *
 * Created: 3/3/2013 8:12:18 PM
 *  Author: HP User
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "Task.h"
#include "commonheader.h"


using namespace helicopter::tasks;

namespace helicopter
{
	namespace scheduler
	{
		class Scheduler
		{
			private:
			
			
				static Scheduler *scheduler;
			
			    //TODO Switch this to an ENUM.
				static const int NoPrescaling = 1;
				static const int PrescaleByEight = (1 << CS11);
				static const int PrescaleBySixtyFour = (1 << CS10) | (1 << CS11);
				static const int PrescaleByTwofiftysix = (1 << CS12);
				static const int PrescaleByTentwentyfour = (1 << CS12) | (1 << CS10);
						
			
				//TODO: Remove this and just use the heap.
				static const int MAXNUMOFTASKS = 10;
			
				Task *tasks [MAXNUMOFTASKS];
				
				int numOfTasks;
				
				//This is the value feed to the timer to meet the 
				//desired frequency.
				int targetTimerCount;
				
				PRESCALER prescaler;
				
				/*
				 * Prescaler must be 1,8,24,256, or 1024                                
				 */
				Scheduler(int targetTimerCount, PRESCALER prescaler);
				~Scheduler();
			
			public:
				

				static Scheduler  *getScheduler();
			


			
				int addTask(Task *task);
			
				/*
				* Iterates through the tasks calling their init methods
				* and sets up the timer for execution, but does not
				* start the timer.
				* The timer thats used is timer 1 (TCCR1B)
				*/
				void init();
			
				void dispatch();
			
			    /*
				* Starts the timer for task execution.
				*/
				void start();
				
				int getNumOfTasks()
				{
					return numOfTasks;
				}
				
				Task **getTasks()
				{
					return tasks;
				}
		};
	}
}




#endif /* SCHEDULER_H_ */
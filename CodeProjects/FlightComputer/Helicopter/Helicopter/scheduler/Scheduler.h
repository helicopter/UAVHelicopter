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
#include "CommonHeader.h"


using namespace helicopter::tasks;

namespace helicopter
{
	namespace scheduler
	{
		/**
		 * This class represents a scheduler used to schedule tasks to execute at precise intervals.
		 * This class uses a timer to setup an interrupt routine. That routine gets kicked off at
		 * a given frequency. The tasks which are to be executed at that frequency are then marked
		 * as being ready to be executed within the interrupt service handler. However
		 * the execution of the task isn't done until the dispatcher gets called in the main
		 * routine (outside of the interrupt handler)
		 */
		class Scheduler
		{
			private:
			
			
				//static Scheduler *scheduler;
			
			    //TODO Switch this to an ENUM.
				
				
				//Prescalers for different timers are different
				static const int NoPrescaling = 1;
				static const int PrescaleByEight = (1 << CS21);
				static const int PrescaleBySixtyFour = (1 << CS22);
				static const int PrescaleByTwofiftysix = (1 << CS22) | (1 << CS21);
				static const int PrescaleByTentwentyfour = (1 << CS22) | (1<< CS21) | (1 << CS20);
				
				/*static const int NoPrescaling = 1;
				static const int PrescaleByEight = (1 << CS21);
				static const int PrescaleBySixtyFour = (1 << CS20) | (1 << CS21);
				static const int PrescaleByTwofiftysix = (1 << CS22);
				static const int PrescaleByTentwentyfour = (1 << CS22) | (1 << CS20);*/
				
						
			
				//TODO: Remove this and just use the heap.
				static const int MAXNUMOFTASKS = 25;
			
				Task *tasks [MAXNUMOFTASKS];
				
				int numOfTasks;
				
				//This is the value feed to the timer to meet the 
				//desired frequency.
				int targetTimerCount;
				
				//This is a value feed to the timer to prescale the clock by this number
				PRESCALER prescaler;
				
				//Flag for indicating that the processing of the dispatched tasks took longer then the
				//time allowed. I.e. the timer triggered again before the tasks being dispatched were done
				//processing. (Thereby interrupting those dispatching tasks.)
				bool blowFrameDetected;
				
				//flag which indicates that the processor completed dispatching the tasks.
				//This is used to detect if a frame was blown because this flag will be
				//false if the dispatching didn't complete by the time the timer triggered again.
				bool completedDispatch;
				
				static Scheduler *scheduler;
				
				/**
				 * Create a scheduler object.
				 * The variables for constructing the scheduler to run at a certain time
				 * should be set in a specific manner to adhere to this formula:
				 * Target Timer Count = (((Input Frequency / Prescaler) / Target Frequency) 
				 * Where target timer count is the number that the timer will use to
				 * determine if the interrupt routine will execute.
				 * @param cpuSpeed The speed of the processor (e.g. 16,000,000 hz)
				 * @param prescaler The prescaler to be applied to the scheduler's timer
				 * @param schedulerTickFrequencyHz the desired frequency that the scheduler should run at.
				 * This will be how frequently per second the interrupt routine which drives
				 * the scheduler will get called (tick).
				 * Examples:
				 * Target Timer Count = (((Input Frequency / Prescaler) / Target Frequency)
				 * (((16000000 / 64) / 200)) = 1250
				 * (((1000000 / 8) / 200)) = 625
				 *
				 * Because this is now using an 8 bit timer, the scheduler tick freq cant be less than 70.
				*/
				Scheduler(unsigned long cpuSpeed, PRESCALER prescaler, int schedulerTickFrequencyHz);
				~Scheduler();
			
			
			public:
				
				/**
				 * If the Scheduler hasn't been created yet, then construct the 
				 * scheduler pseudo-'singleton' and return it.
				 */
				static Scheduler  *getScheduler();
				
				
				/**
				 * Adds a task to the scheduler.
				 * @param task the task to add to the scheduler to be scheduled
				 * @return -1 if the task list is full, 0 otherwise.
				 */
				int addTask(Task *task);
			
				/*
				* Iterates through the tasks calling their init methods
				* and sets up the timer for execution, but does not
				* start the timer.
				* The timer thats used is timer 1 (TCCR1B)
				*/
				void init();
			
				/**
				 * When called, it iterates through the tasks, and for each task that has been marked ready to
				 * run, dispatches (executes) that task. This should be called in the
				 * 'main' loop. Whereas the determination for which tasks should be marked as ready
				 * to execute is done in the interrupt service handler.
				 */
				void dispatch();
			
			    /*
				* Starts the timer for determining which tasks are ready to run.
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
				
				/**
				 * Detects if the processing was completed before the next scheduler triggered.
				 * @return True if the scheduler timer triggered again before the tasks were fully processes,
				 * False if all the tasks were finished processing (i.e. the dispatch method finished) before
				 * the timer triggered again.
				 */
				bool hasBlownFrame()
				{
					return blowFrameDetected;
				}
				
				/**
				 * Sets whether a blown frame was detected. (I.e. the dispatcher didn't finish before
				 * the scheduler timmer was triggered again)
				 * @param blownFrameDetected true if a frame was blown, false otherwise.
				 */
				void hasBlownFrame(bool blowFrameDetected)
				{
					this->blowFrameDetected = blowFrameDetected;
				}
				
				/**
				 * Returns a flag indicating that the dispatch method has completed. This is for
				 * use by the scheduler to determine if the dispatcher blew a frame.
				 * @return true if the dispatch method completed, false otherwise. 
				 */
				bool hasCompletedDispatch()
				{
					return completedDispatch;
				}
		};
	}
}




#endif /* SCHEDULER_H_ */
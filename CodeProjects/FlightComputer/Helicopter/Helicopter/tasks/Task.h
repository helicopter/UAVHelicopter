/*
 * Task.h
 *
 * Created: 3/3/2013 7:50:04 PM
 *  Author: HP User
 */ 


#ifndef TASK_H_
#define TASK_H_

namespace helicopter
{
	
	namespace tasks
	{
		class Task
		{
			private:
				/**
				 * This represents how many ticks left until this task executes.
				 * I.e. if this task goes off every 200 ticks, but 50 ticks
				 * have gone by since the last execution, then this value
				 * will be 150.
				 */
				int delay;
				
				/*
				* This represents how many ticks will go by between executions of this task.
				* i.e. if this task should go off every 200 ticks, this value will be 200.
				*/
				int period;
				bool isReadyToRun;
				
			protected:
				/**
				* Implemented by subclasses for the actual logic to execute
				*/
				virtual void runTaskImpl(){;}
				
			public:
				/**
				 * Create a task to be executed by the scheduler
				 * @param delay A task can be delayed by the Delay parameter's number of scheduler 'ticks'
				 * E.g. for a dely of 3, and a period of 5, then the first time the task will execute
				 * is 8 ticks from now, and then every 5 ticks after that. After that point
				 * the delay becomes a counter for how many more ticks until the next
				 * execution of the task.
				 * @param period The number of scheduler 'ticks' to be executed before
				 * this task is ready to run.
				 */
				Task (int delay, int period);
				
				virtual ~Task(){}
				
				/**
				 * Function to initialize the task before execution.
				 */
				virtual void init(){;}
					
				/**
				 * Executes the task. First disables intterrupts and then calls
				 * subclasses runTaskImpl to execute the actual task logic.
				 */
				virtual void runTask();
				
					
				int getDelay()
				{
					return delay;
				}
				
				int getPeriod()
				{
					return period;
				}
				
				void setDelay(int delay)
				{
					this->delay = delay;
				}
				
				bool getIsReadyToRun()
				{
					return isReadyToRun;
				}
				
				void setIsReadyToRun(bool isReadyToRun)
				{
					this->isReadyToRun = isReadyToRun;
				}
				
				/**
				 * Reduces the delay by one tick.
				 */
				void reduceDelay()
				{
					delay--;
				}
		};
	}
}




#endif /* TASK_H_ */
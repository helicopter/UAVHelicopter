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
				int delay;
				int period;
				bool isReadyToRun;
				
			protected:
				virtual void runTaskImpl(){;}
				
			public:
				Task (int delay, int period);
				virtual void init(){;}
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
				
				void reduceDelay()
				{
					delay--;
				}
		};
	}
}




#endif /* TASK_H_ */
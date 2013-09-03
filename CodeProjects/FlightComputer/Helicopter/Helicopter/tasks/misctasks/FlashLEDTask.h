/*
 * LedFlashTask.h
 *
 * Created: 4/10/2013 11:36:46 PM
 *  Author: HP User
 */ 


#ifndef FlashLEDTASK_H_
#define FlashLEDTASK_H_

#include <avr/io.h>
#include <util/delay.h>

#include "Task.h"

using namespace helicopter::tasks;

namespace helicopter
{
	
	namespace tasks
	{
		namespace misctasks
		{
			

			class FlashLEDTask : public Task
			{
				private:
					bool isOn;
				
				public:

				FlashLEDTask (int delay, int period) : 
					Task(delay, period),
					isOn(false)
				{
					
				}
	
				void init()
				{
					DDRA |= (1<<PA3);
				}
	
				void runTaskImpl() {
					if (isOn)
					{
						PORTA &= ~(1<<PA3);
						isOn = false;
					}else
					{
						PORTA |= (1<<PA3);
						isOn = true;
					}
				}
			};
		}
	}
}



#endif /* FlashLEDTASK_H_ */
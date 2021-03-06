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
		/**
			* This task is used to periodically flash an LED on the flight computer.
			*/
		class FlashLEDTask : public Task
		{
			private:
				static const int BLUELED = PA3;
				static const int YELLOWLED = PA4;
				static const int REDLED = PA5;
				
				bool isOff;
					
				int ledToBlink;
				
			public:

			FlashLEDTask (int delay, int period, int ledToBlink = BLUELED) : 
				Task(delay, period),
				isOff(false),
				ledToBlink(ledToBlink)
			{
					
			}
	
			void init()
			{
				DDRA |= (1<<ledToBlink);
			}
	
			/**
				* Flashes the LED
				*/
			void runTaskImpl() {
				if (isOff)
				{
					//Turn On the LED
					PORTA &= ~(1<<ledToBlink);
					isOff = false;
				}else
				{
					//Turn Off the LED
					PORTA |= (1<<ledToBlink);
					isOff = true;
				}
			}
		};
	}
}



#endif /* FlashLEDTASK_H_ */
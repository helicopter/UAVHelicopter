/*
 * Timer.h
 *
 * Created: 9/14/2013 6:40:22 PM
 *  Author: HP User
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "CommonHeader.h"

namespace helicopter
{
	namespace util
	{
		/**
		 * Helper class for representing a timer for measuring timeouts.
		 * TODO This should probably be a singleton since under the covers theres only one timer
		 * so you should be able to create multiple of these.
		 */
		class Timer
		{
			private:
				static const int NoPrescaling = 1;
				static const int PrescaleByEight = (1 << CS01);
				static const int PrescaleBySixtyFour = (1 << CS00) | (1 << CS01);
				static const int PrescaleByTwofiftysix = (1 << CS02);
				static const int PrescaleByTentwentyfour = (1 << CS02) | (1 << CS00);
				
				unsigned long cpuSpeed;
				PRESCALER timerPrescaler;
				int timeoutMilliseconds;
				
				int numberOfTimeouts;
				
			
			public:
			
				//This means that the timer can't be used by multiple threads. since this is single threaded it works
				//just don't use the timer in an interrupt.
				static int timeoutCounter;

				/**
				 * timeoutMilliseconds has to be <= 1000.
				 * @param cpuSpeed The speed that the cpu processor is running at
				 * @param timerPrescaler The prescaler that should be used for this processor
				 * @timeoutMilliseconds the milliseconds that should go by before the
				 * timer times out. This value must be under 1000.
				 * These variables will have to adhere to this equation:
				 * OCR = ((cpuSpeed /  timerPrescaler) / frequency)
				 *
				 * Now that this is using an 8 bit timer, prescaler has to be 1024, and timeout millizeconds
				 * has to be divisible by 10 and greater than 10. E.g. 10, 20, 30, 200, etc. 
				 */
				Timer (unsigned long cpuSpeed, PRESCALER timerPrescaler, unsigned long timeoutMilliseconds):
					cpuSpeed(cpuSpeed), timerPrescaler(timerPrescaler), timeoutMilliseconds(timeoutMilliseconds),
					numberOfTimeouts(0)
					{
						
					}
					
				
				/**
				* Start the timer. 
				* This method will ensure that the timer is stopped (by calling stop()) before it starts.
				*/
				void startTimer();
				
				/**
				 * Determines if the timer has timed out
				 * @return true if the timer has timed out
				 * false if it has not timed out, or if the timer wasn't started.
				 */
				bool hasTimedout();
				
				/**
				 * Stops the timer, and resets the clock.
				 */
				void stopTimer();
		};
	}
}




#endif /* TIMER_H_ */
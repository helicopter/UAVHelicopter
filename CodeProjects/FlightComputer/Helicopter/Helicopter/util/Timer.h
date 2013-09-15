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
				static const int PrescaleByEight = (1 << CS31);
				static const int PrescaleBySixtyFour = (1 << CS30) | (1 << CS31);
				static const int PrescaleByTwofiftysix = (1 << CS32);
				static const int PrescaleByTentwentyfour = (1 << CS32) | (1 << CS30);
				
				unsigned long cpuSpeed;
				int timerPrescaler;
				int timeoutMilliseconds;
			
			public:

				/**
				 * timeoutMilliseconds has to be <= 1000.
				 */
				Timer (unsigned long cpuSpeed, int timerPrescaler, unsigned long timeoutMilliseconds):
					cpuSpeed(cpuSpeed), timerPrescaler(timerPrescaler), timeoutMilliseconds(timeoutMilliseconds)
					{
						
					}
					
					
				void startTimer();
				
				bool hasTimedout();
				
				void stopTimer();
		};
	}
}




#endif /* TIMER_H_ */
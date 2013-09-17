/*
 * Timer.cpp
 *
 * Created: 9/14/2013 6:40:35 PM
 *  Author: HP User
 */ 
#include "Timer.h"

using namespace helicopter::util;

void Timer::startTimer()
{
	//stop timer in case it was already started.
	stopTimer();
	
	int frequency = 1000 / timeoutMilliseconds;
	
	//setup timer. This value has to be less than the max number of a 16 bit numer 65535.
	OCR3A = ((cpuSpeed /  timerPrescaler) / frequency); //Set Clear Timer on Compare (auto reset) (CTC)
		
	TCCR3B |= (1 << WGM32); //Configure timer 1 for ctc mode
		
	TIMSK3 |= (1 << OCIE3A); //enable ctc interrupt
		
	switch(timerPrescaler)
	{
		case 1:
		TCCR3B |= NoPrescaling;
		break;
		case 8:
		TCCR3B |= PrescaleByEight;
		break;
		case 64:
		TCCR3B |= PrescaleBySixtyFour;
		break;
		case 256:
		TCCR3B |= PrescaleByTwofiftysix;
		break;
		case 1024:
		TCCR3B |= PrescaleByTentwentyfour;
		break;
		default:
		TCCR3B |= NoPrescaling;
	}
}

bool Timer::hasTimedout()
{
	return (TIFR3 & (1<<OCF3A)) != 0;
}

void Timer::stopTimer()
{
	//stop the timer
	TCCR3B = 0;
	
	//clear the timeout flag.
	TIFR3 |= (1<<OCF3A);
}
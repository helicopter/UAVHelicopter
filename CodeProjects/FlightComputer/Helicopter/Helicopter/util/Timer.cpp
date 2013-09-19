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
	
	int frequency = 0;
	
	if (timeoutMilliseconds > 1000) 
	{
		frequency = 1;
	}else
	{
		frequency = 1000 / timeoutMilliseconds;
	}

	//setup timer.
	//set the output compare register so that when the Timer Counter (TCNT) matches this output register
	//it indicates that the timer has timed out.
	OCR3A = ((cpuSpeed /  timerPrescaler) / frequency); 
		
	TCCR3B |= (1 << WGM32); //Configure timer 1 for Clear on Timer Compare (CTC) mode so that if there is a match, the TCNT goes back to 0.
		
	//Set the Timer Counter Control Register (TCCR) with the appropriate prescaller
	//which causes the timer counter to start (i.e. the timer starts).
	switch(timerPrescaler)
	{
		case NO_PRESCALER:
		TCCR3B |= NoPrescaling;
		break;
		case PRESCALE_BY_EIGHT:
		TCCR3B |= PrescaleByEight;
		break;
		case PRESCALE_BY_SIXTYFOUR:
		TCCR3B |= PrescaleBySixtyFour;
		break;
		case PRESCALE_BY_TWOFIFTYSIX:
		TCCR3B |= PrescaleByTwofiftysix;
		break;
		case PRESCALE_BY_TENTWENTYFOUR:
		TCCR3B |= PrescaleByTentwentyfour;
		break;
		default:
		TCCR3B |= NoPrescaling;
	}
}

bool Timer::hasTimedout()
{
	//Check the Timer Interrupt Flags Register (TIFR) to determine if the flag was set indicating
	//that the timer has timed out.
	return (TIFR3 & (1<<OCF3A)) != 0;
}

void Timer::stopTimer()
{
	//stop the timer
	TCCR3B = 0;
	
	//Then clear the timer counter (sets the count back to 0 so when the timer starts again, it starts from 0). 
	//Clear after stopping in order to avoid missing a compare match.
	TCNT3 = 0;
	
	//clear the timeout flag.
	TIFR3 |= (1<<OCF3A);
}
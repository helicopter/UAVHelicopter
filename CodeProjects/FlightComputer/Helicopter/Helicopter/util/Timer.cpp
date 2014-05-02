/*
 * Timer.cpp
 *
 * Created: 9/14/2013 6:40:35 PM
 *  Author: HP User
 */ 
#include "Timer.h"

using namespace helicopter::util;

int Timer::timeoutCounter = 0;


void Timer::startTimer()
{
	//stop timer in case it was already started.
	stopTimer();
	
	/*int frequency = 0;
	
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
	OCR0A = ((cpuSpeed /  timerPrescaler) / frequency);
	*/ 
	OCR0A = ((cpuSpeed /  timerPrescaler) / 100); //fire off every 10 ms. 
	
	numberOfTimeouts = timeoutMilliseconds / 10;
	timeoutCounter = 0;
		
	//Configure timer 1 for Clear on Timer Compare (CTC) mode so that if there is a match, the TCNT goes back to 0.
	//note, this is different than the 8 bit timer which sets wgmn1 on tccrna to set CTC.
	TCCR0A |= (1 << WGM01); 
		
		
	//TCCR3A |= (1 << WGM31);	//Configure timer 1 for Clear on Timer Compare (CTC) mode so that if there is a match, the TCNT goes back to 0.
		
	
	
	TIMSK0 |= (1 << OCIE0A); //enable ctc interrupt for OCR0A
		
		
		
	//Set the Timer Counter Control Register (TCCR) with the appropriate prescaller
	//which causes the timer counter to start (i.e. the timer starts).
	switch(timerPrescaler)
	{
		case NO_PRESCALER:
		TCCR0B |= NoPrescaling;
		break;
		case PRESCALE_BY_EIGHT:
		TCCR0B |= PrescaleByEight;
		break;
		case PRESCALE_BY_SIXTYFOUR:
		TCCR0B |= PrescaleBySixtyFour;
		break;
		case PRESCALE_BY_TWOFIFTYSIX:
		TCCR0B |= PrescaleByTwofiftysix;
		break;
		case PRESCALE_BY_TENTWENTYFOUR:
		TCCR0B |= PrescaleByTentwentyfour;
		break;
		default:
		TCCR0B |= NoPrescaling;
	}
	
	sei();
}

bool Timer::hasTimedout()
{
	//Check the Timer Interrupt Flags Register (TIFR) to determine if the flag was set indicating
	//that the timer has timed out.
	//return (TIFR0 & (1<<OCF0A)) != 0;
	
	return timeoutCounter>=numberOfTimeouts;
}

void Timer::stopTimer()
{
	//Disable interrupt.
	TIMSK0 &= ~(1 << OCIE0A);
	
	//stop the timer
	TCCR0B = 0;
	
	//Then clear the timer counter (sets the count back to 0 so when the timer starts again, it starts from 0). 
	//Clear after stopping in order to avoid missing a compare match.
	TCNT0 = 0;
	
	//clear the timeout flag.
	TIFR0 |= (1<<OCF0A);
	
	timeoutCounter = 0;
}

ISR(TIMER0_COMPA_vect)
{
	Timer::timeoutCounter++;
}
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
	int frequency = 1000 / timeoutMilliseconds;
	
	
	//setup timer. This value has to be less than the max number of a 16 bit numer 65535.
	OCR3A = ((cpuSpeed /  timerPrescaler) / frequency); //Set Clear Timer on Compare (auto reset) (CTC)
//OCR1A = 2551;
//timerPrescaler=64;
//OCR1A = 31250;
//timerPrescaler=1024;
//OCR1A = 15625;
//timerPrescaler=1024;
		
	TCCR3B |= (1 << WGM32); //Configure timer 1 for ctc mode
		
	TIMSK3 |= (1 << OCIE3A); //enable ctc interrupt
		
	//sei(); //Enable global interrupts
	//The corresponding
	//Interrupt Vector (see “Interrupts” on page 105) is executed when the OCFnA Flag, located in
	//TIFRn, is set.
	
	
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
	TCCR3B = 0;
	
	TIFR3 |= (1<<OCF3A);
}
/*
 * TimerTests.cpp
 *
 * Created: 9/14/2013 6:27:33 PM
 *  Author: HP User
 */ 

#include "Timer.h"
#include "TimerTests.h"
#include "UnitTestUtils.h"
#include "CommonHeader.h"

using namespace helicopter::util;





int timeout_test(TestCase *test)
{	
	bool isOn = false;
	
	Timer t(F_CPU,PRESCALE_BY_TENTWENTYFOUR,500);
	
	t.startTimer();
	
	//turn yellow LED off.
	PORTA |= (1<<PA4);
	
	while (true)
	{

		
		if (t.hasTimedout())
		{
			t.stopTimer();
			

			if (isOn)
			{
				PORTA |= (1<<PA4);
				isOn = false;
			}else
			{
				PORTA &= ~(1<<PA4);
				isOn = true;
			}
			
			//restart
			t.startTimer();

		}
	}
	
	
	return 0;
}


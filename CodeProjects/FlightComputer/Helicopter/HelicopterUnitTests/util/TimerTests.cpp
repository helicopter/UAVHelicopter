/*
 * TimerTests.cpp
 *
 * Created: 9/14/2013 6:27:33 PM
 *  Author: HP User
 */ 

#include "Timer.h"
#include "TimerTests.h"
#include "UnitTestUtils.h"
#include "commonheader.h"

using namespace helicopter::util;


bool isOn = false;

unsigned int counter = 0;


int timeout_test(TestCase *test)
{	
	bool hastimedout = false;
	Timer t(F_CPU,1024,500);
	
	t.startTimer();
	
PORTA |= (1<<PA4);
	//PORTA &= ~(1<<PA4);
	while (true)
	{
		/*if (counter++ == 65524)
		{
			counter = 0;
						if (isOn)
						{
							PORTA &= ~(1<<PA4);
							isOn = false;
						}else
						{
							PORTA |= (1<<PA4);
							isOn = true;
						}
		}*/
		
		
		if (t.hasTimedout())
		{
	//		PORTA &= ~(1<<PA4);
		/*	hastimedout = true;
			PORTA &= ~(1<<PA4);
		}
		if (hastimedout)
		{
			//PORTA &= ~(1<<PA4);
		}*/
			t.stopTimer();
			
			//if (counter++ == 1)
			{
				counter = 0;
				if (isOn)
				{
					PORTA &= ~(1<<PA4);
					isOn = false;
				}else
				{
					PORTA |= (1<<PA4);
					isOn = true;
				}
			}
			
			/*if (isOn)
			{
				PORTA &= ~(1<<PA4);
				isOn = false;
			}else
			{
				PORTA |= (1<<PA4);
				isOn = true;
			}
			*/
			//restart
			t.startTimer();

		}
	}
	

	
	/*
	Timer t(F_CPU,8,500);
	
	t.startTimer();
	
	AssertTrue(t.hasTimedout() == false, 1);
	
	for (unsigned int i = 0; i < 10; i++)
	{
		
	}
	
	AssertTrue(t.hasTimedout(), 2);
	
	t.stopTimer();
	*/
	
	return 0;
}


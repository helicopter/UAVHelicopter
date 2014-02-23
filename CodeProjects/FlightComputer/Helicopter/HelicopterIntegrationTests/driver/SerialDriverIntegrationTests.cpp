/*
 * SerialDriverTests.cpp
 *
 * Created: 9/1/2013 7:34:13 PM
 *  Author: HP User
 */ 

#include "SerialDriverIntegrationTests.h"
#include "SerialDriver.h"
#include "UnitTestUtils.h"
#include "Timer.h"

#include <string.h>

using namespace helicopter::drivers;
using namespace helicopter::util;

int serialreceivetransmit_test(TestCase *test)
{
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 75);
	
	SerialDriver serialDriver (38400, SerialDriver::Zero, false);
	serialDriver.initialize();
	
	byte transmitVal = 127;
	AssertTrue2(serialDriver.transmit(transmitVal, t) == 0, 1);
	
	byte receiveVal = 0;

	AssertTrue2(serialDriver.receive(receiveVal, t) == 0, 2);

	AssertTrue2(receiveVal == transmitVal, 3);
	
	for (byte i = 0; i < 127; i++)
	{
		transmitVal = i;
		
		AssertTrue2(serialDriver.transmit(transmitVal, t) == 0, 4);

		AssertTrue2(serialDriver.receive(receiveVal, t) == 0, 5);
			
		AssertTrue2(receiveVal == transmitVal, 6);
	}
	
	
	//Transmit 'success' message.
	transmitVal = 12;
	AssertTrue2(serialDriver.transmit(transmitVal, t) == 0, 7);
	
	return 0;
}
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
/*	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 75);
	
	SerialDriver serialDriver (38400, SerialDriver::Zero, t, false);
	serialDriver.initialize();
	
	byte transmitVal = 127;
	AssertTrue2(serialDriver.transmitByte(transmitVal) == 0, 1);
	
	byte receiveVal = 0;

	AssertTrue2(serialDriver.receiveByte(receiveVal) == 0, 2);

	AssertTrue2(receiveVal == transmitVal, 3);
	
	for (byte i = 0; i < 127; i++)
	{
		transmitVal = i;
		
		AssertTrue2(serialDriver.transmitByte(transmitVal) == 0, 4);

		AssertTrue2(serialDriver.receiveByte(receiveVal) == 0, 5);
			
		AssertTrue2(receiveVal == transmitVal, 6);
	}
	
	
	//Transmit 'success' message.
	transmitVal = 12;
	AssertTrue2(serialDriver.transmitByte(transmitVal) == 0, 7);
	
	return 0;*/

Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 75);
	SerialDriver serialDriver (57600, SerialDriver::Zero, t, false);
	serialDriver.initialize();
	
	byte buff[4];

	serialDriver.receiveByte(buff[0]);
	serialDriver.receiveByte(buff[1]);
	serialDriver.receiveByte(buff[2]);
	serialDriver.receiveByte(buff[3]);
	
	float val = 0.0d;
	
	memcpy(&val, buff, 4);
	
	byte transmitVal = 2;
	
	if (val > -123.0 && val < -121.0)
	{
		transmitVal = 1;
	}
	if (val == -122.21683502197266f)
	{
		transmitVal = 3;
	}
	
	byte buff2[4];
	
	memcpy(buff2, &val, 4);
	
	serialDriver.transmitByte(buff2[0]);
	serialDriver.transmitByte(buff2[1]);
	serialDriver.transmitByte(buff2[2]);
	serialDriver.transmitByte(buff2[3]);
	
	return 0;
}
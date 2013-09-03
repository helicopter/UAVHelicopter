/*
 * SerialDriverTests.cpp
 *
 * Created: 9/1/2013 7:34:13 PM
 *  Author: HP User
 */ 

#include "SerialDriverIntegrationTests.h"
#include "SerialDriver.h"
#include "UnitTestUtils.h"

using namespace helicopter::drivers;

int serialreceivetransmit_test(TestCase *test)
{
	SerialDriver serialDriver (38400, SerialDriver::Zero, false);
	serialDriver.initialize();
	
	byte transmitVal = 127;
	AssertTrue(serialDriver.transmitByte(transmitVal) == 0, 1);
	
	byte receiveVal = 0;

	AssertTrue(serialDriver.receiveByte(receiveVal) == 0, 2);

	AssertTrue(receiveVal == transmitVal, 3);
	
	for (byte i = 0; i < 127; i++)
	{
		transmitVal = i;
		
		AssertTrue(serialDriver.transmitByte(transmitVal) == 0, 4);

		AssertTrue(serialDriver.receiveByte(receiveVal) == 0, 5);
			
		AssertTrue(receiveVal == transmitVal, 6);
	}
	
	
	//Transmit 'success' message.
	transmitVal = 12;
	AssertTrue(serialDriver.transmitByte(transmitVal) == 0, 7);
	
	return 0;
}
/*
 * GPSSensorTests.cpp
 *
 * Created: 2/23/2014 9:48:36 PM
 *  Author: HP User
 */ 
#include <math.h>
#include <util/delay.h>

#include "GPSSensorTests.h"
#include "UnitTestUtils.h"
#include "ImuSensorTests.h"
#include "SPIDriver.h"
#include "IMUSensor.h"
#include "Timer.h"
#include "SerialDriver.h"
#include "GPSSensor.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;

int readgps_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	
	/**
	 * Note, this serial driver must be on baud rate 9600 because the GPS code is
	 * hard coded to communicate over serial at 9600 baud.
	 */
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	gpsSerialDriver->initialize();


	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();

	AssertTrue(gpsSensor->getLattitude() == 0.0f);
	AssertTrue(gpsSensor->getLongitude() == 0.0f);	
	
	//Wait until the GPS sensor is ready.
	while (!gpsSensor->isGpsReady())
	{
		_delay_ms(1000);
	}
	
	AssertTrue(gpsSensor->readSensor());
	
	AssertTrue(gpsSensor->getLattitude() != 0.0f);
	AssertTrue(gpsSensor->getLongitude() != 0.0f);
	
	while (true)
	{
		_delay_ms(50);
		AssertTrue(gpsSensor->readSensor());
		
		serialDriver->transmit('S');

		serialDriver->transmit(gpsSensor->getLattitude());
		serialDriver->transmit(gpsSensor->getLongitude());
		
	}
	//
	/**
	 * TODO Notes: I'll want to investigate the 'correction' fields.
	 * perhaps they have a way of correcting the drift I aws seeing and will
	 * give me a more accurate reading. 
	 * I'll want to be sure to implement the position fix flags to ensure
	 * that the GPS is actually activated and has a good fix. (adobe page 51)
	 */
	
	return 0;
}


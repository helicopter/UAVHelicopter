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
#include "BarometerSensor.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;

int readgps_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::Zero, true);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true);
	serialDriver->initialize();
//
	//GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	//gpsSensor->init();
	//
	////Delay for 30 seconds
	//for (int i = 0; i < 30);
	//{
		//_delay_ms(1000);
	//}
//
	//
	//AssertTrue(baroSensor->getLattitude() == 0.0f);
	//AssertTrue(baroSensor->getLongitude() == 0.0f);	
	//
	//AssertTrue(gpsSensor->isGpsReady());
	//
	//AssertTrue(gpsSensor->readSensor());
	//
	//AssertTrue(baroSensor->getLattitude() != 0.0f);
	//AssertTrue(baroSensor->getLongitude() != 0.0f);
	//
	//while (true)
	//{
		//_delay_ms(50);
		//baroSensor->readSensor();
		//
		//serialDriver->transmit('S',timer);
//
		//serialDriver->transmit(baroSensor->getLattitude(), timer);
		//serialDriver->transmit(baroSensor->getLongitude(), timer);
		//
	//}
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


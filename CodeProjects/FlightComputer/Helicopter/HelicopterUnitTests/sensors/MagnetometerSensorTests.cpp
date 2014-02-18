/*
 * MagnetometerSensorTests.cpp
 *
 * Created: 8/20/2013 8:37:31 PM
 *  Author: HP User
 */ 

#include "MagnetometerSensorTests.h"
#include "MagnetometerDriver.h"
#include "MockMagnetometerDriver.h"
#include "MagnetometerSensor.h"
#include "UnitTestUtils.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;

int readandconvertmagnetometer_test(TestCase *test)
{
	//MagnetometerDriver *driver = new MockMagnetometerDriver(10, 20, 30);
	//driver->initialize();
	//
	//
	//MagnetometerSensor *magSensor = new MagnetometerSensor(driver);
	//
	//
	////////////////////////////////////////////////////////////////////////////
	////test that the default raw sensor values before reading the sensor is 0.
	////////////////////////////////////////////////////////////////////////////
	//AssertTrue2(magSensor->getRawX() == 0, 1);
	//AssertTrue2(magSensor->getRawY() == 0, 2);
	//AssertTrue2(magSensor->getRawZ() == 0, 3);
	//
	////////////////////////////////////////////////////////////////////////////
	////Test that after reading the sensor, the raw sensor values are not 0.
	////////////////////////////////////////////////////////////////////////////
	//AssertTrue2(magSensor->readSensor() == 0, 10);
	//
	//AssertTrue2(magSensor->getRawX() == 10, 4);
	//AssertTrue2(magSensor->getRawY() == 20, 5);
	//AssertTrue2(magSensor->getRawZ() == 30, 6);
	//
	//AssertTrue2(round(magSensor->getFRDX()) == -20, 7);
	//AssertTrue2(round(magSensor->getFRDY()) == -10, 8);
	//AssertTrue2(round(magSensor->getFRDZ()) == -30, 9);
	
	return 0;
};
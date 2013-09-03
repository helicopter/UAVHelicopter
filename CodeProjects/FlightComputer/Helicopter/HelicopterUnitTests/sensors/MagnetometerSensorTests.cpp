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
	MagnetometerDriver *driver = new MockMagnetometerDriver(10, 20, 30);
	driver->initialize();
	
	
	MagnetometerSensor *magSensor = new MagnetometerSensor(driver);
	
	
	//////////////////////////////////////////////////////////////////////////
	//test that the default raw sensor values before reading the sensor is 0.
	//////////////////////////////////////////////////////////////////////////
	AssertTrue(magSensor->getRawX() == 0, 1);
	AssertTrue(magSensor->getRawY() == 0, 2);
	AssertTrue(magSensor->getRawZ() == 0, 3);
	
	//////////////////////////////////////////////////////////////////////////
	//Test that after reading the sensor, the raw sensor values are not 0.
	//////////////////////////////////////////////////////////////////////////
	AssertTrue(magSensor->readSensor() == 0, 10);
	
	AssertTrue(magSensor->getRawX() == 10, 4);
	AssertTrue(magSensor->getRawY() == 20, 5);
	AssertTrue(magSensor->getRawZ() == 30, 6);
	
	AssertTrue(round(magSensor->getFRDX()) == -20, 7);
	AssertTrue(round(magSensor->getFRDY()) == -10, 8);
	AssertTrue(round(magSensor->getFRDZ()) == -30, 9);
	
	return 0;
};
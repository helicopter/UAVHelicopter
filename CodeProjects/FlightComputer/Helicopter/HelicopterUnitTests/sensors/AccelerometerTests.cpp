/*
 * AccelerometerTests.cpp
 *
 * Created: 2/1/2014 12:26:26 PM
 *  Author: HP User
 */ 

#include "UnitTestUtils.h"
#include "AccelerometerTests.h"
#include "SPIDriver.h"

using namespace helicopter::drivers;

int readaccelerometer_test(TestCase *test)
{
	SPIDriver *driver = new SPIDriver();
	/*
	AccelerometerSensor *accelSensor = new AccelerometerSensor(driver);
	
	accelSensor->getXAccel();
	
	AssertTrue(accelSensor->getRawX() == 0);
	AssertTrue(accelSensor->getRawY() == 0);
	AssertTrue(accelSensor->getRawZ() == 0);
	
	accelSensor->readSensor();
	
	int accelX = accelSensor->getRawX();
	int accelY = accelSensor->getRawY();
	int accelZ = accelSensor->getRawZ();
	
	AssertTrue(accelX != 0);
	AssertTrue(accelY != 0);
	AssertTrue(accelZ != 0);
	
	//can I do a matrix rotation?
	
	return  (rawMagX * (cos(theta)*cos(sai))) + (rawMagY * (sin(phi)*sin(theta) * cos(sai) - cos(phi)*sin(sai))) + (rawMagZ * (cos(phi)*sin(theta)*cos(sai)+sin(phi)*sin(sai)));
	
	AssertTrue(accelSensor->getFRDX() != 0);
	AssertTrue(accelSensor->getFRDY() != 0);
	AssertTrue(accelSensor->getFRDZ() != 0);
	*/
	return 0;
}
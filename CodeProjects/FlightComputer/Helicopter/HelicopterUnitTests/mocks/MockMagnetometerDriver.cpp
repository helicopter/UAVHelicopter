/*
 * MockMagnetometerDriver.cpp
 *
 * Created: 8/21/2013 9:15:23 PM
 *  Author: HP User
 */ 

#include "MockMagnetometerDriver.h"

MockMagnetometerDriver::MockMagnetometerDriver(int16_t fakeRawXReading, int16_t fakeRawYReading, int16_t fakeRawZReading)
{
	this->fakeRawXReading = fakeRawXReading;
	this->fakeRawYReading = fakeRawYReading;
	this->fakeRawZReading = fakeRawZReading;
}

int MockMagnetometerDriver::init()
{
	return 0;
}

int MockMagnetometerDriver::readSensor(int16_t *rawMagX, int16_t *rawMagY, int16_t *rawMagZ)
{
	*rawMagX = fakeRawXReading;
	*rawMagY = fakeRawYReading;
	*rawMagZ = fakeRawZReading;
	
	return 0;
}
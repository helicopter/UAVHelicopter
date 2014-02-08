/*
 * AccelerometerTests.cpp
 *
 * Created: 2/1/2014 12:26:26 PM
 *  Author: HP User
 */ 
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UnitTestUtils.h"
#include "ImuSensorTests.h"
#include "SPIDriver.h"
#include "IMUSensor.h"
#include "Timer.h"
#include "SerialDriver.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;

/*
	struct SensorReading
	{
		int accelX;
		int accelY;
		int accelZ;
		int gyroX;
		int gyroY;
		int gyroZ;

		float accelFrdX;
		float accelFrdY;
		float accelFrdZ;
		float gyroFrdX;
		float gyroFrdY;
		float gyroFrdZ;
	};
	*/

void send(SerialDriver *driver, Timer *t, float val)
{	
	driver->transmitByte((byte)(((int)val) >> 8),t);
	driver->transmitByte((byte)val,t);
}

void send(SerialDriver *driver, Timer *t, int val)
{
	driver->transmitByte((byte) (val >> 8),t);
	driver->transmitByte((byte)val,t);	
}

int readimu_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true);
	serialDriver->initialize();
	
	SPIDriver *driver = new SPIDriver();
	driver->init();
	
	IMUSensor *imuSensor = new IMUSensor(driver);
	
	imuSensor->init();
	
	AssertTrue(imuSensor->getRawAccX() == 0);
	AssertTrue(imuSensor->getRawAccY() == 0);
	AssertTrue(imuSensor->getRawAccZ() == 0);
	
	imuSensor->readSensor();
	
	while(true)
	{
		
		_delay_ms(500);
		
		imuSensor->readSensor();
		
		
		int accelX = imuSensor->getRawAccX();
		int accelY = imuSensor->getRawAccY();
		int accelZ = imuSensor->getRawAccZ();
		int gyroX = imuSensor->getRawGyroX();
		int gyroY = imuSensor->getRawGyroY();
		int gyroZ = imuSensor->getRawGyroZ();

		float accelFrdX = imuSensor->getFRDAccX();
		float accelFrdY = imuSensor->getFRDAccY();
		float accelFrdZ = imuSensor->getFRDAccZ();
		float gyroFrdX = imuSensor->getFRDGyroX();
		float gyroFrdY = imuSensor->getFRDGyroY();
		float gyroFrdZ = imuSensor->getFRDGyroZ();
		
		serialDriver->transmitByte('S',timer);		
		send(serialDriver, timer, accelX);
		send(serialDriver, timer, accelY);
		send(serialDriver, timer, accelZ);
		
		
		send(serialDriver, timer, gyroX);
		send(serialDriver, timer, gyroY);
		send(serialDriver, timer, gyroZ);
		send(serialDriver, timer, accelFrdX);
		send(serialDriver, timer, accelFrdY);
		send(serialDriver, timer, accelFrdZ);
		send(serialDriver, timer, gyroFrdX);
		send(serialDriver, timer, gyroFrdY);
		send(serialDriver, timer, gyroFrdZ);
	
	}

	return 0;
}
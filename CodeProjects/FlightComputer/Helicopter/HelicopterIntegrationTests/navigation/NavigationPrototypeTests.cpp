/*
 * NavigationPrototypeTests.cpp
 *
 * Created: 3/18/2014 6:52:51 PM
 *  Author: HP User
 */ 

#include <util/delay.h>

#include "NavigationPrototypeTests.h"
#include "SPIDriver.h"
#include "SerialDriver.h"
#include "UnitTestUtils.h"
#include "Timer.h"
#include "IMUSensor.h"
#include "GPSSensor.h"


using namespace helicopter::drivers;
using namespace helicopter::util;
using namespace helicopter::sensors;


int ahrs_test(TestCase *test)
{
	//Create objects for reading sensors
	SPIDriver *spiDriver = new SPIDriver();
	
	IMUSensor *imuSensor = new IMUSensor(spiDriver);
	imuSensor->init();
	
	//Create AHRS
	//AHRS *ahrs = new AHRS();
	
	
	int counter = 0;
	
	//Supposed to be a 50 hz loop. Not even close due to lag of transmitting data.
	while (true)
	{
		//Read IMU
		imuSensor->readSensor();
	
		//Read GPS at 1 hz.
		if (counter >= 50)
		{
			counter = 0;
		}else
		{
			counter++;
		}
		
		//Update AHRS
		
	
		//Send linear acc, and euler angles to GCS.
		
		_delay_ms(20);	
	}

	
	return 0;
}

int prototypefusion_test(TestCase *test)
{
	//implement ahrs system
	//create tests for that
	//then develop complementary filter
	
	
	//Create and initialize IMU sensor
	//Create and initialize GPS
	
	
	
	//count
	//for every 10 counts, read the gps
	//every time read the imu
	//run the algorithm
	//send the data to the GCS.
	
	
	return 0;
}
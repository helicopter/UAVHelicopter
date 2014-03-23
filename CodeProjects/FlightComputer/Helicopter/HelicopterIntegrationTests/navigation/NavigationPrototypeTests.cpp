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
#include "TWIDriver.h"
#include "MagnetometerSensor.h"
#include "AHRS.h"


using namespace helicopter::drivers;
using namespace helicopter::util;
using namespace helicopter::sensors;
using namespace helicopter::util;
using namespace helicopter::navigation;


//TODO:
//Check the timing of this function, can it keep up with adequate speed?
//(assuming it didn't use serial communication every iteration)
//Perhaps even remove the delay since serial driver will be slow enough.

int ahrs_test(TestCase *test)
{
	//Create objects for reading sensors
	SPIDriver *spiDriver = new SPIDriver();
	spiDriver->init();
	
	TWIDriver *twDriver = new TWIDriver();
	
	SerialDriver *serialDriver = new SerialDriver(115200, SerialDriver::Zero, true, NULL);
	serialDriver->init();
	
	MagnetometerSensor *magnetometerSensor = new MagnetometerSensor(twDriver);
	magnetometerSensor->init();
	
	IMUSensor *imuSensor = new IMUSensor(spiDriver);
	imuSensor->init();
	

	//Create AHRS. 98f is the sampling frequency of the gyroscope
	AHRS *ahrs = new AHRS(1/98.0f);
	
	
	int counter = 0;
	
	//Supposed to be a 50 hz loop. Not even close due to lag of transmitting data.
	while (true)
	{
		//Read IMU
		imuSensor->readSensor();
		magnetometerSensor->readSensor();
		
		/*ahrs->update(.2, .3, .4,
			.5, .6, .7,
			.8, .9, .10);*/
		
		//Update AHRS
		ahrs->update(imuSensor->getFRDAccXMss(), imuSensor->getFRDAccYMss(), imuSensor->getFRDAccZMss(),
					 imuSensor->getFRDGyroXRs(), imuSensor->getFRDGyroYRs(), imuSensor->getFRDGyroZRs(),
					 magnetometerSensor->getFRDX(), magnetometerSensor->getFRDY(), magnetometerSensor->getFRDZ());
		
		if (counter++ > 10)
		{
			//Send linear acc, and euler angles to GCS.
			serialDriver->transmit((byte)'S');
			
			
			/*serialDriver->transmit(ahrs->getYawRads());
			serialDriver->transmit(ahrs->getPitchRads());
			serialDriver->transmit(ahrs->getRollRads());*/
			/*serialDriver->transmit(imuSensor->getFRDGyroZRs());
			serialDriver->transmit(imuSensor->getFRDGyroYRs());
			serialDriver->transmit(imuSensor->getFRDGyroXRs());		
			serialDriver->transmit(ahrs->data1);
			serialDriver->transmit(ahrs->data2);
			serialDriver->transmit(ahrs->data3);*/

			
			/*
			serialDriver->transmit(imuSensor->getFRDAccXMss());
			serialDriver->transmit(imuSensor->getFRDAccYMss());
			serialDriver->transmit(imuSensor->getFRDAccZMss());
			serialDriver->transmit(imuSensor->getFRDGyroXRs());
			serialDriver->transmit(imuSensor->getFRDGyroYRs());
			serialDriver->transmit(imuSensor->getFRDGyroZRs());
			serialDriver->transmit(magnetometerSensor->getFRDX());
			serialDriver->transmit(magnetometerSensor->getFRDY());
			serialDriver->transmit(magnetometerSensor->getFRDZ());
			*/
			
		
			
			serialDriver->transmit(ahrs->getYawRads());
			serialDriver->transmit(ahrs->getPitchRads());
			serialDriver->transmit(ahrs->getRollRads());
			serialDriver->transmit(ahrs->getLinearAccelerationXMss());
			serialDriver->transmit(ahrs->getLinearAccelerationYMss());
			serialDriver->transmit(ahrs->getLinearAccelerationZMss());
			
			
			counter = 0;
		}
		
		
		//_delay_ms(20);	
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
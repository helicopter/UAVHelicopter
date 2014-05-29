/*
 * AccelerometerTests.cpp
 *
 * Created: 2/1/2014 12:26:26 PM
 *  Author: HP User
 */ 
#include <math.h>
#include <util/delay.h>

#include "UnitTestUtils.h"
#include "ImuSensorTests.h"
#include "SPIDriver.h"
#include "IMUSensor.h"
#include "Timer.h"
#include "SerialDriver.h"
#include "TWIDriver.h"
#include "MagnetometerSensor.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;


void send(SerialDriver *driver, float val)
{	
	driver->transmit((byte)(((int)val) >> 8));
	driver->transmit((byte)val);
}

void send(SerialDriver *driver, int val)
{
	driver->transmit((byte) (val >> 8));
	driver->transmit((byte)val);	
}


int readmag_test(TestCase *test)
{
	byte SENSOR_ADDRESS = 0x3C;
	byte WRITE_OPERATION = 0x00;
	byte READ_OPERATION = 0x01;
	
	byte DATA_OUTPUT_X_MSG_REGISTER = 0x03;
	byte CONFIGURATION_REGISTER_A = 0x00;
	byte MODE_REGISTER = 0x02;
	
	byte REGISTER_A_CONFIGURATION_75HZ_8AVG = 0x78;
	byte COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE = 0x00;
	
	int magXVal = 0;
	int magZVal = 0;
	int magYVal = 0;
	
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	serialDriver->init();

	TWIDriver *driver = new TWIDriver();
	
	/**
	* Configure the device
	*/
	driver->start();
	
	//Indicate that the next value is a write operation 
	if (!driver->write(SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		AssertTrue(false);
	}
		
	//Write to the device so that the 'pointer' moves to register A
	if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		AssertTrue(false);
	}
		
	//Write the configuration value to register A
	if (!driver->write(REGISTER_A_CONFIGURATION_75HZ_8AVG, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		AssertTrue(false);
	}	


	driver->stop();


	//set compass mode
	if(!driver->start())
	{
		AssertTrue(false);
	}
		
	//Indicate that the next value is a write operation
	if (!driver->write(SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		AssertTrue(false);
	}		
		
	//Write to the device so that the 'pointer' moves to the mode register
	if (!driver->write(MODE_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		AssertTrue(false);
	}

	//Write the configuration value to the mode register
	if (!driver->write(COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		AssertTrue(false);
	}

	driver->stop();


	
	while (true)
	{
		
		/**
		 * Setup device to start reading mag data
		 */
		if (!driver->start())
		{
			AssertTrue(false);
		}
		
		/**
		 * Send a command to the sensor indicating that the next value
		 * is a write operation. I.e. we will tell the sensor what internal address
		 * to jump to.
		 */
		if (!driver->write(SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		{
			AssertTrue(false);
		}		
		
		/**
		 * Send the address of the magnetic X register to the sensor
		 * to move the internal 'cursor' to that position so the subsequent 
		 * read command will return the value at that address
		 */
		if (!driver->write(DATA_OUTPUT_X_MSG_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		{
			AssertTrue(false);
		}
		

		//(from atmega2560 doc)
		//In order to enter 'master receiver' mode (the mode we enter into in the next command), a start condition must be entered.
		if (!driver->start())
		{
			AssertTrue(false);
		}

		
		/**
		 * Tell the sensor to read data from the address given above, and send it
		 * to us.
		 */
		if (!driver->write(SENSOR_ADDRESS | READ_OPERATION, TWIDriver::MASTERRECEIVER_SLAVE_READ_ACK))
		{
			AssertTrue(false);
		}
		
		byte highByte = 0;
		byte lowByte = 0;
		
		highByte = driver->readByte(true);
		lowByte = driver->readByte(true);
		magXVal = (highByte << 8) | (lowByte);
		
		highByte = driver->readByte(true);
		lowByte = driver->readByte(true);
		magZVal = (highByte << 8) | (lowByte);
		
		highByte = driver->readByte(true);
		
		//We don't send an acknowledgment on the last byte to be read (the magnetic Y values low byte) because of an apparent undocumented requirement by the device
		//that the nack needs to be sent to have the internal pointer go back to the magnetic X variable.
		lowByte = driver->readByte(false);
		magYVal = (highByte << 8) | (lowByte);		
	
		//transmit stop condition
		driver->stop();
	
		serialDriver->transmit('S');
		serialDriver->transmit(magXVal);
		serialDriver->transmit(magZVal);
		serialDriver->transmit(magYVal);

		_delay_ms(700);
	
	}

	return 0;
}




int magdriver_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	serialDriver->init();

	TWIDriver *driver = new TWIDriver();
	
	MagnetometerSensor *magSensor = new MagnetometerSensor(driver);
	
	magSensor->init();
	
/*	AssertTrue(magSensor->getRawX() == 0);
	AssertTrue(magSensor->getRawY() == 0);
	AssertTrue(magSensor->getRawZ() == 0);
	AssertTrue(magSensor->getFRDX() == 0);
	AssertTrue(magSensor->getFRDY() == 0);
	AssertTrue(magSensor->getFRDZ() == 0);
	
	magSensor->readSensor();
	
	AssertTrue(magSensor->getRawX() != 0);
	AssertTrue(magSensor->getRawY() != 0);
	AssertTrue(magSensor->getRawZ() != 0);
	AssertTrue(magSensor->getFRDX() != 0);
	AssertTrue(magSensor->getFRDY() != 0);
	AssertTrue(magSensor->getFRDZ() != 0);
	
	AssertTrue(magSensor->getFRDX() != magSensor->getRawX());*/
	
	while (true)
	{
		_delay_ms(700);
		magSensor->readSensor();
		
		serialDriver->transmit((byte)'S');
		
		serialDriver->transmit((int)magSensor->getRawX());
		serialDriver->transmit((int)magSensor->getRawY());
		serialDriver->transmit((int)magSensor->getRawZ());
		send(serialDriver, magSensor->getFRDX());
		send(serialDriver, magSensor->getFRDY());
		send(serialDriver, magSensor->getFRDZ());
	}
	
	return 0;
}
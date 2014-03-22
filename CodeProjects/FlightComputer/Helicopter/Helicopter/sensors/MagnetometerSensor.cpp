/*
 * MagnetometerSensor.cpp
 *
 * Created: 8/22/2013 12:15:07 AM
 *  Author: HP User
 */ 
#include <math.h>
#include <string.h>

#include "MagnetometerSensor.h"
#include "MatrixUtil.h"

using namespace helicopter::sensors;
using namespace helicopter::util;

MagnetometerSensor::MagnetometerSensor(TWIDriver *driver):
	rawMagX(0),
	rawMagY(0),
	rawMagZ(0),
	frdMagX(0),
	frdMagY(0),
	frdMagZ(0),
	driver(driver)
{
	/**
		* Create a rotation matrix to rotate the magnetometer sensor data by
		* 180 degrees on the x axis, then 0 degrees on the y axis, then 90 degrees 
		* counterclockwise on the Z axis.
		*/
	memset(magLBUToFRDRotationMatrix,0,sizeof(magLBUToFRDRotationMatrix));
					
	MatrixUtil::CreateRotationMatrix(M_PI, 0.0f, -1 * (M_PI/2), magLBUToFRDRotationMatrix);
}

bool MagnetometerSensor::init()
{

	/**
	* Configure the output rate of the sensor, and number of samples that are averaged
	* per measurement output
	*/
	
	//Start a two wire transaction
	if(!driver->start())
	{
		return false;
	}
	
	//Indicate that the next value is a write operation 
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}
		
	//Write to the device so that the 'pointer' moves to register A
	if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}
		
	//Write the configuration value to register A
	if (!driver->write(REGISTER_A_CONFIGURATION_75HZ_8AVG, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}	

	//end the transaction
	driver->stop();


	/**
	* Set the compass mode for continuous measurement
	*/
	//start a transaction
	if(!driver->start())
	{
		return false;
	}
		
	//Indicate that the next value is a write operation
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}		
		
	//Write to the device so that the 'pointer' moves to the mode register
	if (!driver->write(MODE_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//Write the configuration value to the mode register
	if (!driver->write(COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//end the transaction
	driver->stop();
	
	return true;
}

int MagnetometerSensor::readSensor()
{
	/**
	* Setup device to start reading mag data
	*/
	if (!driver->start())
	{
		return -1;
	}
		
	/**
		* Send a command to the sensor indicating that the next value
		* is a write operation. I.e. we will tell the sensor what internal address
		* to jump to.
		*/
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return -1;
	}		
		
	/**
		* Send the address of the magnetic X register to the sensor
		* to move the internal 'cursor' to that position so the subsequent 
		* read command will return the value at that address
		*/
	if (!driver->write(DATA_OUTPUT_X_MSG_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return -1;
	}
		

	//(from atmega2560 doc)
	//In order to enter 'master receiver' mode (the mode we enter into in the next command), a start condition must be entered.
	if (!driver->start())
	{
		return -1;
	}

		
	/**
		* Tell the sensor to read data from the address given above, and send it
		* to us.
		*/
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | READ_OPERATION, TWIDriver::MASTERRECEIVER_SLAVE_READ_ACK))
	{
		return -1;
	}
		
	byte highByte = 0;
	byte lowByte = 0;
		
	highByte = driver->readByte(true);
	lowByte = driver->readByte(true);
	rawMagX = (highByte << 8) | (lowByte);
		
	highByte = driver->readByte(true);
	lowByte = driver->readByte(true);
	rawMagZ = (highByte << 8) | (lowByte);
		
	highByte = driver->readByte(true);
		
	//We don't send an acknowledgment on the last byte to be read (the magnetic Y values low byte) because of an apparent undocumented requirement by the device
	//that the nack needs to be sent to have the internal pointer go back to the magnetic X variable.
	lowByte = driver->readByte(false);
	rawMagY = (highByte << 8) | (lowByte);		
	
	//transmit stop condition
	driver->stop();
	
	//Convert the raw values to FRD values.
	float rotatedValues[3] = {0};
	
	int values[3] = {rawMagX, rawMagY, rawMagZ};
	
	MatrixUtil::RotateMatrix(magLBUToFRDRotationMatrix, values, rotatedValues);
	
	frdMagX = rotatedValues[0];
	frdMagY = rotatedValues[1];
	frdMagZ = rotatedValues[2];	
		
	return 0;
}
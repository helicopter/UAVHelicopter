/*
 * MagnetometerSensor.cpp
 *
 * Created: 8/22/2013 12:15:07 AM
 *  Author: HP User
 */ 

#include "MagnetometerSensor.h"

using namespace helicopter::sensors;

MagnetometerSensor::MagnetometerSensor(MagnetometerDriver *driver):
	rawMagX(0),
	rawMagY(0),
	rawMagZ(0),
	magDriver(driver)
{

}

int MagnetometerSensor::readSensor()
{
	if (magDriver != NULL)
	{
		return magDriver->readSensor(&rawMagX, &rawMagY, &rawMagZ);
	}

	return -1;
}

/**
 * Rotate X axis
 * first around Phi (x axis) then around theta (y axis) then around sai (z axis)
 * so that the x axis now points out of the front of the aircraft.
 * This method uses Euler angle rotations.
 */
double MagnetometerSensor::getFRDX()
{
	//TODO **** note make this happen only once when the data is read.
	return  (rawMagX * (cos(theta)*cos(sai))) + (rawMagY * (sin(phi)*sin(theta) * cos(sai) - cos(phi)*sin(sai))) + (rawMagZ * (cos(phi)*sin(theta)*cos(sai)+sin(phi)*sin(sai)));
}

/**
 * Rotate Y axis
 * first around Phi (x axis) then around theta (y axis) then around sai (z axis)
 * so that the Y axis now points out of the right side of the aircraft
 * This method uses Euler angle rotations.
 */
double MagnetometerSensor::getFRDY()
{
	return (rawMagX * (cos(theta)*sin(sai))) + (rawMagY * (sin(phi)*sin(theta)*sin(sai) + cos(phi)*cos(sai))) + (rawMagZ * (cos(phi)*sin(theta)*sin(sai) - sin(phi)*cos(sai)));
}

/**
 * Rotate Z axis
 * first around Phi (x axis) then around theta (y axis) then around sai (z axis)
 * so that the Z axis now points out of the bottom of the aircraft
 * This method uses Euler angle rotations.
 */
double MagnetometerSensor::getFRDZ()
{
	return (rawMagX * (-1 * sin(theta))) + (rawMagY * (sin(phi)*cos(theta))) + (rawMagZ * (cos(phi)*cos(theta)));
}
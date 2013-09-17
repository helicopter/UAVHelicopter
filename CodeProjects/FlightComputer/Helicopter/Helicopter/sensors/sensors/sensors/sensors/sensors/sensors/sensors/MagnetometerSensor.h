/*
 * MagnetometerSensor.h
 *
 * Created: 8/22/2013 12:02:10 AM
 *  Author: HP User
 */ 


#ifndef MAGNETOMETERSENSOR_H_
#define MAGNETOMETERSENSOR_H_

#include <math.h>

#include "commonheader.h"
#include "MagnetometerDriver.h"

using namespace helicopter::drivers;


namespace helicopter
{
	namespace sensors
	{
		/**
		 * This class represents the logic for reading 
		 * the magnetometer sensors and converting the
		 * sensor values between raw sensor data, and 
		 * usable formats.
		 * 
		 * The supported sensor is the HMC5883L
		 *
		 * The raw orientation of the sensors is body frame:
		 * X pointing out the left side of the CPU
		 * Y points behind the CPU
		 * Z points UP out of the CPU.
		 *
		 * The FRD sensor readings are in body frame Front Right Down
		 * X pointing out the nose (front) of the aircraft
		 * Y is pointing out of the right of the aircraft
		 * Z is pointing down out of the aircraft
		 */
		class MagnetometerSensor
		{
			private:
				//Rotation about y axis
				static const double theta = 0;
				
				//Rotation about Z axis
				static const double sai = (M_PI / 2) * -1;
				
				//Rotation about X axis
				static const double phi = M_PI;
							
				int16_t rawMagX;
				int16_t rawMagY;
				int16_t rawMagZ;
				
				MagnetometerDriver *magDriver;
			
			public:
				MagnetometerSensor(MagnetometerDriver *driver);
				
				/**
				 * This method reads the physical values from the sensor and stores
				 * the values in the rawMag fields. 
				 * Reading the sensor is separate from getting the NED values to separate
				 * the 'reading' tasks, from the 'convertion' tasks.
				 * @return 0 if successful in reading the sensor, -1 otherwise
				 */
				int readSensor();
			
				/**
				 * Retrieves the raw magnetic X sensor reading.
				 */
				int16_t getRawX()
				{
					return rawMagX;
				}
				
				/**
				 * Retrieves the raw magnetic Y sensor reading.
				 */
				int16_t getRawY()
				{
					return rawMagY;
				}
				
				/**
				 * Retrieves the raw magnetic Z sensor reading.
				 */				
				int16_t getRawZ()
				{
					return rawMagZ;
				}
				
				/**
				 * Retrieves the X sensor reading converted so that
				 * it points out the front of the CPU
				 */				
				double getFRDX();
				
				/**
				 * Retrieves the Y sensor reading converted so that
				 * it points out the right of the CPU
				 */	
				double getFRDY();
				
				/**
				 * Retrieves the Z sensor reading converted so that
				 * it points down out of the CPU
				 */					
				double getFRDZ();
				
		};
	}
}



#endif /* MAGNETOMETERSENSOR_H_ */
/*
 * MagnetometerDriver.h
 *
 * Created: 8/21/2013 8:51:47 PM
 *  Author: HP User
 */ 


#ifndef MAGNETOMETERDRIVER_H_
#define MAGNETOMETERDRIVER_H_

#include "CommonHeader.h"

namespace helicopter
{
	namespace drivers
	{

		/** 
		* This class represents the logic necessary to communicate with the
		* magnetometer sensor
		*/
		class MagnetometerDriver
		{
			private:
			
			public:
				
				/** 
				* An initializer to initialize the driver and
				* sensor. This is necessary before communication with
				* the sensor can begin.
				* @return 0 if successful, -1 if there was an error.
				*/
				virtual int initialize();
				
				/**
				 * Reads the raw X, Y, and Z sensor readings from the magnetometer
				 * @param rawMagX the magnetometer's reading in the X direction
				 * @param rawMagY the magnetometer's reading in the y direction
				 * @param rawMagZ the magnetometer's reading in the z direction
				 * @return 0 if successful, -1 otherwise
				 */
				virtual int readSensor(int16_t *rawMagX, int16_t *rawMagY, int16_t *rawMagZ);
		};
	}
}



#endif /* MAGNETOMETERDRIVER_H_ */
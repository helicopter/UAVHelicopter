/*
 * GPSSensor.h
 *
 * Created: 2/24/2014 8:20:34 PM
 *  Author: HP User
 */ 


#ifndef GPSSENSOR_H_
#define GPSSENSOR_H_

#include "SerialDriver.h"

using namespace helicopter::drivers;


namespace helicopter
{
	namespace sensors
	{
		class GPSSensor
		{
			private:
				SerialDriver *serialDriver;
				
				float latitude;
				float longitude;
				
			public:

				GPSSensor(SerialDriver *serialDriver): 
					serialDriver (serialDriver),
					latitude (0),
					longitude (0)
				{
				}
				
				
				/**
				 * Reads the sensor values from the sensor and converts the values into
				 * usable lat, long formats
				 * @return bool True if the read was successful, false otherwise.
				 */
				bool readSensor();
				
				/**
				 * Configures and initializes the GPS
				 */
				void init();
				
				bool isGpsReady();
				
				float getLattitude()
				{
					return latitude;
				}
				
				float getLongitude()
				{
					return longitude;
				}
				
		};
	}
}




#endif /* GPSSENSOR_H_ */
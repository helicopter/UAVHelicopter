/*
 * MagnetometerSensor.h
 *
 * Created: 8/22/2013 12:02:10 AM
 *  Author: HP User
 */ 


#ifndef MAGNETOMETERSENSOR_H_
#define MAGNETOMETERSENSOR_H_

#include <math.h>

#include "CommonHeader.h"
#include "TWIDriver.h"

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
				static const byte WRITE_OPERATION = 0x00;
				static const byte READ_OPERATION = 0x01;
							
				/**
				* The two wire address for the magentometer sensor. This is used to
				* reference the magnetometer on the two wire bus since the bus can be
				* share across multiple sensors.
				*/
				static const byte MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS = 0x3C;
	
				static const byte DATA_OUTPUT_X_MSG_REGISTER = 0x03;
				static const byte CONFIGURATION_REGISTER_A = 0x00;
				static const byte CONFIGURATION_REGISTER_B = 0x01;
				
				static const byte MODE_REGISTER = 0x02;
	
				static const byte REGISTER_A_CONFIGURATION_75HZ_8AVG = 0x78;
				static const byte REGISTER_B_CONFIGURATION_660_GAIN = 0x60;
				static const byte COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE = 0x00;
							
				int rawMagX;
				int rawMagY;
				int rawMagZ;
				
				float frdMagX;
				float frdMagY;
				float frdMagZ;	
				
				/**
				 * The magnetometer is mounted so that the X axis points out the left
				 * side of the device, the Y axis points out the back of the device
				 * and the Z axis points up from the device. This rotation matrix
				 * rotates those values to X-Front, Y-Right, Z-Down
				 */
				float magLBUToFRDRotationMatrix[3][3];			
				
				TWIDriver *driver;
			
			public:
static float _offset[3];
static float debug[3];			
				MagnetometerSensor(TWIDriver *driver);
				
				/**
				 * Configures and initializes the magnetometer sensor.
				 * @return true if initialization was successful
				 * false if an error occurred (an incorrect acknowledgment was received)
				 */
				bool init();
				
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
				float getFRDX()
				{
					return frdMagX;
				}
				
				/**
				 * Retrieves the Y sensor reading converted so that
				 * it points out the right of the CPU
				 */	
				float getFRDY()
				{
					return frdMagY;
				}
				
				/**
				 * Retrieves the Z sensor reading converted so that
				 * it points down out of the CPU
				 */					
				float getFRDZ()
				{
					return frdMagZ;
				}
				
				
				void learn_offsets(void);
		};
	}
}



#endif /* MAGNETOMETERSENSOR_H_ */
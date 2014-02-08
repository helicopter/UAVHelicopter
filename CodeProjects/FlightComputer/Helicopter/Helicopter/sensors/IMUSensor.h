/*
 * AccelerometerSensor.h
 *
 * Created: 2/4/2014 10:57:10 PM
 *  Author: HP User
 */ 


#ifndef IMUSENSOR_H_
#define IMUSENSOR_H_

#include <math.h>
#include <string.h>

#include "SPIDriver.h"
#include "MatrixUtil.h"

using namespace helicopter::drivers;
using namespace helicopter::util;

namespace helicopter
{
	namespace sensors
	{
		/**
		 * This class represents the logic for reading 
		 * the Innertial Measurement Unit (IMU) sensors and converting the
		 * sensor values between raw sensor data, and 
		 * usable formats.
		 * 
		 * The supported sensor is the MPU-6000
		 *
		 * The raw orientation of the accelerometer sensors is body frame:
		 * X pointing out the left side of the ardupilot board
		 * Y points out the front of the ardupilot board
		 * Z points UP out of the ardupilot board
		 *
		 * The FRD sensor readings are in body frame Front Right Down
		 * X pointing out the nose (front) of the aircraft
		 * Y is pointing out of the right of the aircraft
		 * Z is pointing down out of the aircraft
		 */
		class IMUSensor
		{
			private:
			
				static const byte REG_PWR_MGMT_1 = 0x6B;
				static const byte REG_USER_CTRL = 0x6A;
				static const byte REG_SMPLRT_DIV = 0x19;
				static const byte REG_CONFIG = 0x1A;
				static const byte REG_GYRO_CONFIG = 0x1B;
				static const byte REG_ACCEL_CONFIG = 0X1C;
				static const byte REG_ACCEL_XOUT_H = 0x3B;
	
				static const byte BITS_DEVICE_RESET = 0x80;
				static const byte BITS_CLKSEL_GYROZ = 0x03;
				static const byte BITS_I2C_IF_DIS = 0x10;
				static const byte BITS_SMPLRT_DIV = 0x00;
				static const byte BITS_DLPF_CFG = 0x02; //digital low pass filter speed 94 Hz for accel, 98Hz for gyro
				static const byte BITS_FS_SEL = 0x18; //+-2000 */s. Full scale range of the gyro scope. This is the max setting. LSB Sensitivity = 16.4 LSB/*/s (page 32)
				static const byte BITS_AFS_SEL = 0x00; //+- 2g scale range of the accelerometer. This is the minimum setting. LSB  sensitivity = 16384 LSB/g (page 30)
	
				static const byte readCommand = 0x80;
				
				float accelRFUToFRDRotationMatrix[3][3];
				
	
				SPIDriver *spiDriver; 
				
				/**
				 * The raw sensor readings from the sensor
				 */
				int rawAccX;
				int rawAccY;
				int rawAccZ;
				
				int rawGyroX;
				int rawGyroY;
				int rawGyroZ;
				
				/**
				 * The raw sensor readings converted to Front-Right-Down format
				 * where X goes out the front, Y axis goes out the right, and positive Z
				 * values point down out the bottom of the ardupilot device.
				 */
				float frdAccX;
				float frdAccY;
				float frdAccZ;
				
				float frdGyroX;
				float frdGyroY;
				float frdGyroZ;
				
				
			public:
				/**
				 * Constructs the accel sensor.
				 * @param driver The SPI driver used to communicate with the
				 * MPU6000's accelerometer sensor via the SPI protocol.
				 */
				IMUSensor(SPIDriver *spiDriver): 
					spiDriver (spiDriver),
					rawAccX(0),
					rawAccY(0),
					rawAccZ(0),
					rawGyroX(0),
					rawGyroY(0),
					rawGyroZ(0),
					frdAccX(0),
					frdAccY(0),
					frdAccZ(0)
				{
					/**
					 * Create a rotation matrix to rotate the accelerometer sensor data by
					 * 180 degrees on the x axis, then 0 degrees on the y axis, then 90 degrees 
					 * clockwise on the Z axis.
					 */
					memset(accelRFUToFRDRotationMatrix,0,sizeof(float)*9);
					
					MatrixUtil::CreateRotationMatrix(M_PI, 0.0f, (M_PI/2), accelRFUToFRDRotationMatrix);
				}
				
				/**
				 * Reads the sensor values from the sensor and converts the values into FRD format.
				 * (Basically populates the rawXYZ and FRDXYZ fields with data from the sensor)
				 */
				void readSensor();
				
				/**
				 * Initializes the sensor with the desired sensor sensitivity settings, etc. 
				 */
				void init();
				
				int getRawAccX() { return rawAccX;}
				int getRawAccY() { return rawAccY;}
				int getRawAccZ() { return rawAccZ;}
					
				int getRawGyroX() { return rawGyroX;}
				int getRawGyroY() { return rawGyroY;}
				int getRawGyroZ() { return rawGyroZ;}					
				
				float getFRDAccX() { return frdAccX;}
				float getFRDAccY() { return frdAccY;}
				float getFRDAccZ() { return frdAccZ;}
					
				float getFRDGyroX() { return frdGyroX;}
				float getFRDGyroY() { return frdGyroY;}
				float getFRDGyroZ() { return frdGyroZ;}			
		};
	}
}





#endif /* IMUSENSOR_H_ */
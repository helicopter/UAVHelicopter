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
		 *
		 * Information on the MPU-6000 can be found in the following document:
		 * RM-MPU-6000A.pdf
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
				
				//+-2000 */s. Full scale range of the gyro scope. This is the max setting. LSB Sensitivity = 16.4 LSB/*/s (page 32). 
				//Note: if this is changed, you have to change the RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER which converts raw data to rads per sec.
				static const byte BITS_FS_SEL = 0x18; 
				
				//+- 2g scale range of the accelerometer. This is the minimum setting. LSB  sensitivity = 16384 LSB/g (page 30)
				//Note: if this is changed, you have to change the RAW_ACC_TO_RADS_PER_SECOND_SECOND_CONVERTER which converts raw data to
				//acceleration
				static const byte BITS_AFS_SEL = 0x00; 
	
				static const byte readCommand = 0x80;
				
				/**
				 * Per page 32 in the RM-MPU-6000A document, since FS_SEL is 
				 * set to +-2000 degrees per sec, there are 16.4 LSB's per degree per second
				 * therefore, to get radians per sec, you have to take the sensor reading,
				 * / by 16.4, which gives you degrees per sec, then convert that to rads.
				 * 16.4 comes from document, 180 / pi is used to convert degrees to rads.
				 */
				static const float RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER;

				/**
				 * Per page 30 in the RM-MPU-6000A document, since AFS_SEL is 
				 * set to +-2g, there are 16384 LSB's per g
				 * This value converts from the raw data to acceleration in meters per second per second.
				 */				
				static const float RAW_ACC_TO_RADS_PER_SECOND_SECOND_CONVERTER;
				
				float imuRFUToFRDRotationMatrix[3][3];
				
	
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
				float frdAccXMss;
				float frdAccYMss;
				float frdAccZMss;
				
				float frdGyroXRs;
				float frdGyroYRs;
				float frdGyroZRs;
				
				bool hasBeenRead;
				bool missedRead;
				
				//Offsets for raw gyro readings. [0] = x, [1] = y, [2] = z
				float gyroOffsets[3];
				
				
				
			public:
				/**
				 * Constructs the Inertial Measurement Unit sensor.
				 * @param driver The SPI driver used to communicate with the
				 * MPU6000's sensor via the SPI protocol.
				 */
				IMUSensor(SPIDriver *spiDriver): 
					spiDriver (spiDriver),
					rawAccX(0),
					rawAccY(0),
					rawAccZ(0),
					rawGyroX(0),
					rawGyroY(0),
					rawGyroZ(0),
					frdAccXMss(0),
					frdAccYMss(0),
					frdAccZMss(0),
					frdGyroXRs(0),
					frdGyroYRs(0),
					frdGyroZRs(0),
					hasBeenRead(true),
					missedRead(false)
				{
					/**
					 * Create a rotation matrix to rotate the accelerometer sensor data by
					 * 180 degrees on the x axis, then 0 degrees on the y axis, then 90 degrees 
					 * clockwise on the Z axis.
					 */
					memset(imuRFUToFRDRotationMatrix,0,sizeof(imuRFUToFRDRotationMatrix));
					
					MatrixUtil::CreateRotationMatrix(M_PI, 0.0f, (M_PI/2), imuRFUToFRDRotationMatrix);
					
					gyroOffsets[0] = 0;
					gyroOffsets[1] = 0;
					gyroOffsets[2] = 0;
				}
				
				bool missedReading()
				{
					//when sensor receives new sensor reading, set hasBeenRead to false.
					//if hasBeenRead is still false when another sensor reading happens, then set
					//missedReading to true.
					bool val = missedRead;
					missedRead = false;
					hasBeenRead = true;
					return val;
					
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
				
				/**
				 * Returns the acceleration in meters per second per second in the X direction in Front (X) Right (Y) Down (Z) coordinate system
				 */
				float getFRDAccXMss() { return frdAccXMss;}
					
				/**
				 * Returns the acceleration in meters per second per second in the Y direction in Front (X) Right (Y) Down (Z) coordinate system
				 */					
				float getFRDAccYMss() { return frdAccYMss;}
					
				/**
				 * Returns the acceleration in meters per second per second in the Z direction in Front (X) Right (Y) Down (Z) coordinate system
				 */					
				float getFRDAccZMss() { return frdAccZMss;}
					
				/**
				 * Returns the angular velocity in rads per second about the X axis in Front (X) Right (Y) Down (Z) coordinate system
				 */					
				float getFRDGyroXRs() { return frdGyroXRs;}
					
				/**
				 * Returns the angular velocity in rads per second about the Y axis in Front (X) Right (Y) Down (Z) coordinate system
				 */							
				float getFRDGyroYRs() { return frdGyroYRs;}
					
				/**
				 * Returns the angular velocity in rads per second about the Z axis in Front (X) Right (Y) Down (Z) coordinate system
				 */							
				float getFRDGyroZRs() { return frdGyroZRs;}	
		};
	}
}





#endif /* IMUSENSOR_H_ */
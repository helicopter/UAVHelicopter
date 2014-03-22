/*
 * AccelerometerSensor.cpp
 *
 * Created: 2/4/2014 10:57:20 PM
 *  Author: HP User
 */ 
#include <util/delay.h>

#include "IMUSensor.h"
#include "CommonHeader.h"

using namespace helicopter::sensors;

const float IMUSensor::RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER = 16.4 * (180 / M_PI);
//const float IMUSensor::RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER = 131 * (180 / M_PI);
//const float IMUSensor::RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER = (180 / (16.4 * M_PI));
const float IMUSensor::RAW_ACC_TO_RADS_PER_SECOND_SECOND_CONVERTER = GRAVITY_MSS / 16384;



void IMUSensor::init()
{
	
	//Reset the sensor in case of a soft reset the sensor
	//might still have had power and thus not reset.
	//End the transaction. If I don't close and restart a new
	//transaction, accelerometer data doesn't seem to be able to be read.
	spiDriver->transactionWrite(REG_PWR_MGMT_1,BITS_DEVICE_RESET);

	//wait for device reset
	_delay_ms(100);

	//Select Gyro Z as the reference clock. Selecting a gyro clock
	//is recommended in pag 41 of RM-MPU-6000A.pdf for improved stability.
	spiDriver->transactionWrite(REG_PWR_MGMT_1,BITS_CLKSEL_GYROZ);
	
	//Wait for the system to process the clock change. This
	//takes a long time and reading accelerometer data
	//returns 0 unless this is done.
	_delay_ms(100);

	//Enable SPI Interface / disable i2c.
	spiDriver->transactionWrite(REG_USER_CTRL,BITS_I2C_IF_DIS);
	
	//Set the sampling rate to 1kHz.
	spiDriver->transactionWrite(REG_SMPLRT_DIV,BITS_SMPLRT_DIV);
	
	
	//Configures the Digital Low Pass Filter. This is how frequently the sensor readings are updated
	//for accelerometers and gyroscopes.
	spiDriver->transactionWrite(REG_CONFIG,BITS_DLPF_CFG);
	
	//Configures the sensitivity / scale range of the gyroscope
	spiDriver->transactionWrite(REG_GYRO_CONFIG,BITS_FS_SEL);
	
	//configure the sensitivity / scale range of the accelerometer
	spiDriver->transactionWrite(REG_ACCEL_CONFIG,BITS_AFS_SEL);
}

void IMUSensor::readSensor()
{
	//indicate that this device is about to begin communicating with the sensor.
	spiDriver->beginTransaction();

	/**
	 * Move the pointer on the address for the x accelerometer value
	 * on the inertial measurement unit. When reads are performed
	 * it will start by reading the register provided by the address
	 * and then move the pointer one register position down in
	 * it's 'table'. See page 7 of RM-MPU-6000A.pdf
	 * so subsequent reads will read the next position that the register
	 * is now pointing to.
	 * The address is 'or'ed by the 'readcommand' to indicate to the
	 * sensor that the subsequent commands should read data
	 * from the sensor.
	 */	
	spiDriver->write(REG_ACCEL_XOUT_H | readCommand);

	rawAccX = spiDriver->readInt();
	rawAccY = spiDriver->readInt();
	rawAccZ = spiDriver->readInt();
	
	//read the temp data, and ignore it since it isn't used.
	spiDriver->readInt();
	
	rawGyroX = spiDriver->readInt();
	rawGyroY = spiDriver->readInt();
	rawGyroZ = spiDriver->readInt();
	
	/**
	 * The master (this CPU) then pulls the slave select line low indicating
	 * that it is done communicating.
	 */
	spiDriver->endTransaction();
	
	//Convert the raw values to FRD values.
	float rotatedValues[3] = {0};
	float rotatedValues2[3] = {0};
		
	int values[3] = {rawAccX, rawAccY, rawAccZ};
	int values2[3] = {rawGyroX, rawGyroY, rawGyroZ};
		
	MatrixUtil::RotateMatrix(imuRFUToFRDRotationMatrix, values, rotatedValues);
	MatrixUtil::RotateMatrix(imuRFUToFRDRotationMatrix, values2, rotatedValues2);
	
	frdAccXMss = rotatedValues[0] * RAW_ACC_TO_RADS_PER_SECOND_SECOND_CONVERTER;
	frdAccYMss = rotatedValues[1] * RAW_ACC_TO_RADS_PER_SECOND_SECOND_CONVERTER;
	frdAccZMss = rotatedValues[2] * RAW_ACC_TO_RADS_PER_SECOND_SECOND_CONVERTER;
	
	frdGyroXRs = rotatedValues2[0] / RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER;
	frdGyroYRs = rotatedValues2[1] / RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER;
	frdGyroZRs = rotatedValues2[2] / RAW_GYRO_TO_RADS_PER_SECOND_CONVERTER;
}

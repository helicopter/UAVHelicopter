/*
 * SPIDriverTests.cpp
 *
 * Created: 1/27/2014 9:44:04 PM
 *  Author: HP User
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "UnitTestUtils.h"
#include "SPIDriverTests.h"
#include "SPIDriver.h"
#include "CommonHeader.h"

using namespace helicopter::drivers;
using namespace helicoptertestscommon::util::testframework;




int spi_messaging_test(TestCase *test)
{
	
	byte REG_PWR_MGMT_1 =  0x6B;
	byte REG_USER_CTRL =  0x6A;
	byte REG_SMPLRT_DIV = 0x19;
	byte REG_CONFIG = 0x1A;
	byte REG_GYRO_CONFIG = 0x1B;
	byte REG_ACCEL_CONFIG = 0X1C;
	byte REG_ACCEL_XOUT_H = 0x3B;
	
	byte BITS_DEVICE_RESET = 0x80;
	byte BITS_CLKSEL_GYROZ = 0x03;
	byte BITS_I2C_IF_DIS = 0x10;
	byte BITS_SMPLRT_DIV = 0x00;
	byte BITS_DLPF_CFG = 0x02; //digital low pass filter speed 94 Hz for accel, 98Hz for gyro
	byte BITS_FS_SEL = 0x18; //+-2000 */s. Full scale range of the gyro scope. This is the max setting. LSB Sensitivity = 16.4 LSB/*/s (page 32)
	byte BITS_AFS_SEL = 0x00; //+- 2g scale range of the accelerometer. This is the minimum setting. LSB  sensitivity = 16384 LSB/g (page 30)
	
	byte readCommand = 0x80;
	
	SPIDriver *spiDriver = new SPIDriver();
	
	spiDriver->init();
	

	
	//Code based off of rdupilot code.

	//Reset the sensor in case of a soft reset the sensor
	//might still have had power and thus not reset.
	spiDriver->beginTransaction();
	spiDriver->write(REG_PWR_MGMT_1,BITS_DEVICE_RESET);

	//End the transaction. If I don't close and restart a new
	//transaction, accelerometer data doesn't seem to be able to be read.
	spiDriver->endTransaction();
	
	//wait for device reset
	_delay_ms(100);

	spiDriver->beginTransaction();

	//Select Gyro Z as the reference clock. Selecting a gyro clock
	//is recommended in pag 41 of RM-MPU-6000A.pdf for improved stability.
	spiDriver->write(REG_PWR_MGMT_1,BITS_CLKSEL_GYROZ);
	
	//Wait for the system to process the clock change. This
	//takes a long time and reading accelerometer data
	//returns 0 unless this is done.
	_delay_ms(100);

	//Enable SPI Interface / disable i2c.
	spiDriver->write(REG_USER_CTRL,BITS_I2C_IF_DIS);
	
	//Set the sampling rate to 1kHz.
	spiDriver->write(REG_SMPLRT_DIV,BITS_SMPLRT_DIV);
			
	//Configures the Digital Low Pass Filter. This is how frequently the sensor readings are updated
	//for accelerometers and gyroscopes.
	spiDriver->write(REG_CONFIG,BITS_DLPF_CFG);	
	
	//Configures the sensitivity / scale range of the gyroscope
	spiDriver->write(REG_GYRO_CONFIG,BITS_FS_SEL);	
			
	//configure the sensitivity / scale range of the accelerometer
	spiDriver->write(REG_ACCEL_CONFIG,BITS_AFS_SEL);	
			


	
	//According to page 30 of RM-MPU-6000A.pdf, the external registers for accelerometer
	//data is updated when the serial interface is idle. So we release the line
	//and start a new transaction
	spiDriver->endTransaction();
	_delay_ms(1);
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

	int accelX = spiDriver->readInt16();
	int accelY = spiDriver->readInt16();
	int accelZ = spiDriver->readInt16();
	
	/**
	 * The master (this CPU) then pulls the slave select line low indicating
	 * that it is done communicating.
	 */
	spiDriver->endTransaction();

	AssertTrue(accelX != 0);		
	AssertTrue(accelY != 0);
	AssertTrue(accelZ != 0);
	AssertTrue(accelX != accelY && accelY != accelZ);

	return 0;
};

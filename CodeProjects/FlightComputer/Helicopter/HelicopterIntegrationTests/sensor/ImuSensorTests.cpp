/*
 * AccelerometerTests.cpp
 *
 * Created: 2/1/2014 12:26:26 PM
 *  Author: HP User
 */ 
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include "UnitTestUtils.h"
#include "ImuSensorTests.h"
#include "SPIDriver.h"
#include "IMUSensor.h"
#include "Timer.h"
#include "SerialDriver.h"
#include "TWIDriver.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;

/*
	struct SensorReading
	{
		int accelX;
		int accelY;
		int accelZ;
		int gyroX;
		int gyroY;
		int gyroZ;

		float accelFrdX;
		float accelFrdY;
		float accelFrdZ;
		float gyroFrdX;
		float gyroFrdY;
		float gyroFrdZ;
	};
	*/

void send(SerialDriver *driver, Timer *t, float val)
{	
	driver->transmitByte((byte)(((int)val) >> 8),t);
	driver->transmitByte((byte)val,t);
}

void send(SerialDriver *driver, Timer *t, int val)
{
	driver->transmitByte((byte) (val >> 8),t);
	driver->transmitByte((byte)val,t);	
}






















int START = 0x08;
int REPEATSTART = 0x10;
int MT_SLA_ACK = 0x18;
int MT_DATA_ACK = 0x28;

float x_scale = 1,y_scale = 1,z_scale = 1,x_max,y_max,z_max;

//Sends data on the two wire (i2c) interface.
bool twSendData(uint8_t data, int ack)
{
	
	//	sendBytes("sending data\r\n");
	TWDR = data; //load address to send.
	TWCR = (1<<TWINT) | (1<<TWEN); // start transmision of address
	
	while (!(TWCR & (1<<TWINT))) //wait for address to be written.
	;
	
	if (ack == 0) return true;
	
	
	if ((TWSR & 0xF8) != ack)
	{
		//		sendBytes("No ack\r\n");
		return false;
	}
	
	return true;
}



void setConfigRegA(uint8_t regAvalue)
{
	

	//Send a start condition.
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	//wait for twint flag set. indicates that start was sent.
	while (!(TWCR & (1<<TWINT)))
	;
	
	//Verify that start was successfuly sent.
	if ((TWSR & 0xF8) != START)
	{
	}else
	{
		if(twSendData(0x3C, MT_SLA_ACK))
		{
			//register A
			if(twSendData(0x00, MT_DATA_ACK))
			{
				//	if(twSendData(0x78, MT_DATA_ACK)) //75 samples a sec, with 8 samples averaged per output.
				//if(twSendData(0x70, MT_DATA_ACK)) //75 samples a sec, with 8 samples averaged per output. <--comments wrong
				if(twSendData(regAvalue, MT_DATA_ACK)) //75 samples a sec, with 8 samples averaged per output.
				{
					
				}
				
				
			}
			
			
		}
		
		//then register, then value.
		
	}
	
	//transmit stop condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}



int readimu_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true);
	serialDriver->initialize();
	
	SPIDriver *driver = new SPIDriver();
	driver->init();
	
	IMUSensor *imuSensor = new IMUSensor(driver);
	
	imuSensor->init();
	
	AssertTrue(imuSensor->getRawAccX() == 0);
	AssertTrue(imuSensor->getRawAccY() == 0);
	AssertTrue(imuSensor->getRawAccZ() == 0);
	
	imuSensor->readSensor();
	
	while(true)
	{
		
		_delay_ms(500);
		
		imuSensor->readSensor();
		
		
		int accelX = imuSensor->getRawAccX();
		int accelY = imuSensor->getRawAccY();
		int accelZ = imuSensor->getRawAccZ();
		int gyroX = imuSensor->getRawGyroX();
		int gyroY = imuSensor->getRawGyroY();
		int gyroZ = imuSensor->getRawGyroZ();

		float accelFrdX = imuSensor->getFRDAccX();
		float accelFrdY = imuSensor->getFRDAccY();
		float accelFrdZ = imuSensor->getFRDAccZ();
		float gyroFrdX = imuSensor->getFRDGyroX();
		float gyroFrdY = imuSensor->getFRDGyroY();
		float gyroFrdZ = imuSensor->getFRDGyroZ();
		
		serialDriver->transmitByte('S',timer);		
		send(serialDriver, timer, accelX);
		send(serialDriver, timer, accelY);
		send(serialDriver, timer, accelZ);
		
		
		send(serialDriver, timer, gyroX);
		send(serialDriver, timer, gyroY);
		send(serialDriver, timer, gyroZ);
		send(serialDriver, timer, accelFrdX);
		send(serialDriver, timer, accelFrdY);
		send(serialDriver, timer, accelFrdZ);
		send(serialDriver, timer, gyroFrdX);
		send(serialDriver, timer, gyroFrdY);
		send(serialDriver, timer, gyroFrdZ);
	
	}

	return 0;
}



void setCompassMode(uint8_t compassMode)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	//wait for twint flag set. indicates that start was sent.
	while (!(TWCR & (1<<TWINT)))
	;

	//Verify that start was successfully sent.
	if ((TWSR & 0xF8) != START)
	{
	}else
	{
		if(twSendData(0x3C, MT_SLA_ACK))
		{
			if(twSendData(0x02, MT_DATA_ACK))
			{
				if(twSendData(compassMode, MT_DATA_ACK))
				{
					
				}
				
				
			}
			
			
		}
		
		//then register, then value.
		
	}

	//transmit stop condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}


int16_t magXVal = 0;
int16_t magZVal = 0;
int16_t magYVal = 0;

int readmag_test(TestCase *test)
{
	byte SENSOR_ADDRESS = 0x3C;
	byte WRITE_OPERATION = 0x00;
	byte READ_OPERATION = 0x01;
	
	byte DATA_OUTPUT_X_MSG_REGISTER = 0x03;
	byte CONFIGURATION_REGISTER_A = 0x00;
	byte MODE_REGISTER = 0x02;
	
	byte REGISTER_A_CONFIGURATION_75HZ_8AVG = 0x78;
	byte COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE = 0x00;
	
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true);
	serialDriver->initialize();

	TWIDriver *driver = new TWIDriver();
	
	//remove driver->init.

		/**
		* Configure the device
		*/
		driver->start();
	
		//Indicate that the next value is a write operation 
		if (!driver->write(SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		{
			AssertTrue(false);
		}
		
		//Write to the device so that the 'pointer' moves to register A
		if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		{
			AssertTrue(false);
		}
		
		//Write the configuration value to register A
		if (!driver->write(REGISTER_A_CONFIGURATION_75HZ_8AVG, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		{
			AssertTrue(false);
		}	


		driver->stop();


		//set compass mode
		if(!driver->start())
		{
			AssertTrue(false);
		}
		
		//Indicate that the next value is a write operation
		if (!driver->write(SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		{
			AssertTrue(false);
		}		
		
		//Write to the device so that the 'pointer' moves to the mode register
		if (!driver->write(MODE_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		{
			AssertTrue(false);
		}

		//Write the configuration value to the mode register
		if (!driver->write(COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		{
			AssertTrue(false);
		}

		driver->stop();


	
	while (true)
	{
		
		/**
		 * Setup device to start reading mag data
		 */
		if (!driver->start())
		{
			AssertTrue(false);
		}
		
		/**
		 * Send a command to the sensor indicating that the next value
		 * is a write operation. I.e. we will tell the sensor what internal address
		 * to jump to.
		 */
		if (!driver->write(SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		{
			AssertTrue(false);
		}		
		
		/**
		 * Send the address of the magnetic X register to the sensor
		 * to move the internal 'cursor' to that position so the subsequent 
		 * read command will return the value at that address
		 */
		if (!driver->write(DATA_OUTPUT_X_MSG_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		{
			AssertTrue(false);
		}
		

		//(from atmega2560 doc)
		//In order to enter 'master receiver' mode (the mode we enter into in the next command), a start condition must be entered.
		if (!driver->start())
		{
		AssertTrue(false);
		}

		
		/**
		 * Tell the sensor to read data from the address given above, and send it
		 * to us.
		 */
		if (!driver->write(SENSOR_ADDRESS | READ_OPERATION, TWIDriver::MASTERRECEIVER_SLAVE_READ_ACK))
		{
			AssertTrue(false);
		}
		
		byte highByte = 0;
		byte lowByte = 0;
		
		highByte = driver->readByte(true);
		lowByte = driver->readByte(true);
		magXVal = (highByte << 8) | (lowByte);
		
		highByte = driver->readByte(true);
		lowByte = driver->readByte(true);
		magZVal = (highByte << 8) | (lowByte);
		
		highByte = driver->readByte(true);
		
		//We don't send an acknowledgment on the last byte to be read (the magnetic Y values low byte) because of an apparent undocumented requirement by the device
		//that the nack needs to be sent to have the internal pointer go back to the magnetic X variable.
		lowByte = driver->readByte(false);
		magYVal = (highByte << 8) | (lowByte);		
		
		//
		//for (int i = 0; i < 3; i++)
		//{
			//byte highByte = driver->readByte(true);
			//
			//byte lowByte = 0;
			//
			//if (i == 2)
			//{
				////TWCR = (1<<TWINT) | (1<<TWEN); // start transmision of address (flvs now - re-enables tw (twen), and clears int flag(twint) twint is reset after each databyte is received)
				////We don't send an acknowledgment on the last byte to be read (the magnetic Y values low byte) because of an apparent undocumented requirement by the device
				////that the nack needs to be sent to have the internal pointer go back to the magnetic X variable.
				//lowByte = driver->readByte(false);
			//}else
			//{
				////TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); // start transmision of address (flvs now send acknolwedgement?)
				//lowByte = driver->readByte(true);
			//}
			//
			//if (i == 0)
			//{
				//magXVal = (highByte << 8) | (lowByte);
				//magXVal = ((float) magXVal) / x_scale;
			//}
			//if (i == 1)
			//{
				//magZVal = (highByte << 8) | lowByte;
				//magZVal = ((float) magZVal) / z_scale;
			//}
			//
			//if (i == 2)
			//{
				//magYVal = (highByte << 8) | lowByte;
				//magYVal = ((float) magYVal) / y_scale;
				//
			//}
			//
		//}
	
	
	
	//transmit stop condition
	driver->stop();
	
	
	
	
	serialDriver->transmitByte('S',timer);
	send(serialDriver, timer, magXVal);
	send(serialDriver, timer, magZVal);
	send(serialDriver, timer, magYVal);
	
	_delay_ms(700);
	
	}
	
	
	
	
//
	//TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
	//
	////**** I THINK THE INIT METHOD OF THE DRIVER CAN BE DELETED SINCE IT'S JUST ENABLES THE LINE.
	//
		///**
		//* Configure the device
		//*/
		//driver->start();
	//
		////Indicate that the next value is a write operation 
		//if (!driver->write(WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		//{
			//AssertTrue(false);
		//}
		//
		////Write to the device so that the 'pointer' moves to register A
		//if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		//{
			//AssertTrue(false);
		//}
		//
		////Write the configuration value to register A
		//if (!driver->write(REGISTER_A_CONFIGURATION_75HZ_8AVG, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		//{
			//AssertTrue(false);
		//}	
//
//
		//driver->stop();
//
//
		///**
		//* Set compass mode.
		//*/
		//if(!driver->start())
		//{
			//AssertTrue(false);
		//}
		//
		////Indicate that the next value is a write operation
		//if (!driver->write(WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		//{
			//AssertTrue(false);
		//}		
		//
		////Write to the device so that the 'pointer' moves to the mode register
		//if (!driver->write(MODE_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		//{
			//AssertTrue(false);
		//}
//
		////Write the configuration value to the mode register
		//if (!driver->write(COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		//{
			//AssertTrue(false);
		//}
//
		//driver->stop();
//
//
//
//
		//while(true)
		//{
		///**
		 //* Setup device to start reading mag data
		 //*/
		//if (!driver->start())
		//{
			//AssertTrue(false);
		//}
		//
		///**
		 //* Send a command to the sensor indicating that the next value
		 //* is a write operation. I.e. we will tell the sensor what internal address
		 //* to jump to.
		 //*/
		//if (!driver->write(WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
		//{
			//AssertTrue(false);
		//}
		//
		///**
		 //* Send the address of the magnetic X register to the sensor
		 //* to move the internal 'cursor' to that position so the subsequent 
		 //* read command will return the value at that address
		 //*/
		//if (!driver->write(DATA_OUTPUT_X_MSG_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
		//{
			//AssertTrue(false);
		//}
	//
		//
		//
		////Send a repeated start command (indicates that we wish to initiate a new transfer without giving up the bus)
		//if (!driver->start())
		//{
			//AssertTrue(false);
		//}	
					//
		///**
		 //* Tell the sensor to read data from the address given above, and send it
		 //* to us.
		 //*/
		//if (!driver->write(READ_OPERATION, TWIDriver::MASTERRECEIVER_SLAVE_READ_ACK))
		//{
			//AssertTrue(false);
		//}
		//
//
		//
		////twint + twen to start transmission. 
		//
		////here, i need to send acknowledge.
		//
		//
		//
		//
		///**
		 //* Start reading mag data.
		 //*/
//
		//
			//
			//
//
			//byte highByte = 0;
			//byte lowByte = 0;
//
			////x,z,y, left(x), back(y) up(z)
			//int magXLBU = 0;
			//int magYLBU = 0;
			//int magZLBU = 0;
//
			//highByte = driver->readByte();
			//
			//
			//lowByte = driver->readByte();
//
			//magXLBU = (highByte << 8) | lowByte;
			//
			////AssertTrue(magXLBU != 0);
//
			//highByte = driver->readByte();
			//lowByte = driver->readByte();
//
			//magZLBU = (highByte << 8) | lowByte;
//
			//highByte = driver->readByte();
			//lowByte = driver->readByte();
	//TWCR = (1<<TWINT) | (1<<TWEN) ; //Send nack
	//
	////Wait for the interrupt to be reset, indicating that we received the data
	//while (!(TWCR & (1<<TWINT))) ;
	//
	////Read the data.
	//lowByte = TWDR;
//
			//magYLBU = (highByte << 8) | lowByte;		
		//
		//
			//serialDriver->transmitByte('S',timer);
			//send(serialDriver, timer, magXLBU);
			//send(serialDriver, timer, magZLBU);
			//send(serialDriver, timer, magYLBU);
			//
////			TWCR = (1<<TWINT) | (1<<TWEN);
			//
			//_delay_ms(500);
			//
			//
			////send stop
			//driver->stop();
			//
		//}		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		//send sto[?	
		//i think we have to wait for interrupt here. (or 6 ms)
			
			
		//Wait 6 ms or monitor status register or DRDY hardware interrupt pin
			
			
			//send(serialDriver, timer, gyroFrdZ);
		
		//what happens now?
		//reset interrupt flag (TWINT), twi enable acknowledge bit(twea)



	return 0;
}
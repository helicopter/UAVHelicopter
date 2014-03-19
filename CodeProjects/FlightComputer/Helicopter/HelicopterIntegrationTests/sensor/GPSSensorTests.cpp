/*
 * GPSSensorTests.cpp
 *
 * Created: 2/23/2014 9:48:36 PM
 *  Author: HP User
 */ 
#include <math.h>
#include <util/delay.h>

#include "GPSSensorTests.h"
#include "UnitTestUtils.h"
#include "ImuSensorTests.h"
#include "SPIDriver.h"
#include "IMUSensor.h"
#include "Timer.h"
#include "SerialDriver.h"
#include "GPSSensor.h"
#include "MockSerialDriver.h"
#include "CoordinateUtil.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;

int baudchange_test(TestCase *test)
{
	Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 500);
	
	
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	serialDriver->initialize();
	
	serialDriver->transmit('T');
	
	//read 100 bytes to clear the buffer
	for (int i = 0; i < 100; i++)
	{
			byte b = 0;
			gpsSerialDriver->receive(b);
			serialDriver->transmit(b);
	}
	
	byte posllh[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};

	//byte status[] = {0xB5, 0x62, 0x01, 0x03, 0x00, 0x00, 0x04, 0x0D};
	
	byte cfg2[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x54};
	byte cfg[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};
	
	gpsSerialDriver->clearBuffer();
	gpsSerialDriver->transmit((const char*)cfg, sizeof(cfg));

//	UCSR1B &= ~((1<<RXEN1) | (1<<TXEN1));
//	UBRR1 = 200;
//	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
//	UCSR1A |= (1<<U2X1);
//	UCSR1B |= (1<<RXEN1) | (1<<TXEN1);	
	
	//read 100 bytes to clear the buffer and ack value.
	
	byte b = 0;
	gpsSerialDriver->receive(b);
	serialDriver->transmit(b);
	while (b != 0xB5)
	{
		
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	for (int i = 0; i < 9; i++)
	{
		
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	
	
	
	
	
	
	
	
	
	//try to setup inf
	//
	byte inf[] = {0xB5, 0x62, 0x06, 0x02, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0xE0};
			gpsSerialDriver->clearBuffer();
			gpsSerialDriver->transmit((const char*)inf, sizeof(inf));
	gpsSerialDriver->receive(b);
	serialDriver->transmit(b);
	while (b != 0xB5)
	{
		
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	for (int i = 0; i < 9; i++)
	{
		
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	gpsSerialDriver->clearBuffer();
	gpsSerialDriver->transmit((const char*)cfg2, sizeof(cfg2));
	
	UCSR1B &= ~((1<<RXEN1) | (1<<TXEN1));
	UBRR1 = 206;
	UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
	UCSR1A |= (1<<U2X1);
	UCSR1B |= (1<<RXEN1) | (1<<TXEN1);
	
	//read straggling bytes and ack
/*	gpsSerialDriver->receive(b);
	serialDriver->transmit(b);
	while (b != 0xB5)
	{
		
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	for (int i = 0; i < 9; i++)
	{
		
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}	*/

	//gpsSerialDriver->transmit((const char*)cfg, sizeof(cfg));

//
	//for (int i = 0; i < 10; i++)
	//{
	//
		//gpsSerialDriver->receive(b);
		//serialDriver->transmit(b);
	//}
//
//

	

	
	//Transmit a full message
	//gpsSerialDriver->transmit((const char*)posllh, sizeof(posllh));
	
		DDRA |= (1<<PA3);
		
		PORTA &= ~(1<<PA3);
		
		
		
	while (true)
	{
		gpsSerialDriver->transmit((const char*)posllh, sizeof(posllh));
		gpsTimer->startTimer();
		
		while (!gpsTimer->hasTimedout())
		{
			gpsSerialDriver->receive(b);
			serialDriver->transmit(b);
		}
		gpsTimer->stopTimer();
		
		serialDriver->transmit('S');
	//	UBRR1 = UBRR1 + 1;
		serialDriver->transmit((byte)UBRR1);
		
	}		
	
	//see what data is return.
	while (true)
	{
		//byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	return 0;
}



/**
 * Verify that the GPS will ignore additional messages that are sent to it
 * if we didn't fully read the previous message.
 * Results show that I actually will receive the other message. This seems to indicate that
 * the gps will ignore other messages only when it's transmitting an ack.
 * oddly, if I fully transmit one message, then another message without reading anything in between
 * I get the responses to the messages in reverse order. (the message sent last is the first one I get).
 * so the buffer is a LIFO queue.
 */
int gpsPartialReceive_test(TestCase *test)
{
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, NULL);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	serialDriver->initialize();
	
	serialDriver->transmit('T');
	
	byte posllh[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};

	byte posecef[] = {0xB5, 0x62, 0x01, 0x01, 0x00, 0x00, 0x02, 0x07};

	//byte status[] = {0xB5, 0x62, 0x01, 0x03, 0x00, 0x00, 0x04, 0x0D};
	
	byte cfg[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};
	
	gpsSerialDriver->transmit((const char*)cfg, sizeof(cfg));
	
	for (int i = 0; i < 10; i++)
	{
		byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	//Transmit a full message
	gpsSerialDriver->transmit((const char*)posllh, sizeof(posllh));
	
	//partially read the message
	for (int i = 0; i < 13; i++)
	{
		byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	//Transmit a full message
	gpsSerialDriver->transmit((const char*)posecef, sizeof(posecef));
	
	//fully read the message

	
	//see what data is return.
	while (true)
	{
		byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	return 0;
}

/**
 * Test that the gps properly handles partially transmitted message.
 * The lesson learned is that the GPS will only receive data when it's not already
 * transmitting data, and it will 'wait' until it finishes transmitting data
 * before it receives data again.
 */
int partialTransmit_test(TestCase *test)
{
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, NULL);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	serialDriver->initialize();
	
	serialDriver->transmit('T');
	
	byte posllh[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};

	byte posecef[] = {0xB5, 0x62, 0x01, 0x01, 0x00, 0x00, 0x02, 0x07};

	//byte status[] = {0xB5, 0x62, 0x01, 0x03, 0x00, 0x00, 0x04, 0x0D};
		
	byte cfg[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};			
	
	gpsSerialDriver->transmit((const char*)cfg, sizeof(cfg));
	
	for (int i = 0; i < 10; i++)
	{
		byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	//Transmit a partial message
	gpsSerialDriver->transmit((const char*)posllh, sizeof(posllh) - 3);
	
	//Transmit a full message
	gpsSerialDriver->transmit((const char*)posecef, sizeof(posecef));
	

	
	//see what data is return.
	while (true)
	{
		byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
	}
	
	return 0;
}

int nedGps_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 50);
	Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);
	
	/**
	 * Note, this serial driver must be on baud rate 9600 because the GPS code is
	 * hard coded to communicate over serial at 9600 baud.
	 */
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(115200, SerialDriver::Zero, true, timer);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();
	
	//set rate to 5 hz (200 ms)
	//	byte rate[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};
		
	//	gpsSerialDriver->transmit((const char*)rate, sizeof(rate));

	//wait until sensor is ready
	while (!gpsSensor->isGpsReady())
	{
		_delay_ms(50);
		gpsSensor->readSensorNavStatus();
	}
	
	int errorCount1 = 0;
	int errorCount2 = 0;
	
	while (gpsSensor->getXEcefCm() == 0 || gpsSensor->getYEcefCm() == 0 || gpsSensor->getZEcefCm() == 0)
	{
		if(gpsSensor->readSensorNavStatus() != 0) errorCount1++;
		if(gpsSensor->readSensorLLH() != 0) errorCount1++;
		if(gpsSensor->readSensorECEF() != 0) errorCount1++;		
	}
	
	/**
	 * calculate eceftoned rotation matrix and set initial ECEF values
	 */
	float EcefToLocalNEDRotationMatrix[3][3];
	memset(EcefToLocalNEDRotationMatrix, 0, 3 * 3 * sizeof(float));
	
	float latitude = (float) gpsSensor->getLatitudeDegE7() / 10000000.0f;
	float longitude = (float) gpsSensor->getLongitudeDegE7() / 10000000.0f;

	CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(latitude, longitude, EcefToLocalNEDRotationMatrix);

	long initialXPositionEcef = gpsSensor->getXEcefCm();
	long initialYPositionEcef = gpsSensor->getYEcefCm();
	long initialZPositionEcef = gpsSensor->getZEcefCm();
		
	
	while (true)
	{
		
		
		//Read the sensor data
		int status = gpsSensor->readSensorECEF();
		
		if(status == -1)errorCount1++;
		if(status == -2)errorCount2++;
		
		
		//Get the current helicopters position relative to it's starting point in ecef.
		long differenceXECEF = gpsSensor->getXEcefCm() - initialXPositionEcef;
		long differenceYECEF = gpsSensor->getYEcefCm() - initialYPositionEcef;
		long differenceZECEF = gpsSensor->getZEcefCm() - initialZPositionEcef;
		
		
		float rotatedMatrix[3] = {};
		float positionMatrix[3] = {(float)differenceXECEF, (float)differenceYECEF, (float)differenceZECEF};
			
		//Rotate the current ecef position from earth centered earth fixed (ECEF) into North-East-Down(NED).
		//Iterate through the rows of the rotation matrix
		MatrixUtil::RotateMatrix(EcefToLocalNEDRotationMatrix,positionMatrix,rotatedMatrix);
			
		float localNEDXCm = rotatedMatrix[0];
		float localNEDYCm = rotatedMatrix[1];
		float localNEDZCm = rotatedMatrix[2];
		
		
		//Transmit Data
		serialDriver->transmit((byte) 'T'); //sync byte
		
		serialDriver->transmit(localNEDXCm);
		serialDriver->transmit(localNEDYCm);
		serialDriver->transmit(localNEDZCm);
		
		
		serialDriver->transmit(initialXPositionEcef);
		serialDriver->transmit(initialYPositionEcef);
		serialDriver->transmit(initialZPositionEcef);
		serialDriver->transmit(gpsSensor->getXEcefCm());
		serialDriver->transmit(gpsSensor->getYEcefCm());
		serialDriver->transmit(gpsSensor->getZEcefCm());						
		serialDriver->transmit(differenceXECEF);
		serialDriver->transmit(differenceYECEF);
		serialDriver->transmit(differenceZECEF);
		serialDriver->transmit(errorCount1);
		serialDriver->transmit(errorCount2);
		
		
	}
	
	
	return 0;
}




int readRealSensor_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 50);
	Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 500);
	
	/**
	 * Note, this serial driver must be on baud rate 9600 because the GPS code is
	 * hard coded to communicate over serial at 9600 baud.
	 */
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();

	
		
	while (true)
	{
		/*
		byte msg[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};
		gpsSerialDriver->clearBuffer();
		
		gpsSerialDriver->transmit((char *) msg,sizeof(msg));
		
		for (int i = 0; i < 34; i++)
		{
			byte b = 0;
			gpsSerialDriver->receive(b);
			serialDriver->transmit(b);
		}
		*/
		
		serialDriver->transmit((byte) 'T');
		
		//Read the sensor data
		gpsSensor->readSensorECEF();		
		gpsSensor->readSensorLLH();		
		gpsSensor->readSensorNavStatus();
		
		//Transmit Data
		serialDriver->transmit(gpsSensor->getLatitudeDegE7());
		serialDriver->transmit(gpsSensor->getLongitudeDegE7());
		serialDriver->transmit(gpsSensor->getPositionAccuracyEstimateEcefCm());
		serialDriver->transmit(gpsSensor->getPositionFixStatus());
		serialDriver->transmit(gpsSensor->getXEcefCm());
		serialDriver->transmit(gpsSensor->getYEcefCm());
		serialDriver->transmit(gpsSensor->getZEcefCm());
		
		
		
		
		/*
		byte b = 0;
		gpsSerialDriver->receive(b);
		serialDriver->transmit(b);
		*/
		
		
		//_delay_ms(350);
	}
	
	
	return 0;
}






int readSensor_test(TestCase *test)
{
	MockSerialDriver *gpsSerialDriver = new MockSerialDriver(150, true);
	

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	//gpsSensor->init(); 
	
	byte llMsg[] = {0x13, 0x44, 0x99, 0x22, 0xB5, 0x62, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xAA, //first bytes are junk to ensure it skips junk data.
		0xB5, 0x62, 0x01, 0x02, 0x1C, 0x00, //header, id, length
		0x00, 0x00, 0x00, 0x00, 0x7B, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00  //data
	};
	memcpy(gpsSerialDriver->buffer, llMsg, sizeof(llMsg));	
	
	AssertTrue(gpsSensor->getXEcefCm() == 0);
	AssertTrue(gpsSensor->getYEcefCm() == 0);
	AssertTrue(gpsSensor->getZEcefCm() == 0);
	AssertTrue(gpsSensor->getLatitudeDegE7() == 0);
	AssertTrue(gpsSensor->getLongitudeDegE7() == 0);
	AssertTrue(gpsSensor->getPositionAccuracyEstimateEcefCm() == 0);
	AssertTrue(gpsSensor->getPositionFixStatus() == GPSSensor::INVALID);
	
	AssertTrue(gpsSensor->readSensorLLH() == 0);
	AssertTrue(gpsSensor->getLatitudeDegE7() == 124);
	AssertTrue(gpsSensor->getLongitudeDegE7() == 123);
	AssertTrue(gpsSensor->getXEcefCm() == 0);
	AssertTrue(gpsSensor->getYEcefCm() == 0);
	AssertTrue(gpsSensor->getZEcefCm() == 0);
	AssertTrue(gpsSensor->getPositionFixStatus() == GPSSensor::INVALID);
	
	byte ecefMsg[] = {0x13,0x44,0x99,0x22,//first four bytes are junk to ensure it skips junk data.
		0xB5, 0x62, 0x01, 0x01, 0x14, 0x00, //header, id, length
		0x00, 0x00, 0x00, 0x00, 0x7B, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00  //data
	};
	memcpy(gpsSerialDriver->buffer, ecefMsg, sizeof(ecefMsg));
	gpsSerialDriver->receiveCounter = 0;
		
	AssertTrue(gpsSensor->readSensorECEF() == 0);
	AssertTrue(gpsSensor->getXEcefCm() == 123);
	AssertTrue(gpsSensor->getYEcefCm() == 124);
	AssertTrue(gpsSensor->getZEcefCm() == 125);
	
	AssertTrue(gpsSensor->getPositionAccuracyEstimateEcefCm() == 244);
	

	byte navStatusMsg[] = {0x13,0x44,0x99,0x22,//first four bytes are junk to ensure it skips junk data.
		0xB5, 0x62, 0x01, 0x03, 0x10, 0x00, //header, id, length
		0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00  //data
	};
	memcpy(gpsSerialDriver->buffer, navStatusMsg, sizeof(navStatusMsg));
	gpsSerialDriver->receiveCounter = 0;	
	
	AssertTrue(gpsSensor->readSensorNavStatus() == 0);
	AssertTrue(gpsSensor->getPositionFixStatus() == GPSSensor::VALID);
		

	//figure out how to cancel data from gps in ubx mode.

	return 0;
}

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

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;


int parsegpsmsg_test(TestCase *test)
{
	//TODO:
	//long was wrong, find out why - could be because i need to multiply, then cut off.
	//change to subtract from 0 after checking that it's a digit
	//implement more tests with blank data, and other values for lat/long
	//implement test in order to verify what it thinks one meter is. use led to indicate when we got data.
	//Will the x,y,z values be in centimeters or what? That will greatly affect the gain calculations.
	
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	
	/**
	 * Note, this serial driver must be on baud rate 9600 because the GPS code is
	 * hard coded to communicate over serial at 9600 baud.
	 */
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	gpsSerialDriver->initialize();


	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	//gpsSensor->init();
	
	
	gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\r\n");
	
	AssertTrue(gpsSensor->getLatitude() == 472852395UL);
	AssertTrue(gpsSensor->getLongitude() == 85652536UL);
	
	//Test that it correctly represents negative degrees.
	gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,4717.11437,S,00833.91522,W,0.004,77.52,091202,,,A*57\r\n");
	
	AssertTrue(gpsSensor->getLatitude() == -472852395L);
	AssertTrue(gpsSensor->getLongitude() == -85652536L);	
	
	//Test that it correctly appends 0's to the values
	gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,4730.0000,S,00833.91522,W,0.004,77.52,091202,,,A*57\r\n");
		
	AssertTrue(gpsSensor->getLatitude() == -475000000);
	
	//Test a set of blank values because if the status is invalid, the data could be blank because according to the doc
	//the gps wont send invalid data.
	gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,,,,,,,,,,A*57\r\n");
	
	AssertTrue(gpsSensor->getLatitude() == -475000000);//This should be the previous value, it shouldn't change. 
		
	return 0;
}



int readSensor_test(TestCase *test)
{
	MockSerialDriver *driver = new MockSerialDriver(150, true);
	
	char *msg = "$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\r\n";
	
	memcpy(driver->buffer, msg, strlen(msg));
	
	GPSSensor *gpsSensor = new GPSSensor(driver);
	gpsSensor->readSensor();
	
	AssertTrue(gpsSensor->getLatitude() == 472852395UL);
	AssertTrue(gpsSensor->getLongitude() == 85652536UL);
	
	return 0;
}

int readgps_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	
	/**
	 * Note, this serial driver must be on baud rate 9600 because the GPS code is
	 * hard coded to communicate over serial at 9600 baud.
	 */
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, false, NULL);
	gpsSerialDriver->initialize();

	//SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();

	AssertTrue(gpsSensor->getLatitude() == 0.0f);
	AssertTrue(gpsSensor->getLongitude() == 0.0f);	
	
	gpsSensor->readSensor();
	
	//Wait until the GPS sensor is ready.
	//while (!gpsSensor->isGpsReady())
	while (true)
	{
		serialDriver->transmit('S');
		long status = gpsSensor->readSensor();
		if (status == 5439488)
		{
			serialDriver->transmit(11);
		}else
		{
			serialDriver->transmit(status);
		}
		
	/*	byte bt = 0;
				while (bt != '\n')
				{
					gpsSerialDriver->receive(bt);
					serialDriver->transmit(bt);
				}*/
		serialDriver->transmit(gpsSensor->getLatitude());
		serialDriver->transmit(gpsSensor->getLongitude());
		//serialDriver->transmit((const char *) gpsSensor->getRawMsg(),gpsSensor->getMsgBytesRead());
		
		_delay_ms(500);
		
	/*	serialDriver->transmit('S');
		//gpsSerialDriver->transmit("$EIGPQ,RMC*3A\r\n", 17);
		byte bt = 0;
		while (bt != '\n')
		{
			gpsSerialDriver->receive(bt);
			serialDriver->transmit(bt);	
		}*/
		
		//gpsSensor->readSensor();
		/*for (int i = 0; i < 100; i++)
		{
			serialDriver->transmit(gpsSensor->getRawMsg()[i]);
		}*/
		/*while (true)
		{
				gpsSerialDriver->receive(bt);
				serialDriver->transmit(bt);		
		}*/
		
		//serialDriver->transmit((const char *) gpsSensor->getRawMsg(),gpsSensor->getMsgBytesRead());
		
	//	_delay_ms(1000);
		//gpsSerialDriver->transmit("$EIGPQ,RMC*3A\r\n", 17);
		//gpsSensor->readSensor();
	}
	
	AssertTrue(gpsSensor->readSensor());
	
	AssertTrue(gpsSensor->getLatitude() != 0.0f);
	AssertTrue(gpsSensor->getLongitude() != 0.0f);
	
	while (true)
	{
		_delay_ms(500);
		AssertTrue(gpsSensor->readSensor());
		
		serialDriver->transmit('S');

		serialDriver->transmit(gpsSensor->getLatitude());
		serialDriver->transmit(gpsSensor->getLongitude());
		
	}
	
	/**
	 * TODO Notes: I'll want to investigate the 'correction' fields.
	 * perhaps they have a way of correcting the drift I aws seeing and will
	 * give me a more accurate reading. 
	 * I'll want to be sure to implement the position fix flags to ensure
	 * that the GPS is actually activated and has a good fix. (adobe page 51)
	 */
	
	return 0;
}


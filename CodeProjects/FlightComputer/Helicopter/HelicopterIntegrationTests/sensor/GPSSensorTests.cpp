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



int readRealSensor_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 20);
	Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100);
	
	/**
	 * Note, this serial driver must be on baud rate 9600 because the GPS code is
	 * hard coded to communicate over serial at 9600 baud.
	 */
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, NULL);
	gpsSerialDriver->initialize();

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();

	byte msg[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};
		
	while (true)
	{
		/*
		gpsSerialDriver->clearBuffer();
		
		gpsSerialDriver->transmit((char *) msg,sizeof(msg));
		
		for (int i = 0; i < 34; i++)
		{
			byte b = 0;
			gpsSerialDriver->receive(b);
			serialDriver->transmit(b);
		}
		*/
		
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
		
		
		
		_delay_ms(350);
	}
	
	
	return 0;
}






int readSensor_test(TestCase *test)
{
	MockSerialDriver *gpsSerialDriver = new MockSerialDriver(150, true);
	

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init(); 
	
	byte llMsg[] = {0x13,0x44,0x99,0x22,//first four bytes are junk to ensure it skips junk data.
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

/*
int readSensor_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 50); 
	
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, NULL);
	gpsSerialDriver->initialize();


	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	serialDriver->initialize();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();
	
	AssertTrue(gpsSensor->getXEcefCm() == 0);
	AssertTrue(gpsSensor->getYEcefCm() == 0);
	AssertTrue(gpsSensor->getZEcefCm() == 0);
	
	AssertTrue(gpsSensor->readSensor() == 0);
	
	AssertTrue(gpsSensor->getXEcefCm() != 0);
	AssertTrue(gpsSensor->getYEcefCm() != 0);
	AssertTrue(gpsSensor->getZEcefCm() != 0);

	return 0;
}

*/









//
//int parsegpsmsg_test(TestCase *test)
//{
	////TODO:
	////implement test in order to verify what it thinks one meter is. use led to indicate when we got data.
	////Will the x,y,z values be in centimeters or what? That will greatly affect the gain calculations.
	//
	//Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	//
	///**
	 //* Note, this serial driver must be on baud rate 9600 because the GPS code is
	 //* hard coded to communicate over serial at 9600 baud.
	 //*/
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	//gpsSerialDriver->initialize();
//
//
	//SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	//serialDriver->initialize();
//
	//GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	//
	//gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\r\n");
	//
	//AssertTrue(gpsSensor->getLatitude() == 472852395UL);
	//AssertTrue(gpsSensor->getLongitude() == 85652536UL);
	//
	////Test that it correctly represents negative degrees.
	//gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,4717.11437,S,00833.91522,W,0.004,77.52,091202,,,A*57\r\n");
	//
	//AssertTrue(gpsSensor->getLatitude() == -472852395L);
	//AssertTrue(gpsSensor->getLongitude() == -85652536L);	
	//
	////Test that it correctly appends 0's to the values
	//gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,4730.0000,S,00833.91522,W,0.004,77.52,091202,,,A*57\r\n");
		//
	//AssertTrue(gpsSensor->getLatitude() == -475000000);
	//
	////Test a set of blank values because if the status is invalid, the data could be blank because according to the doc
	////the gps wont send invalid data.
	//gpsSensor->parseFields(75, (byte *)"$GPRMC,083559.00,A,,,,,,,,,,A*57\r\n");
	//
	//AssertTrue(gpsSensor->getLatitude() == -475000000);//This should be the previous value, it shouldn't change. 
		//
	//return 0;
//}
//
//int readSensor_test(TestCase *test)
//{
	//MockSerialDriver *driver = new MockSerialDriver(150, true);
	//
	//char msg[] = "$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57\r\n";
	//
	//memcpy(driver->buffer, msg, strlen(msg));
	//
	//GPSSensor *gpsSensor = new GPSSensor(driver);
	//gpsSensor->readSensor();
	//
	//AssertTrue(gpsSensor->getLatitude() == 472852395UL);
	//AssertTrue(gpsSensor->getLongitude() == 85652536UL);
	//
	//return 0;
//}
//
///**
//* We work with degrees in terms of a integer datatype because the cpu can't represent
//* the full degrees value accurately as a single floating point number.
//*/
//float convertToNED( int64_t newLat , int64_t initialLatitude)
//{
	//int64_t exponentShifter = pow(10, GPSSensor::LATLONG_SIGNIFICANT_DIGITS_SHIFTED);
	//
	////Convert latitude to rads. Note: there is a lot of precision lost, but it's OK since the majority
	////of precision loss would have been if we tried to keep latitude completely as a float.
	//float latRads = (newLat / exponentShifter) * (M_PI/180.0);
	//
	////Calculate how many km's per degree of lat.
	//float kmPerDegree = 111.41288 * cos(latRads) - 0.09350 * cos(3 * latRads) + 0.00012 * cos(5 * latRads);
	//
	////Convert to a floating point number (which is now has a small enough number of significant
	////digits for the CPU to represent accurately) and convert to km.
	////latitude difference converted to degrees (hence divide by exponentshifter) = degree difference
	////then multiply by kmPerDegree to get how many km's distance between the two locations, then multiply by 1000
	////to convert km to meters.
	//float distanceMeters = (((newLat - initialLatitude)/(float)exponentShifter) * kmPerDegree) * 1000;
	//
	//return distanceMeters;
//}
//
//
//int readgps_test(TestCase *test)
//{
	//Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	//
	///**
	 //* Note, this serial driver must be on baud rate 9600 because the GPS code is
	 //* hard coded to communicate over serial at 9600 baud.
	 //*/
	////SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, false, NULL);
	//gpsSerialDriver->initialize();
//
	////SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	//SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	//serialDriver->initialize();
//
	//GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	//gpsSensor->init();
//
	//AssertTrue(gpsSensor->getLatitude() == 0.0f);
	//AssertTrue(gpsSensor->getLongitude() == 0.0f);	
	//
	//
	//gpsSensor->readSensor();
	//
	////Wait until the GPS sensor is ready.
	////while (!gpsSensor->isGpsReady())
	//while (true)
	//{
		//serialDriver->transmit('S');
		//
		//long status = gpsSensor->readSensor();
		//
		///*
		//serialDriver->transmit(status);
//
		//serialDriver->transmit(gpsSensor->getLatitude());
		//serialDriver->transmit(gpsSensor->getLongitude());*/
		//for (int i = 0; i < gpsSensor->getMsgBytesRead(); i++)
		//{
			//serialDriver->transmit(gpsSensor->getRawMsg()[i]);
		//}
		//
		//_delay_ms(500);
	//}
	//
	//return 0;
//}
//
//
//
//
//int calcXY_test(TestCase *test)
//{
	//Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 500); //Good timeout when using the USB
	//
	//
	//
	//
////	SerialDriver *gpsSerialDriver2 = new SerialDriver(9600, SerialDriver::One, false, NULL);
////	gpsSerialDriver2->initialize();
////	gpsSerialDriver2->transmit("$PUBX,41,1,0002,0002,38400,1*25\r\n");
////protocol mask (0002) is described on page 131 of ublox6 receiverdescriptionprotocolspec.
////gpsSerialDriver2->transmit("$PUBX,41,1,0002,0002,38400,1*25\r\n");
//
//
//
	//
	///**
	 //* Note, this serial driver must be on baud rate 9600 because the GPS code is
	 //* hard coded to communicate over serial at 9600 baud.
	 //*/
//SerialDriver *gpsSerialDriver3 = new SerialDriver(9600, SerialDriver::One, true, timer);
//gpsSerialDriver3->initialize();
////gpsSerialDriver3->transmit("$PUBX,41,1,0002,0002,38400,0*24\r\n");
	//
////gpsSerialDriver3->transmit("$PUBX,41,1,0002,0002,19200,0*2F\r\n");
//
///*
	//gpsSerialDriver3->transmit("$PUBX,40,RMC,0,0,0,0,0,0*47\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,VTG,0,0,0,0,0,0*5E\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,DTM,0,0,0,0,0,0*46\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GBS,0,0,0,0,0,0*4D\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GPQ,0,0,0,0,0,0*5D\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GRS,0,0,0,0,0,0*5D\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GST,0,0,0,0,0,0*5B\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,GSV,0,0,0,0,0,0*59\r\n");
	//gpsSerialDriver3->transmit("$PUBX,40,THS,0,0,0,0,0,0*54\r\n");
//*/
	//
                             ////gpsSerialDriver3->transmit("$PUBX,41,1,0001,0001,57600,0*2F\r\n");
							 ////gpsSerialDriver3->transmit("$PUBX,41,2,0007,0002,57600,0*29\r\n");
							//// gpsSerialDriver3->transmit("$PUBX,41,1,0007,0002,57600,0*2A\r\n");
////generated using U-Center -> Messages View	
///*						
//byte bytes[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 
	//0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 
	//0x00, 0x00, 0x00, 0xE1, 0x00, 0x00, 0x03, 
	//0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	//0xD8, 0x9D};
//*/	
////gpsSerialDriver3->transmit((const char*)bytes, 28);
//
///*
//byte bytes[] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x03, 0x1B, 0x9A};
//gpsSerialDriver3->transmit((const char*)bytes, 21);
//gpsSerialDriver3->transmit((const char*)bytes);
//*/
//
////gpsSerialDriver3->transmit("$PUBX,41,2,0007,0002,57600,0*29\r\n");
////gpsSerialDriver3->transmit("$PUBX,41,1,0002,0002,38400,0*24\r\n");
////note: double data rate doesn't seem to be needed for 9600 but it does seem to be needed for 38.
//
////SerialDriver *gpsSerialDriver = new SerialDriver(57600, SerialDriver::One, true, NULL);
//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, NULL);
	////SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, timer);
	////SerialDriver *gpsSerialDriver = new SerialDriver(38400, SerialDriver::One, false, NULL);
	//gpsSerialDriver->initialize();
////gpsSerialDriver->transmit("$PUBX,41,2,0002,0002,38400,1*26\r\n");	
////gpsSerialDriver->transmit("$PUBX,41,1,0002,0002,38400,1*25\r\n");	
//
//
////	gpsSerialDriver->transmit("$PUBX,40,RMC,0,0,0,0,0,0*47\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,VTG,0,0,0,0,0,0*5E\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,DTM,0,0,0,0,0,0*46\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GBS,0,0,0,0,0,0*4D\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GPQ,0,0,0,0,0,0*5D\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GRS,0,0,0,0,0,0*5D\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GST,0,0,0,0,0,0*5B\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,GSV,0,0,0,0,0,0*59\r\n");
	//gpsSerialDriver->transmit("$PUBX,40,THS,0,0,0,0,0,0*54\r\n");
//
//gpsSerialDriver->transmit("$PUBX,41,1,0001,0001,9600,0*14\r\n");	
	//
//
////	UBRR1 = 16;
////	UBRR1 = 51;
//
////UBRR1 = 40;	
////gpsSerialDriver->transmit("$PUBX,41,1,0002,0002,38400,0*24\r\n");	
////gpsSerialDriver->transmit("$PUBX,41,1,0002,0002,9600,1*15\r\n");
//
	////SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	//SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, NULL);
	//serialDriver->initialize();
	//
	//byte counter = 0;
	//
////Read ECEf message
////byte bytes[] = {0xB5, 0x62, 0x01, 0x01, 0x00, 0x00, 0x02, 0x07};
////gpsSerialDriver3->transmit((const char*)bytes, sizeof(bytes));
//
//_delay_ms(10);
//gpsSerialDriver3->clearBuffer();
//
////set port config. 
////9600 port config
////byte portCfg[] = {0x24, 0x50, 0x55, 0x42, 0x58, 0x2C, 0x34, 0x31, 0x2C, 0x31, 0x2C, 0x30, 0x30, 0x30, 0x31, 0x2C, 0x30, 0x30, 0x30, 0x31, 0x2C, 0x39, 0x36, 0x30, 0x30, 0x2C, 0x31, 0x2A, 0x31, 0x35, 0x0D, 0x0A};
////byte portCfg[] = {0x24, 0x50, 0x55, 0x42, 0x58, 0x2C, 0x34, 0x31, 0x2C, 0x31, 0x2C, 0x30, 0x30, 0x30, 0x37, 0x2C, 0x30, 0x30, 0x30, 0x31, 0x2C, 0x33, 0x38, 0x34, 0x30, 0x30, 0x2C, 0x31, 0x2A, 0x32, 0x33, 0x0D, 0x0A};
//
////i think this is 34
	//byte portCfg[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x54};
//
//
////		byte portCfg[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x90};
//gpsSerialDriver3->transmit((const char*)portCfg, sizeof(portCfg));
//
//DDRA |= (1<<PA5);
//PORTA &= ~(1<<PA5);
//
//
//
///*
//UCSR1B &= ~((1<<RXEN1) | (1<<TXEN1));	
//UBRR1 = 51;
//
//UCSR1B |= (1<<RXEN1) | (1<<TXEN1);	
//*/
///*
//for (int i = 0; i < 10; i++)
//{
	//byte b = 0;
	//gpsSerialDriver3->receive(b);
	//serialDriver->transmit(b);
//}
//*/
////byte portCfg2[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x02, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x68};
////gpsSerialDriver3->transmit((const char*)portCfg2, sizeof(portCfg2));
//
////read port config
////byte readPortcfg[] = {0xB5, 0x62, 0x06, 0x00, 0x00, 0x00, 0x06, 0x18};
////gpsSerialDriver3->transmit((const char*)readPortcfg, sizeof(readPortcfg));
//
//
//PORTA |= (1<<PA5);	
//
////read debug
////byte readPortcfg[] = {0xB5, 0x62, 0x04, 0x04, 0x00, 0x00, 0x08, 0x1C};
////	byte readPortcfg[] = {0xB5, 0x62, 0x04, 0x00, 0x00, 0x00, 0x04, 0x10};
////gpsSerialDriver3->transmit((const char*)readPortcfg, sizeof(readPortcfg));
//
	//while (true)
	//{
////	gpsSerialDriver3->transmit("$EIGPQ,RMC*3A\r\n");
//
//
//if (counter++ >= 200)
//{
	//counter = 0;
	//
	//if ((PORTA & (1<<PA5)) != 0)
	//{
		//
		//PORTA &= ~(1<<PA5);		
	//}else
	//{
		//PORTA |= (1<<PA5);	
	//}
//
//}
//
//byte b2 = 0;
//
//
//gpsSerialDriver3->startTimer();
//gpsSerialDriver3->receive(b2);
//serialDriver->transmit(b2);
//gpsSerialDriver3->stopTimer();
//
//
//
//
//
///*
//
//gpsSerialDriver3->startTimer();
//int status = gpsSerialDriver3->transmit((const char*)bytes, sizeof(bytes));
//
//byte msgBytes[28] = {0};
//
//byte b2 = 0;
//int counter = 0;
//
//status = gpsSerialDriver3->receive(b2);
//
//while (b2 != 0xB5 && status == 0)
//{
	//status = gpsSerialDriver3->receive(b2);
//}
//
//
//
//if (status == 0)
//{
//msgBytes[0] = b2;	
//
//for (int i = 1; i < 28 && status == 0; i++)
//{
	//counter++;
	//byte b = 0;
	//
	//
	////while (b != '\n')
		//{
			//status = gpsSerialDriver3->receive(b);
//
			//msgBytes[i] = b;
//
//
			//
			////serialDriver->transmit(b);		
		//}
//}
//gpsSerialDriver3->stopTimer();
//
//if (status == 0)
//{
	//
//
//serialDriver->transmit((byte)'S');
//serialDriver->transmit(msgBytes[13]);
//serialDriver->transmit(msgBytes[12]);
//serialDriver->transmit(msgBytes[11]);
//serialDriver->transmit(msgBytes[10]);
//}else
//{
	//serialDriver->transmit((byte)'B');
	//serialDriver->transmit((byte)counter);
//}
//
//
//
//*/
//
//
//
//
//
//
//
//
//
//
//
//
///*for (int i = 0; i < 28; i++)
//{
//
	//
	//serialDriver->transmit(msgBytes[i]);
//}*/
//}
///*
//serialDriver->transmit(msgBytes[13]);
//serialDriver->transmit(msgBytes[12]);
//serialDriver->transmit(msgBytes[11]);
//serialDriver->transmit(msgBytes[10]);
//*/
//
		///*if (counter++ >= 200)
		//{
			//counter = 0;
			//UBRR1 = UBRR1 - 1;
			//serialDriver->transmit('*');
			//serialDriver->transmit('*');
			//char b[3] = {0};
				//itoa((int)UBRR1, b, 10);
			//serialDriver->transmit(b,3);
		//}*/
////	}
	//
	//
	//
	//
	//
	////AssertTrue(gpsSerialDriver->transmit("$PUBX,41,1,0002,0002,9600,1*15\r\n") == 0);
//
	////GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	////gpsSensor->init();
	////
	////
	////
	////
	////
	////
	////
	////
	////
	////
	////
////
	////AssertTrue(gpsSensor->getLatitude() == 0.0f);
	////AssertTrue(gpsSensor->getLongitude() == 0.0f);	
	////
	////
	////gpsSensor->readSensor() == 0;
	//
	///**
	 //* Wait until the GPS has a fixed status.
	 //*/
///*	while (gpsSensor->getPositionFixStatus() != GPSSensor::VALID)
	//{
		//gpsSensor->readSensor();
		//
		//_delay_ms(500);
	//}*/
	//
	///**
	 //* Get the 'initial' lat long.
	 //*/
	////gpsSensor->readSensor();
	////
	////long initialLatitude = gpsSensor->getLatitude();
	////long initialLongitude = gpsSensor->getLongitude();
	////
	////long newLat = 0;
	////long newLong = 0;
	////
	////
	////
	////
	////
	////
	////
	////
	////while (true)
	////{
		////_delay_ms(500);
		//////AssertTrue(gpsSensor->readSensor() == 0);
		////int status = 0;
		////status = gpsSensor->readSensor();
		////
		////newLat = gpsSensor->getLatitude();
		////newLong = gpsSensor->getLongitude();
		////
		/////**
		 ////* The equations for calculating distances is described here: http://www.calgary.rasc.ca/latlong.htm
		 ////* Note: It's probably better to go lat/long->ECEF->NED
		 ////*/
		//////float earthsAverageCircumferenceLat = 6371.00 * 2 * M_PI;
		//////float distancePerDegreeLat = earthsAverageCircumference / 360.0; //circumferance divided by degrees of lat
		////
		////float xNED = convertToNED(newLat,initialLatitude);
////
		////
		////
		////
		////serialDriver->transmit('S');
		//////serialDriver->transmit(xNED);
////
		//////serialDriver->transmit(gpsSensor->getLatitude());
		//////serialDriver->transmit(gpsSensor->getLongitude());
	////
		////serialDriver->transmit((int)status);
		////serialDriver->transmit((const char *) gpsSensor->getRawMsg(), gpsSensor->getMsgBytesRead());
		////
	////}
	//
	//return 0;
//}
//
//
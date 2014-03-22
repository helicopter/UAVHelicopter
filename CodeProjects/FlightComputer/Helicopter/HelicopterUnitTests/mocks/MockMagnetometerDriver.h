/*
 * MockMagnetometerDriver.h
 *
 * Created: 8/21/2013 9:08:05 PM
 *  Author: HP User
 */ 


#ifndef MOCKMAGNETOMETERDRIVER_H_
#define MOCKMAGNETOMETERDRIVER_H_

#include "MagnetometerDriver.h"
#include "CommonHeader.h"

using namespace helicopter::drivers;

//////////////////////////////////////////////////////////////////////////
//This class fakes having a driver read sensor values. The fake sensor readings
//are passed into the constructor
//////////////////////////////////////////////////////////////////////////
class MockMagnetometerDriver : public MagnetometerDriver
{
	public:
		int16_t fakeRawXReading;
		int16_t fakeRawYReading;
		int16_t fakeRawZReading;
		
		MockMagnetometerDriver(int16_t fakeRawXReading, int16_t fakeRawYReading, int16_t fakeRawZReading);	
		
		int init();
		
		int readSensor(int16_t *rawMagX, int16_t *rawMagY, int16_t *rawMagZ);
};



#endif /* MOCKMAGNETOMETERDRIVER_H_ */
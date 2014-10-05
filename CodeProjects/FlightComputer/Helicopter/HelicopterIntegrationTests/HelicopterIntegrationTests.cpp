/*
 * HelicopterIntegrationTests.cpp
 *
 * Created: 8/24/2013 4:25:38 PM
 *  Author: HP User.
 */ 
#include "TestSuite.h"
#include "GroundControlStationIntegrationTests.h"
#include "SerialDriverIntegrationTests.h"
#include "ImuSensorTests.h"
#include "BarometerSensorTests.h"
#include "GPSSensorTests.h"
#include "NavigationPrototypeTests.h"

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

using namespace helicoptertestscommon::util::testframework;


int main(void)
{
	TestSuite helicopterTests;

	//helicopterTests.addTest(radiotransmitandreceive_test, 1);
	//helicopterTests.addTest(serialreceivetransmit_test, 2);
	
//	helicopterTests.addTest(reliablyreceive_test, 4);
//	helicopterTests.addTest(readimu_test, 5);
	//helicopterTests.addTest(readmag_test, 6);
	//helicopterTests.addTest(magdriver_test, 6);
	//helicopterTests.addTest(magdriver_test, 6);
	//helicopterTests.addTest(readbaro_test, 6);
//	helicopterTests.addTest(parsegpsmsg_test, 6);
	//helicopterTests.addTest(readgps_test, 6);
	//helicopterTests.addTest(readSensor_test, 6);
	//helicopterTests.addTest(calcXY_test, 6);
//	helicopterTests.addTest(readRealSensor_test, 6);
	//helicopterTests.addTest(readSensor_test, 6);
	//helicopterTests.addTest(nedGps_test, 6);
//	helicopterTests.addTest(newmsgformat_test, 6);
	//helicopterTests.addTest(radiocontrollerservooutput_test, 6);
	
	//helicopterTests.addTest(partialTransmit_test, 6);
	//helicopterTests.addTest(gpsPartialReceive_test, 6);
	//helicopterTests.addTest(baudchange_test, 6);
//	helicopterTests.addTest(ahrs_test, 6);
//	helicopterTests.addTest(fakedata_test, 3);
	//helicopterTests.addTest(systemtelemetrytransmitandreceive_test, 3);
	//helicopterTests.addTest(magdriver_test,10);
	//helicopterTests.addTest(ahrs_test, 11);
	//helicopterTests.addTest(magdriver_test, 11);
	//helicopterTests.addTest(radiocontrollerservooutput_test, 11);
	//helicopterTests.addTest(gainsmessage_test, 11);
	//helicopterTests.addTest(newmsgformat2_test, 11);
	//helicopterTests.addTest(readbaro_test, 11);
	//helicopterTests.addTest(readRealSensor_test, 11);
	//helicopterTests.addTest(asyncserialdriver_test, 11);
	
	//helicopterTests.addTest(gainsmessageasync_test, 11);
	helicopterTests.addTest(imu_test, 11);
	
	
	helicopterTests.runTests();
	
	for (int i = 0; i < helicopterTests.getFailureCount(); i++)
	{
		int failedMethodId = helicopterTests.getFailures()[i].getTestCaseId();
		int failedEvaluationId = helicopterTests.getFailures()[i].getFailedEvaluationId();
		
		//Just used to prevent warnings.
		printf("%i %i", failedEvaluationId, failedMethodId);
		__asm__("nop");
		//Investigate printf because this is only for simulator. but have it so that it can easily be
		//removed for testing on hardware.
	}
	
	if (helicopterTests.getFailureCount() != 0)
	{
		//Turn on the Red LED to indicate test failure.
		//Configure LED's to blink.
		DDRA |= (1<<PA5);
		
		PORTA &= ~(1<<PA5);
		
		return -1;
	}else
	{
		//Turn on the blue led to indicate test success.
		//Configure LED's to blink.
		DDRA |= (1<<PA3);
		
		PORTA &= ~(1<<PA3);
	}
	
	return 0;
}
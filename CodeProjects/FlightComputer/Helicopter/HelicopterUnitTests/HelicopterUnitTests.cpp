/*
 * HelicopterUnitTests.cpp
 *
 * Created: 2/11/2013 10:35:07 PM
 *  Author: HP User
 */ 
#include "TestSuite.h"

#include "Buffer.h"
#include "BufferTests.h"
#include "SchedulerTest.h"
#include "MagnetometerSensorTests.h"
#include "GroundControlStationUnitTests.h"
#include "TimerTests.h"
#include "PIDControllerTests.h"
#include "ServoDriverTests.h"

#include <stdio.h>
#include <avr/io.h>

using namespace helicopter::buffer;


int main(void)
{	
	TestSuite helicopterTests;
	
	//helicopterTests.addTest(buffercreation_test, 1);
	//helicopterTests.addTest(addbufferrecord_test, 2);
	//helicopterTests.addTest(schedulertask_test, 3);
	//helicopterTests.addTest(radiotransandrec_test, 2);
	//helicopterTests.addTest(readandconvertmagnetometer_test, 3);
//	helicopterTests.addTest(telemetry_test, 4);
//	helicopterTests.addTest(telemetrymessage_test, 5);
//	helicopterTests.addTest(telemetrybuildmessage_test, 6);
//	helicopterTests.addTest(gcsinterfacemessagereceivingandtransmitting_test, 7);
	//helicopterTests.addTest(timeout_test, 8);
	helicopterTests.addTest(calculateYaw_test, 9);
	//helicopterTests.addTest(controltailrotorcollective_test, 10);

	
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
/*
 * HelicopterIntegrationTests.cpp
 *
 * Created: 8/24/2013 4:25:38 PM
 *  Author: HP User.
 */ 
#include "TestSuite.h"
#include "RadioInterfaceIntegrationTests.h"
#include "SerialDriverIntegrationTests.h"

#include <stdio.h>

using namespace helicoptertestscommon::util::testframework;

int main(void)
{
	TestSuite helicopterTests;

	//helicopterTests.addTest(radiotransmitandreceive_test, 1);
	//helicopterTests.addTest(serialreceivetransmit_test, 2);
	//helicopterTests.addTest(systemtelemetrytransmitandreceive_test, 3);
	helicopterTests.addTest(reliablyreceive_test, 4);
	
	
	
	
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
		return -1;
	}
	
	return 0;
}
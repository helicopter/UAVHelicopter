/*
 * TestSuite.cpp
 *
 * Created: 2/24/2013 7:27:54 PM
 *  Author: HP User
 */ 
#include "TestSuite.h"
#include "CommonHeader.h"

using namespace helicoptertestscommon::util::testframework;

void TestSuite::addTest(int (*testCaseFunctionPtr) (TestCase *), int testCaseId)
{
	if (this->testsCount < MAXFAILEDTESTCASES)
	{
		TestCase tc(testCaseId);
			
		//set function pointer
		tc.testCaseFunctionPtr = testCaseFunctionPtr;
			
		//add and resize array.
		this->testCases[this->testsCount].testCaseFunctionPtr = testCaseFunctionPtr;
		this->testCases[this->testsCount].setTestCaseId(testCaseId);
		
		this->testsCount++;
	}

}

void TestSuite::runTests()
{
	for (int i = 0; i < this->testsCount; i++)
	{
		if (this->testCases[i].testCaseFunctionPtr(&this->testCases[i]) != 0)
		{
			this->failedTestCases[this->failureCount].setFailedEvaluationId(this->testCases[i].getFailedEvaluationId());
			this->failedTestCases[this->failureCount].setTestCaseId(this->testCases[i].getTestCaseId());
			this->failedTestCases[this->failureCount].testCaseFunctionPtr = this->testCases[i].testCaseFunctionPtr;
			this->failureCount++;
		}
	}
}
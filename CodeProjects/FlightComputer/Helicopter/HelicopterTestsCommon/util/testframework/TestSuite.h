/*
 * TestSuite.h
 *
 * Created: 2/24/2013 7:13:27 PM
 *  Author: HP User
 */ 


#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include "TestCase.h"


namespace helicoptertestscommon
{
	namespace util
	{
		namespace testframework
		{
			class TestSuite
			{
				private:
	
					//I'm making the number of test cases static to save time
					//by not having to put things on the heap.
					//there is also no 'new' operator.
					static const int MAXTESTCASES = 100;
					static const int MAXFAILEDTESTCASES = 100;
	
					int failureCount;
					int testsCount;
					TestCase testCases[MAXTESTCASES];
					TestCase failedTestCases[MAXFAILEDTESTCASES];
		
				public: 
				
					TestSuite()
					{
						failureCount=0;
						testsCount=0;
					}
	
					void addTest(int (*testCaseFunctionPtr) (TestCase *), int);
		
					TestCase* getFailures()
					{
						return failedTestCases;
					}
		
					int getFailureCount()
					{
						return failureCount;
					}
					int getTestsCount()
					{
						return testsCount;
					}
	
					void runTests();
			};
			}
		}
	}
			

#endif /* TESTSUITE_H_ */
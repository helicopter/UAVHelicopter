/*
 * TestCase.h
 *
 * Created: 2/24/2013 7:16:23 PM
 *  Author: HP User
 */ 


#ifndef TESTCASE_H_
#define TESTCASE_H_

#include "stddef.h"

namespace helicoptertestscommon
{
	namespace util
	{
		namespace testframework
		{
			class TestCase
			{
				private:
				int failedEvaluationId;
				int testCaseId;
				
				public:
				int (*testCaseFunctionPtr) (TestCase *test);
				
				TestCase ()
				{
					this->testCaseId = 0;
					failedEvaluationId = 0;
					testCaseFunctionPtr = NULL;
				}
				
				TestCase (int testCaseId)
				{
					this->testCaseId = testCaseId;
					failedEvaluationId = 0;
				}
				
				~TestCase ()
				{
					
					
				}
				
				int getTestCaseId()
				{
					return testCaseId;
				}
				
				void setTestCaseId(int id)
				{
					testCaseId = id;
				}
				
				int getFailedEvaluationId()
				{
					return failedEvaluationId;
				}
				
				void setFailedEvaluationId(int id)
				{
					failedEvaluationId = id;
				}
			};
		}
	}
}




#endif /* TESTCASE_H_ */
/*
 * IncFile1.h
 *
 * Created: 2/24/2013 6:08:15 PM
 *  Author: HP User
 */ 


#ifndef UnitTestUtils_H_
#define UnitTestUtils_H_

#define AssertTrue2(evaluation,failedEvalId) \
 if (!(evaluation)) \
 { \
	 test->setFailedEvaluationId(failedEvalId); \
	 return -1; \
}

#define AssertTrue(evaluation) \
if (!(evaluation)) \
{ \
	test->setFailedEvaluationId(0); \
	return -1; \
}

	 
#define AssertFalse(evaluation) if (evaluation){ return -1;}	
	
#define AboutEqual(value1, value2) (value2 < value1 + .0001 && value2 > value1 - .0001) ? true : false
			

/*
#define AssertFalse (evaluation)			\
if (evaluation)								\
{											\
	return -1;								\
}*/

#endif /* UnitTestUtils_H_ */
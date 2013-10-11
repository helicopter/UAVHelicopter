/*
 * CircularByteBufferTests.cpp
 *
 * Created: 2/24/2013 2:34:50 PM
 *  Author: HP User
 */ 

#include <string.h>
#include "Buffer.h"
#include "UnitTestUtils.h"
#include "TestCase.h"
#include "CommonHeader.h"


using namespace helicopter::buffer;
using namespace helicoptertestscommon::util::testframework;

class TestDataClass
{
	public:
		int someValue1;
		int someValue2;
		double someValue3;
};

int buffercreation_test(TestCase *test)
{
	Buffer buff;
	AssertTrue2(buff.getBufferSize() == 0, 1);
	AssertTrue2(buff.dequeue() == NULL, 2);
	
	
	return 0;
};

/*
int addbufferrecord_test(TestCase *test)
{

	
	//Need test for thread safety. - but that won't happen.
	
	TestDataClass tc;
	tc.someValue1 = 3;
	tc.someValue2 = 44342;
	tc.someValue3 = 44.0221;
	
	Record r;
	r.setRecordData((void *) &tc, sizeof(tc));
	
	Buffer buff;
	
	buff.enqueue(&r);
	
	AssertTrue(buff.getBufferSize() == 1, 1);
	
	Record *newr = (Record *) buff.dequeue();
	
	AssertTrue(newr != NULL, 2);
	AssertTrue(buff.getBufferSize() == 0, 3);
	AssertTrue(newr != &r, 4);
	//int jj = memcmp(newr, &r, sizeof(r));
	AssertTrue(newr->getSizeOfRecord() == r.getSizeOfRecord(), 5);
	
	AssertTrue(((TestDataClass *)newr->getRecordData())->someValue1 == ((TestDataClass *)r.getRecordData())->someValue1, 6);
	
	AssertTrue(((TestDataClass *)newr->getRecordData())->someValue2 == ((TestDataClass *)r.getRecordData())->someValue2, 7);
	
	AssertTrue(((TestDataClass *)newr->getRecordData())->someValue3 == ((TestDataClass *)r.getRecordData())->someValue3, 8);
	
	
	char *saying = "This is a saying";
	
	Record anotherr;
	anotherr.setRecordData(saying, strlen(saying));
	
	buff.enqueue(&anotherr);
	AssertTrue(buff.getBufferSize() == 1, 9);
	
	//test that memory addresses are different.
	Record *anotherr2 = buff.dequeue();
	
	AssertTrue((size_t)anotherr2->getSizeOfRecord() == strlen(saying), 10);
	AssertTrue(strlen(anotherr2->getRecordData()) == strlen(saying), 11);
	AssertTrue(strncmp(anotherr2->getRecordData(), saying, strlen(saying)) == 0, 12);
	
	return 0;
};
*/
/*
int testMaxMinBuffer_test(TestCase *test)
{

	CircularByteBuffer byteBuffer(50,256);
	
	TestDataClass tc;
	tc.someValue1 = 3;
	tc.someValue2 = 44342;
	tc.someValue3 = 44.0221;
	
	byteBuffer.enqueue(tc, sizeof(tc));
	byteBuffer.enqueue("This is a test", sizeof("This is a test"));
	
	AssertTrue(byteBuffer.getBufferSize() == 2, 1);
	
	AssertTrue(memcmp(byteBuffer.dequeue(),&tc,sizeof(tc)) == 0, 2);
	AssertTrue(memcmp(byteBuffer.dequeue(),&"This is a test",sizeof("This is a test")) == 0, 3);
	AssertTrue(byteBuffer.getBufferSize() == 0, 4);
	AssertTrue(byteBuffer.dequeue() == NULL, 5);
	
	
	//I want to use this for logging.
	//so i'll want to add things like PID values
	//I'll want to add char arrays as well.
	//i'll need to send data to other components like sensor data.
	
	return 0;
};
*/

//test adding
//test adding and removing
//position
//size
//numofitems
//test filling it up, test emptying it out, edge cases.
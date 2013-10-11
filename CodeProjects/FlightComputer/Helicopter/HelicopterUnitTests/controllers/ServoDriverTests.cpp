
/*
 * ControlSurfaceController.cpp
 *
 * Created: 9/30/2013 6:03:52 PM
 *  Author: HP User
 */ 
#include "ServoDriverTests.h"
#include "ServoDriver.h"
#include "ControlSurfaceController.h"
#include "UnitTestUtils.h"
#include "MockServoDriver.h"

using namespace helicopter::drivers;



int controltailrotorcollective_test(TestCase *test)
{
	ServoDriver *driver = new MockServoDriver();
	
	driver->MinControlValue(-.65);
	driver->MaxControlValue(.65);
	
	driver->controlTailRotorCollective(.8);
	AssertTrue( ((MockServoDriver*)driver)->TailRotorCollectiveControl() == .65);
	
	driver->controlTailRotorCollective(.44);
	
	AssertTrue(((MockServoDriver*)driver)->TailRotorCollectiveControl() == .44);
	
	
	driver->controlTailRotorCollective(-.8);
	
	AssertTrue( ((MockServoDriver*)driver)->TailRotorCollectiveControl() == -.65);
	
	driver->controlTailRotorCollective(-.44);
	
	AssertTrue(((MockServoDriver*)driver)->TailRotorCollectiveControl() == -.44);
	
	
	return 0;
}


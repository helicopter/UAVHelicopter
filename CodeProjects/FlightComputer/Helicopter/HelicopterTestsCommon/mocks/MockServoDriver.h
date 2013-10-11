/*
 * MockServoDriver.h
 *
 * Created: 10/1/2013 12:18:44 PM
 *  Author: HP User
 */ 


#ifndef MOCKSERVODRIVER_H_
#define MOCKSERVODRIVER_H_

#include "ServoDriver.h"

using namespace helicopter::drivers;

class MockServoDriver : public ServoDriver
{
	private:
	double tailRotorCollectiveControl;
	
	public:
	
	double TailRotorCollectiveControl()
	{
		return tailRotorCollectiveControl;
	}
	
	void controlTailRotorCollectiveImpl(double controlTailRotorCollective)
	{
		tailRotorCollectiveControl = controlTailRotorCollective;
	}
};



#endif /* MOCKSERVODRIVER_H_ */
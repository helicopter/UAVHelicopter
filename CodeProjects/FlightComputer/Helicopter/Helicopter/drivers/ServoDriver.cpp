/*
 * ServoDriver.cpp
 *
 * Created: 9/30/2013 8:52:05 PM
 *  Author: HP User
 */ 
#include "ServoDriver.h"

using namespace helicopter::drivers;

void ServoDriver::controlTailRotorCollective(double controlTailRotorCollective)
{
	double control = controlTailRotorCollective;
	
	if (control > maxControlValue)
	{
		control = maxControlValue;
	}
	
	if (control < minControlValue)
	{
		control = minControlValue;
	}
	
	controlTailRotorCollectiveImpl(control);
}


void ServoDriver::controlTailRotorCollectiveImpl(double controlTailRotorCollective)
{
	
}
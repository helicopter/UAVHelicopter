/*
 * MockSerialDriver.cpp
 *
 * Created: 9/2/2013 3:33:26 PM
 *  Author: HP User
 */ 
#include "MockSerialDriver.h"


int MockSerialDriver::transmit(byte byteToSend)
{
	if (ignoreTransmits == false)
	{
		buffer[transmitCounter++] = byteToSend;
	}
	
	return 0;
}

int MockSerialDriver::receive(byte &receivedByte)
{
	if (timeoutcount != 0 && receiveCounter >= timeoutcount)
	{
		//simulate 'timeout'
		return -1;
	}
	
	receivedByte = buffer[receiveCounter++];
	
	return 0;
}

void MockSerialDriver::enableTimeout(int tCount)
{
	this->timeoutcount = tCount;
}
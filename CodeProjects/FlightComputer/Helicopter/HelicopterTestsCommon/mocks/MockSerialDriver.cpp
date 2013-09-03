/*
 * MockSerialDriver.cpp
 *
 * Created: 9/2/2013 3:33:26 PM
 *  Author: HP User
 */ 
#include "MockSerialDriver.h"


int MockSerialDriver::transmitByte(byte byteToSend)
{
	buffer[transmitCounter++] = byteToSend;
	
	return 0;
}

int MockSerialDriver::receiveByte(byte &receivedByte)
{
	receivedByte = buffer[receiveCounter++];
	
	return 0;
}

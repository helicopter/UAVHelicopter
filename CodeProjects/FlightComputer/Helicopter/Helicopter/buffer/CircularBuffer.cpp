/*
 * CircularBuffer.cpp
 *
 * Created: 9/1/2014 10:09:57 PM
 *  Author: HP User
 */ 

#include "CircularBuffer.h"

using namespace helicopter::buffer;

bool CircularBuffer::enqueue(byte val)
{
	//if the buffer is full, return false.
	if (bytesInQueue >= bufferSize)
	{
		return false;
	}
		
	buffer[tailIndex++] = val;
	
	if (tailIndex >= bufferSize)
	{
		tailIndex = 0;
	}
	
	bytesInQueue++;
	
	return true;
}

bool CircularBuffer::dequeue(byte &val)
{
	val = 0;
	
	//return false if there is no data to return.
	if (bytesInQueue == 0)
	{
		return false;
	}
	
	val = buffer[headIndex++];
	
	if (headIndex >= bufferSize)
	{
		headIndex = 0;
	}
	
	bytesInQueue--;
	
	return true;
}
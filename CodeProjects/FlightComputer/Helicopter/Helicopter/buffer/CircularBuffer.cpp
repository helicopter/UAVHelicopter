/*
 * CircularBuffer.cpp
 *
 * Created: 9/1/2014 10:09:57 PM
 *  Author: HP User
 */ 

#include "CircularBuffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

using namespace helicopter::buffer;

bool CircularBuffer::enqueue(byte val)
{	

/*
if ((tailIndex == headIndex) && bytesInQueue != 0)
{
	PORTA &= ~(1<<PA5);
}
*/

	//if the buffer is full, return false.
	if (bytesInQueue >= bufferSize)
	{
		return false;
	}
cli();		
	buffer[tailIndex++] = val;
	
	if (tailIndex >= bufferSize)
	{
		tailIndex = 0;
	}
	
	bytesInQueue++;
	
	
sei();
	return true;
}

bool CircularBuffer::dequeue(byte &val)
{

/*
if ((tailIndex == headIndex) && bytesInQueue != 0)
{
	PORTA &= ~(1<<PA5);
}
*/
	val = 0;
	
	//return false if there is no data to return.
	if (bytesInQueue == 0)
	{
		return false;
	}
	cli();	
	val = buffer[headIndex++];
	
	if (headIndex >= bufferSize)
	{
		headIndex = 0;
	}
	
	bytesInQueue--;
	
	sei();
	return true;
}



bool CircularBuffer::enqueueInt(byte val)
{

	if ((tailIndex == headIndex) && bytesInQueue != 0)
	{
		PORTA &= ~(1<<PA5);
	}

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

bool CircularBuffer::dequeueInt(byte &val)
{
	if ((tailIndex == headIndex) && bytesInQueue != 0)
	{
		PORTA &= ~(1<<PA5);
	}
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

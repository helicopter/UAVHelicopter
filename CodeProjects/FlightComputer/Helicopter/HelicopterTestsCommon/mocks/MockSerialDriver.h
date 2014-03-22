/*
 * MockSerialDriver.h
 *
 * Created: 9/2/2013 3:32:58 PM
 *  Author: HP User
 */ 


#ifndef MOCKSERIALDRIVER_H_
#define MOCKSERIALDRIVER_H_

#include "SerialDriver.h"
#include "CommonHeader.h"

using namespace helicopter::drivers;

class MockSerialDriver : public SerialDriver
{
	private:
		int numOfCharsInMsg;
		int timeoutcount;
		bool ignoreTransmits;
		
	public:

	int transmitCounter;
	int receiveCounter;
		
	byte *buffer;

	MockSerialDriver (int numOfChars, bool ignoreTransmits = false):
		SerialDriver(0, SerialDriver::Zero,NULL),

		numOfCharsInMsg(numOfChars),
		timeoutcount(0),
		ignoreTransmits(ignoreTransmits),
		transmitCounter(0),
		receiveCounter(0),
		buffer(NULL)	
	{
		buffer = new byte[numOfChars];
	}
	
	~MockSerialDriver()
	{
		if (buffer != NULL)
		{
			delete buffer;
			buffer = NULL;
		}
	}
	
	//resets the internal state as if it's ready to read a brand new message.
	void reset()
	{
		if (buffer != NULL)
		{
			//commenting out in order to support new test cases.
			//delete buffer;
			buffer = NULL;
		}
		buffer = new byte[numOfCharsInMsg];
		
		transmitCounter = 0;
		receiveCounter = 0;
	}

	void init()
	{
		
	}

	int transmit(byte byteToSend);

	int receive(byte &receivedByte);
	
	void enableTimeout(int tCount);
};

#endif /* MOCKSERIALDRIVER_H_ */
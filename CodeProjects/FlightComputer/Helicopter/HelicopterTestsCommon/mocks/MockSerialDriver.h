/*
 * MockSerialDriver.h
 *
 * Created: 9/2/2013 3:32:58 PM
 *  Author: HP User
 */ 


#ifndef MOCKSERIALDRIVER_H_
#define MOCKSERIALDRIVER_H_

#include "SerialDriver.h"
#include "commonheader.h"

using namespace helicopter::drivers;

class MockSerialDriver : public SerialDriver
{
	private:
	byte *buffer;
	int transmitCounter;
	int receiveCounter;
	int numOfCharsInMsg;
	public:

	MockSerialDriver (int numOfChars):
	SerialDriver(0, SerialDriver::Zero),
	buffer(NULL),
	transmitCounter(0),
	receiveCounter(0),
	numOfCharsInMsg(numOfChars)
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
			delete buffer;
			buffer = NULL;
		}
		buffer = new byte[numOfCharsInMsg];
		
		transmitCounter = 0;
		receiveCounter = 0;
	}

	void initialize()
	{
		
	}

	int transmitByte(byte byteToSend);

	int receiveByte(byte &receivedByte);
};

#endif /* MOCKSERIALDRIVER_H_ */
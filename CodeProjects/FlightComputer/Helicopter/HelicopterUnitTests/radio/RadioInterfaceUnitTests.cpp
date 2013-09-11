/*
 * RadioInterfaceUnitTests.cpp
 *
 * Created: 8/26/2013 11:17:26 AM
 *  Author: HP User
 */ 
#include <string.h>
#include <math.h>

#include "RadioInterfaceUnitTests.h"
#include "SerialDriver.h"
#include "MessageBuilder.h"
#include "RadioInterface.h"
#include "UnitTestUtils.h"
#include "MockSerialDriver.h"
#include "SystemTelemetryMessage.h"

using namespace helicopter::drivers;
using namespace std;
using namespace helicopter::interfaces;
using namespace helicopter::messages;


class TestMessage : public Message
{
	public:
		static const byte YAWMSGID = 122;
		private:
		byte *msg;
		
	public:
		TestMessage(byte *msgToSend): Message(TestMessage::YAWMSGID, strlen(msgToSend)+sizeof(msgType))
		{
			this->msg = new byte[strlen(msgToSend)];
			memcpy(this->msg,msgToSend,strlen(msgToSend));
		}
		
		~TestMessage() {
			if (msg != NULL)
			{
				delete msg;
				msg = NULL;
			}
		}
	
		byte *getBytes()
		{
			byte *sendingMsg = new byte[strlen(msg) + sizeof(this->msgType)];
			sendingMsg[0] = msgType;
			memcpy(&sendingMsg[1],this->msg,strlen(msg));
			return sendingMsg;
		}
	
		byte *getMessage()
		{
			//Note, since i'm returning a pointer to an internal data structure
			//this could be bad if this is object is freed. Don't do this for real.
			//make a copy of the object then send it.
			return msg;
		}
	
		void buildMessage(byte *message)
		{
			if (message != NULL)
			{
				if (msg != NULL)
				{
					delete msg;
					msg = NULL;
				}
				
				this->msg = new byte[strlen(message) - sizeof(msgType)];
				this->msgType = message[0];
				memcpy(msg, &message[1], strlen(message));
			}
		}
	
};

class DummyMessageBuilder : public MessageBuilder
{
	private:
	int numOfBytes;
	public:
	
	DummyMessageBuilder (int numOfBytesInMessage): numOfBytes(numOfBytesInMessage)
	{
	}
	
	~DummyMessageBuilder(){}
	
	Message *buildMessage()
	{
		byte msg[1];
		TestMessage *message = new TestMessage(msg);
		
		message->buildMessage(this->internalMessage);
		
		return message;
	}
	
	int getNumOfBytesForMessage(byte messageType)
	{
		//amount passed in + header.
		return numOfBytes;
	}
};

int radiotransandrec_test(TestCase *test)
{

	//////////////////////////////////////////////////////////////////////////
	// Transmit transmitting and receiving a message
	//////////////////////////////////////////////////////////////////////////
	
	byte testMessage[] = "This is a test message";
	
	int sizeOfMsg = strlen(testMessage) + sizeof(byte); //msg size + msgType size.
	
	MockSerialDriver *mockDriver = new MockSerialDriver(sizeOfMsg);
	
	MessageBuilder *msgBuilder = new DummyMessageBuilder(sizeOfMsg);
	
	RadioInterface radioInterface(mockDriver, msgBuilder);
	
	
	
	Message *transmitMessage = new TestMessage(testMessage);
	
	AssertTrue(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//Corrupt the original message to ensure memory isn't being 'held' onto.
	testMessage[2]='z';
	testMessage[3]='w';
	testMessage[4]='c';
	
	Message *receiveMessage = NULL;
	AssertTrue(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue(receiveMessage->getType() == TestMessage::YAWMSGID, 3);
	
	TestMessage *testMsg = (TestMessage *)receiveMessage;
	
	char *blahMsg = testMsg->getMessage();
	
	AssertTrue(strncmp(testMsg->getMessage(), "This is a test message", strlen(testMessage)) == 0, 4);
	
	delete testMsg;
	
	delete mockDriver;
	
	delete msgBuilder;
	
	
	
	return 0;
}







int telemetry_test(TestCase *test)
{
	MessageBuilder *messageBuilder = new MessageBuilder();
	
	//Create a driver for communicating with the radio.
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true);
	serialDriver->initialize();


	SystemTelemetryMessage *message = new SystemTelemetryMessage();
	
	message->MagX(33);
	message->MagY(32);
	message->MagZ(31);
	
	
	
	MockSerialDriver *mockDriver = new MockSerialDriver(message->getNumOfBytes());
		
		
	RadioInterface radioInterface(mockDriver, messageBuilder);
		
		
	
	
	//TODO add some error handling in here
	AssertTrue(radioInterface.transmit(message) == 0, 1);
	
	delete message;
	message = NULL;



	Message *message2 = NULL;
	AssertTrue(radioInterface.receive(message2) == 0, 2);
	
	AssertTrue(message2->getType() == SystemTelemetryMessage::SystemTelemetryMessageType, 3);
	
	SystemTelemetryMessage *testMsg = (SystemTelemetryMessage *)message2;
	
	AssertTrue(testMsg->MagX() == 33, 4);
	AssertTrue(testMsg->MagY() == 32, 5);
	AssertTrue(testMsg->MagZ() == 31, 6);
	
	
	
	
	


	//////////////////////////////////////////////////////////////////////////
	// Transmit transmitting and receiving a message
	//////////////////////////////////////////////////////////////////////////
	/*
	byte testMessage[] = "This is a test message";
	
	int sizeOfMsg = strlen(testMessage) + sizeof(byte); //msg size + msgType size.
	
	MockSerialDriver *mockDriver = new MockSerialDriver(sizeOfMsg);
	
	MessageBuilder *msgBuilder = new DummyMessageBuilder(sizeOfMsg);
	
	RadioInterface radioInterface(mockDriver, msgBuilder);
	
	
	
	Message *transmitMessage = new TestMessage(testMessage);
	
	AssertTrue(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//Corrupt the original message to ensure memory isn't being 'held' onto.
	testMessage[2]='z';
	testMessage[3]='w';
	testMessage[4]='c';
	
	Message *receiveMessage = NULL;
	AssertTrue(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue(receiveMessage->getType() == TestMessage::YAWMSGID, 3);
	
	TestMessage *testMsg = (TestMessage *)receiveMessage;
	
	char *blahMsg = testMsg->getMessage();
	
	AssertTrue(strncmp(testMsg->getMessage(), "This is a test message", strlen(testMessage)) == 0, 4);
	
	delete testMsg;
	
	delete mockDriver;
	
	delete msgBuilder;
	
	*/
	
	return 0;
}
/*
 * RadioInterfaceTests.cpp
 *
 * Created: 8/25/2013 3:56:49 PM
 *  Author: HP User
 */ 

#include "RadioInterfaceIntegrationTests.h"
#include "SerialDriver.h"
#include "UnitTestUtils.h"
#include "RadioInterface.h"
#include "MockSerialDriver.h"
#include "SystemTelemetryMessage.h"

#include <string.h>

using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::messages;
//
//class YawMessage : public Message
//{
	//public:
		//static const byte YAWMSGID = 122;
		//
	//private:
		//int msgToSend;
	//public:
		//YawMessage(int msgToSend): Message(YAWMSGID, sizeof(msgToSend)+sizeof(msgType)), msgToSend(msgToSend){}
		//~YawMessage() {}
	//
		//byte *getBytes()
		//{
			//byte *msg = new byte[sizeof(msgToSend) + sizeof(msgType)];
			//msg[0] = this->msgType;
			//memcpy(&msg[1], &msgToSend, sizeof(msgToSend));
			//
			//return msg;
		//}
		//
		//int getYaw()
		//{
			//return msgToSend;
		//}
		//
		//void buildMessage(byte *message)
		//{
			//if (message != NULL)
			//{
				//msgType = message[0];
				//
				////what about the msg type?
				//memcpy(&msgToSend, &message[1], sizeof(msgToSend));
			//}
		//}
	//
//};
//
//class TestMessageBuilder : public MessageBuilder
//{
	//public: 
	//
		//Message *buildMessage()
		//{
			//YawMessage *message = new YawMessage(0);
			//
			//message->buildMessage(this->internalMessage);
			//
			//return message;
		//}
		//
		//int getNumOfBytesForMessage(byte messageType)
		//{
			//return sizeof(byte)+sizeof(int);
		//}
//};
//
//int radiotransmitandreceive_test(TestCase *test)
//{
//
	////////////////////////////////////////////////////////////////////////////
	//// Transmit a test message
	////////////////////////////////////////////////////////////////////////////
	//
	//MessageBuilder *msgBuilder = new TestMessageBuilder();
	//
	////note:for production, we'll want to set the variable to 'true'
	////SerialDriver *serialDriver = new SerialDriver(38400, SerialDriver::Zero, false);
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, false, true);
	//serialDriver->initialize();	
	//
	////MockSerialDriver *serialDriver = new MockSerialDriver(msgBuilder->getNumOfBytesForMessage(YawMessage::YAWMSGID));
	////serialDriver->initialize();
//
	//
	//
	//RadioInterface radioInterface(serialDriver, msgBuilder);
	//
	//Message *transmitMessage = new YawMessage(321);
	//
	//AssertTrue(radioInterface.transmit(transmitMessage) == 0, 1);
	//
	//delete transmitMessage;
	//
	////////////////////////////////////////////////////////////////////////////
	//// Test receiving a message
	////////////////////////////////////////////////////////////////////////////
	//Message *receiveMessage = NULL;
	//
	////radioInterface.receive(receiveMessage);
	////serialDriver->reset();
	////radioInterface.transmit(receiveMessage);
	//
	//
	//
	//
	//AssertTrue(radioInterface.receive(receiveMessage) == 0, 2);
	//
	//AssertTrue(receiveMessage->getType() == YawMessage::YAWMSGID, 3);
	//
	//YawMessage *yawMsg = (YawMessage *)receiveMessage;
	//
	//AssertTrue(yawMsg->getYaw() == 321, 4);
	//
	//delete yawMsg;
	//
	////////////////////////////////////////////////////////////////////////////
	//// Loop through a bunch of messages and verify that the data was echoed back
	////////////////////////////////////////////////////////////////////////////
	//for (int i = 0; i < 127; i++)
	//{
		//YawMessage loopMsg(i);
		//
		//AssertTrue(radioInterface.transmit(&loopMsg) == 0, 5);
		//
		//
		//Message *recLoopMsg = NULL;
		//AssertTrue(radioInterface.receive(recLoopMsg) == 0, 6);
		//
		//AssertTrue(recLoopMsg->getType() == YawMessage::YAWMSGID, 7);
		//
		//YawMessage *recYawMsg = (YawMessage *)recLoopMsg;
		//
		//AssertTrue(recYawMsg->getYaw() == i, 8);
		//
		//delete recYawMsg;
	//}
	//
	////Send a signal to the other software indicating that this test passed.
	//transmitMessage = new YawMessage(12);
	//
	//AssertTrue(radioInterface.transmit(transmitMessage) == 0, 9);
	//
	//delete transmitMessage;
	//
	//return 0;
//}
//
//
//
int systemtelemetrytransmitandreceive_test(TestCase *test)
{

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	
	//note:for production, we'll want to set the variable to 'true'
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, false, true);
	serialDriver->initialize();
	
	
	RadioInterface radioInterface(serialDriver);
	
	SystemTelemetryMessage *transmitMessage = new SystemTelemetryMessage();
	transmitMessage->MagX(33);
	transmitMessage->MagY(32);
	transmitMessage->MagZ(31);
	
	AssertTrue(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//////////////////////////////////////////////////////////////////////////
	// Test receiving a message
	//////////////////////////////////////////////////////////////////////////
	SystemTelemetryMessage *receiveMessage = NULL;
	
	AssertTrue(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue(receiveMessage->getType() == SystemTelemetryMessage::SystemTelemetryMessageType, 3);
	
	SystemTelemetryMessage *receivedMsg = (SystemTelemetryMessage *)receiveMessage;
	
	AssertTrue(receivedMsg->MagX() == 33, 4);
	AssertTrue(receivedMsg->MagY() == 32, 5);
	AssertTrue(receivedMsg->MagZ() == 31, 6);
	
	delete receivedMsg;
	
	
	//Send a signal to the other software indicating that this test passed.
	transmitMessage = new SystemTelemetryMessage();
	transmitMessage->MagX(12);
	
	AssertTrue(radioInterface.transmit(transmitMessage) == 0, 7);
	
	delete transmitMessage;
	
	return 0;
}




/**
 * Test that the system can reliably send and receive data repeatedly. 
 */
int reliablyreceive_test(TestCase *test)
{

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	
	//note:for production, we'll want to set the variable to 'true'
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true);
	serialDriver->initialize();
	
	
	RadioInterface radioInterface(serialDriver);
	
	//////////////////////////////////////////////////////////////////////////
	// Test receiving a message
	//////////////////////////////////////////////////////////////////////////
	int status = 0;
	int timeoutErrors = 0;
	int crcErrors = 0;
	int msgTypeErrors = 0;
	
	for (int i = 0; i < 127; i++)
	{
		status = 0;
		
		SystemTelemetryMessage *receiveMessage = NULL;
		
		status = radioInterface.receive(receiveMessage);
		
		switch (status)
		{
			case 0:
				break;
			case -1:
				timeoutErrors++;
				break;
			case -2:
				msgTypeErrors++;
				break;
			case -3:
				crcErrors++;
				break;
			default:
				break;
		}
		
		delete receiveMessage;
	}
	

	SystemTelemetryMessage *transmitMessage = new SystemTelemetryMessage();
	
	transmitMessage->MagX(timeoutErrors);
	transmitMessage->MagY(msgTypeErrors);
	transmitMessage->MagZ(crcErrors);
	
	radioInterface.transmit(transmitMessage);
	
	AssertTrue(timeoutErrors == 0, 1);
	AssertTrue(msgTypeErrors == 0, 2);
	AssertTrue(crcErrors == 0, 3);
	
	delete transmitMessage;
	
	return 0;
}
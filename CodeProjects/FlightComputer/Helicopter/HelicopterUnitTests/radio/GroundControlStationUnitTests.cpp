/*
 * RadioInterfaceUnitTests.cpp
 *
 * Created: 8/26/2013 11:17:26 AM
 *  Author: HP User
 */ 
#include <string.h>
#include <math.h>

#include "GroundControlStationUnitTests.h"
#include "SerialDriver.h"
#include "GroundControlStationInterface.h"
#include "UnitTestUtils.h"
#include "MockSerialDriver.h"
#include "SystemTelemetryMessage.h"

using namespace helicopter::drivers;
using namespace std;
using namespace helicopter::interfaces;
using namespace helicopter::messages;

//
//class TestMessage : public Message
//{
	//public:
		//static const byte YAWMSGID = 122;
		//private:
		//byte *msg;
		//
	//public:
		//TestMessage(byte *msgToSend): Message(TestMessage::YAWMSGID, strlen(msgToSend)+sizeof(msgType))
		//{
			//this->msg = new byte[strlen(msgToSend)];
			//memcpy(this->msg,msgToSend,strlen(msgToSend));
		//}
		//
		//~TestMessage() {
			//if (msg != NULL)
			//{
				//delete msg;
				//msg = NULL;
			//}
		//}
	//
		//byte *getBytes()
		//{
			//byte *sendingMsg = new byte[strlen(msg) + sizeof(this->msgType)];
			//sendingMsg[0] = msgType;
			//memcpy(&sendingMsg[1],this->msg,strlen(msg));
			//return sendingMsg;
		//}
	//
		//byte *getMessage()
		//{
			////Note, since i'm returning a pointer to an internal data structure
			////this could be bad if this is object is freed. Don't do this for real.
			////make a copy of the object then send it.
			//return msg;
		//}
	//
		//void buildMessage(byte *message)
		//{
			//if (message != NULL)
			//{
				//if (msg != NULL)
				//{
					//delete msg;
					//msg = NULL;
				//}
				//
				//this->msg = new byte[strlen(message) - sizeof(msgType)];
				//this->msgType = message[0];
				//memcpy(msg, &message[1], strlen(message));
			//}
		//}
	//
//};
//
//class DummyMessageBuilder : public MessageBuilder
//{
	//private:
	//int numOfBytes;
	//public:
	//
	//DummyMessageBuilder (int numOfBytesInMessage): numOfBytes(numOfBytesInMessage)
	//{
	//}
	//
	//~DummyMessageBuilder(){}
	//
	//Message *buildMessage()
	//{
		//byte msg[1];
		//TestMessage *message = new TestMessage(msg);
		//
		//message->buildMessage(this->internalMessage);
		//
		//return message;
	//}
	//
	//int getNumOfBytesForMessage(byte messageType)
	//{
		////amount passed in + header.
		//return numOfBytes;
	//}
//};
//
//int radiotransandrec_test(TestCase *test)
//{
//
	////////////////////////////////////////////////////////////////////////////
	//// transmit transmitting and receiving a message
	////////////////////////////////////////////////////////////////////////////
	//
	//byte testMessage[] = "This is a test message";
	//
	//int sizeOfMsg = strlen(testMessage) + sizeof(byte); //msg size + msgType size.
	//
	//MockSerialDriver *mockDriver = new MockSerialDriver(sizeOfMsg);
	//
	//RadioInterface radioInterface(mockDriver);
	//
	//
	//Message *transmitMessage = new TestMessage(testMessage);
	//
	//AssertTrue(radioInterface.transmit(transmitMessage) == 0, 1);
	//
	//delete transmitMessage;
	//
	////Corrupt the original message to ensure memory isn't being 'held' onto.
	//testMessage[2]='z';
	//testMessage[3]='w';
	//testMessage[4]='c';
	//
	//Message *receiveMessage = NULL;
	//AssertTrue(radioInterface.receive(receiveMessage) == 0, 2);
	//
	//AssertTrue(receiveMessage->getType() == TestMessage::YAWMSGID, 3);
	//
	//TestMessage *testMsg = (TestMessage *)receiveMessage;
	//
	//char *blahMsg = testMsg->getMessage();
	//
	//AssertTrue(strncmp(testMsg->getMessage(), "This is a test message", strlen(testMessage)) == 0, 4);
	//
	//delete testMsg;
	//
	//delete mockDriver;
	//
	//delete msgBuilder;
	//
	//
	//
	//return 0;
//}
//
//
//




int telemetry_test(TestCase *test)
{	
	//Create a driver for communicating with the radio.
//	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true);
//	serialDriver->initialize();


	SystemTelemetryMessage *message = new SystemTelemetryMessage();
	
	message->MagX(33);
	message->MagY(32);
	message->MagZ(31);
	
	
	
	MockSerialDriver *mockDriver = new MockSerialDriver(message->getMessageSize());
		
		
	GroundControlStationInterface radioInterface(mockDriver);
		
		
	
	
	//TODO add some error handling in here
	AssertTrue(radioInterface.transmit(message) == 0, 1);
	
	delete message;
	message = NULL;



	Message *message2 = NULL;
	AssertTrue(radioInterface.receive(message2) == 0, 2);
	
	
	AssertTrue(message2->getType() == SystemTelemetryMessage::MessageType, 3);
	
	SystemTelemetryMessage *testMsg = (SystemTelemetryMessage *)message2;
	
	AssertTrue(testMsg->MagX() == 33, 4);
	AssertTrue(testMsg->MagY() == 32, 5);
	AssertTrue(testMsg->MagZ() == 31, 6);
	
	
	
	
	


	//////////////////////////////////////////////////////////////////////////
	// transmit transmitting and receiving a message
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



int telemetrymessage_test(TestCase *test)
{
	SystemTelemetryMessage *message = new SystemTelemetryMessage();

	message->MagX(32767);
	message->MagY(125);
	message->MagZ(32);

	AssertTrue(message->getType() == SystemTelemetryMessage::MessageType, 1);
	AssertTrue(SystemTelemetryMessage::MessageSize == 7, 2);

	byte* bytes = message->getBytes();

	//verify bytes in the message.
	AssertTrue(bytes[0] == SystemTelemetryMessage::MessageType,3);
	AssertTrue(bytes[1] == (32767 & 0xFF), 4);
	AssertTrue(bytes[2] == ((32767 >> 8) & 0xFF), 5);
	AssertTrue(bytes[3] == 125, 6);
	AssertTrue(bytes[4] == 0, 7);
	AssertTrue(bytes[5] == 32, 8);
	AssertTrue(bytes[6] == 0, 9);
	
	delete [] bytes;
	
	return 0;
}

int telemetrybuildmessage_test(TestCase *test)
{
	byte* msgBytes = new byte[SystemTelemetryMessage::MessageSize];

	msgBytes[0] = SystemTelemetryMessage::MessageType;
	msgBytes[1] = (32767 & 0xFF);
	msgBytes[2] = ((32767 >> 8) & 0xFF);
	msgBytes[3] = (32766 & 0xFF);
	msgBytes[4] = ((32766 >> 8) & 0xFF);
	msgBytes[5] = (32765 & 0xFF);
	msgBytes[6] = ((32765 >> 8) & 0xFF);

	SystemTelemetryMessage *telMsg = SystemTelemetryMessage::buildMessageSt(msgBytes);

	AssertTrue(telMsg->MagX() == (short)32767, 1);
	AssertTrue(telMsg->MagY() == (short)32766, 2);
	AssertTrue(telMsg->MagZ() == (short)32765, 3);
	
	delete telMsg;
	delete [] msgBytes;
	
	return 0;
}

int gcsinterfacemessagereceivingandtransmitting_test(TestCase *test)
{
	int numOfBytesInFullMsg = SystemTelemetryMessage::MessageSize + GroundControlStationInterface::MsgHeaderFooterSize;
	
	SerialDriver *serialPortInterface = new MockSerialDriver(7+numOfBytesInFullMsg);
	MockSerialDriver *mockSPI = (MockSerialDriver*) serialPortInterface;
            

	GroundControlStationInterface *fci = new GroundControlStationInterface(serialPortInterface);

	SystemTelemetryMessage *message = new SystemTelemetryMessage();

	message->MagX(32767);
	message->MagY(125);
	message->MagZ(32);

	fci->transmit(message);
	

	byte *bytes = mockSPI->buffer;
	byte *bytesCopy = new byte[numOfBytesInFullMsg];
	memcpy(bytesCopy, bytes, numOfBytesInFullMsg);


	AssertTrue(bytes[0] == GroundControlStationInterface::SyncByte1, 1);
	AssertTrue(bytes[1] == GroundControlStationInterface::SyncByte2, 1);
	AssertTrue(bytes[2] == GroundControlStationInterface::SyncByte3, 1);
	AssertTrue(bytes[3] == SystemTelemetryMessage::MessageType, 1);
	AssertTrue(bytes[4] == (32767 & 0xFF), 1);
	AssertTrue(bytes[5] == ((32767 >> 8) & 0xFF), 1);
	AssertTrue(bytes[6] == 125, 1);
	AssertTrue(bytes[7] == 0, 1);
	AssertTrue(bytes[8] == 32, 1);
	AssertTrue(bytes[9] == 0, 1);
	AssertTrue(bytes[10] == 29, 1); 
	AssertTrue(bytes[11] == 183, 1); 

	
	Message *msg = NULL;
	
	fci->receive(msg);
	
	SystemTelemetryMessage *message2 = (SystemTelemetryMessage*) msg;

	AssertTrue(message2->MagX() == message->MagX(), 1);
	AssertTrue(message2->MagY() == message->MagY(), 1);
	AssertTrue(message2->MagZ() == message->MagZ(), 1);


	mockSPI->reset();

	/**
	* Test that the interface properly rejects garbage characters
	*/
	byte *lbytes = new byte[7+numOfBytesInFullMsg];

	//junk data
	lbytes[0] = 0x22;
	lbytes[1] = 0x42;
	lbytes[2] = 0x62;
	lbytes[3] = 0xB5;
	lbytes[4] = 0xB5;
	lbytes[5] = 0xB5;
	lbytes[6] = 0x22;
	
	//Real data
	memcpy(&lbytes[7], bytes, numOfBytesInFullMsg);


	mockSPI->buffer = lbytes;

	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;

	AssertTrue(message2->MagX() == message->MagX(), 1);
	AssertTrue(message2->MagY() == message->MagY(), 1);
	AssertTrue(message2->MagZ() == message->MagZ(), 1);

	mockSPI->reset();

	/**
	* Verify that the message is rejected if the checksum is incorrect.
	*/
	byte checksumA = lbytes[numOfBytesInFullMsg + 5];
	byte checksumB = lbytes[numOfBytesInFullMsg + 6];

	lbytes[numOfBytesInFullMsg + 6] = 0xFF;

	mockSPI->buffer = lbytes;

	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;

	AssertTrue(message2 == NULL, 1);

	lbytes[numOfBytesInFullMsg + 6] = checksumB;
	lbytes[numOfBytesInFullMsg + 5] = 0xFF;

	mockSPI->reset();
	mockSPI->buffer = lbytes;

	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;

	AssertTrue(message2 == NULL, 1);

	lbytes[numOfBytesInFullMsg + 5] = checksumA;

	mockSPI->reset();
	mockSPI->buffer = lbytes;

	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;

	AssertTrue(message2 != NULL, 1);

	/*
	* Verify that a message with an incorrect ID is rejected
	*/
	mockSPI->reset();
	mockSPI->buffer = bytesCopy;
	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;
	AssertTrue(message2 != NULL, 1);

	//corrupt the ID field.
	mockSPI->reset();
	mockSPI->buffer = bytesCopy;
	mockSPI->buffer[3] = 22;
	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;
	AssertTrue(message2 == NULL, 1);

	mockSPI->reset();
	mockSPI->buffer = bytesCopy;
	mockSPI->buffer[3] = SystemTelemetryMessage::MessageType;
	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;
	AssertTrue(message2 != NULL, 1);

	//Remove the last byte simulating a timeout exception since it tried to receive more bytes than were transmited.
	mockSPI->reset();
	mockSPI->buffer = bytesCopy;
	mockSPI->enableTimeout(11);
	fci->receive(msg);
	message2 = (SystemTelemetryMessage *) msg;
	AssertTrue(message2 == NULL, 1);
	
	return 0;
}
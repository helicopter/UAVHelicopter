///*
 //* RadioInterfaceTests.cpp
 //*
 //* Created: 8/25/2013 3:56:49 PM
 //*  Author: HP User
 //*/ 
//
#include "GroundControlStationIntegrationTests.h"
#include "SerialDriver.h"
#include "UnitTestUtils.h"
#include "GroundControlStationInterface.h"
#include "MockSerialDriver.h"
#include "SystemTelemetryMessage.h"

#include <string.h>

using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::messages;
////
////class YawMessage : public Message
////{
	////public:
		////static const byte YAWMSGID = 122;
		////
	////private:
		////int msgToSend;
	////public:
		////YawMessage(int msgToSend): Message(YAWMSGID, sizeof(msgToSend)+sizeof(msgType)), msgToSend(msgToSend){}
		////~YawMessage() {}
	////
		////byte *getBytes()
		////{
			////byte *msg = new byte[sizeof(msgToSend) + sizeof(msgType)];
			////msg[0] = this->msgType;
			////memcpy(&msg[1], &msgToSend, sizeof(msgToSend));
			////
			////return msg;
		////}
		////
		////int getYaw()
		////{
			////return msgToSend;
		////}
		////
		////void buildMessage(byte *message)
		////{
			////if (message != NULL)
			////{
				////msgType = message[0];
				////
				//////what about the msg type?
				////memcpy(&msgToSend, &message[1], sizeof(msgToSend));
			////}
		////}
	////
////};
////
////class TestMessageBuilder : public MessageBuilder
////{
	////public: 
	////
		////Message *buildMessage()
		////{
			////YawMessage *message = new YawMessage(0);
			////
			////message->buildMessage(this->internalMessage);
			////
			////return message;
		////}
		////
		////int getNumOfBytesForMessage(byte messageType)
		////{
			////return sizeof(byte)+sizeof(int);
		////}
////};
////
////int radiotransmitandreceive_test(TestCase *test)
////{
////
	//////////////////////////////////////////////////////////////////////////////
	////// Transmit a test message
	//////////////////////////////////////////////////////////////////////////////
	////
	////MessageBuilder *msgBuilder = new TestMessageBuilder();
	////
	//////note:for production, we'll want to set the variable to 'true'
	//////SerialDriver *serialDriver = new SerialDriver(38400, SerialDriver::Zero, false);
	////SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, false, true);
	////serialDriver->initialize();	
	////
	//////MockSerialDriver *serialDriver = new MockSerialDriver(msgBuilder->getNumOfBytesForMessage(YawMessage::YAWMSGID));
	//////serialDriver->initialize();
////
	////
	////
	////RadioInterface radioInterface(serialDriver, msgBuilder);
	////
	////Message *transmitMessage = new YawMessage(321);
	////
	////AssertTrue(radioInterface.transmit(transmitMessage) == 0, 1);
	////
	////delete transmitMessage;
	////
	//////////////////////////////////////////////////////////////////////////////
	////// Test receiving a message
	//////////////////////////////////////////////////////////////////////////////
	////Message *receiveMessage = NULL;
	////
	//////radioInterface.receive(receiveMessage);
	//////serialDriver->reset();
	//////radioInterface.transmit(receiveMessage);
	////
	////
	////
	////
	////AssertTrue(radioInterface.receive(receiveMessage) == 0, 2);
	////
	////AssertTrue(receiveMessage->getType() == YawMessage::YAWMSGID, 3);
	////
	////YawMessage *yawMsg = (YawMessage *)receiveMessage;
	////
	////AssertTrue(yawMsg->getYaw() == 321, 4);
	////
	////delete yawMsg;
	////
	//////////////////////////////////////////////////////////////////////////////
	////// Loop through a bunch of messages and verify that the data was echoed back
	//////////////////////////////////////////////////////////////////////////////
	////for (int i = 0; i < 127; i++)
	////{
		////YawMessage loopMsg(i);
		////
		////AssertTrue(radioInterface.transmit(&loopMsg) == 0, 5);
		////
		////
		////Message *recLoopMsg = NULL;
		////AssertTrue(radioInterface.receive(recLoopMsg) == 0, 6);
		////
		////AssertTrue(recLoopMsg->getType() == YawMessage::YAWMSGID, 7);
		////
		////YawMessage *recYawMsg = (YawMessage *)recLoopMsg;
		////
		////AssertTrue(recYawMsg->getYaw() == i, 8);
		////
		////delete recYawMsg;
	////}
	////
	//////Send a signal to the other software indicating that this test passed.
	////transmitMessage = new YawMessage(12);
	////
	////AssertTrue(radioInterface.transmit(transmitMessage) == 0, 9);
	////
	////delete transmitMessage;
	////
	////return 0;
////}
////
////
////
int systemtelemetrytransmitandreceive_test(TestCase *test)
{

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);
	
	//note:for production, we'll want to set the variable to 'true'
	SerialDriver *serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, t);
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, t, false, true);
	serialDriver->initialize();
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	SystemModel *model = new SystemModel();
	model->ChecksumErrors(2);
	model->MagYawDegrees(2.22);
	model->Timeouts(2);
	model->UnrecognizedMsgTypes(2);
	model->YawControl(2.22);
	model->YawDerivativeError(2.22);
	model->YawIntegral(2.22);
	model->YawProportional(2.22);
	model->YawVelocityDegreesPerSecond(2.22);
	
	
	model->AltitudeFeetAgl(2.22);
	model->LateralControl(2.22);
	model->LateralControlBeforeServoLimitsAdjustment(2.22);
	model->LongitudeControl(2.22);
	model->MainRotorCollectiveControl(2.22);
	
	
	SystemTelemetryMessage *transmitMessage = SystemTelemetryMessage::buildMessageFromModel(model);
	
	AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//////////////////////////////////////////////////////////////////////////
	// Test receiving a message
	//////////////////////////////////////////////////////////////////////////
	Message *receiveMessage = NULL;
	
	AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue2(receiveMessage->getType() == SystemTelemetryMessage::MessageType, 3);
	
	SystemTelemetryMessage *receivedMsg = (SystemTelemetryMessage *)receiveMessage;
	
	SystemModel *model2 = new SystemModel();
	
	receivedMsg->updateModelFromMessage(model2);
	
	AssertTrue(model2->ChecksumErrors() == 2);
	AssertTrue(model2->MagYawDegrees() == 2.22);
	AssertTrue(model2->Timeouts() == 2);
	AssertTrue(model2->UnrecognizedMsgTypes() == 2);
	AssertTrue(model2->YawControl() == 2.22);
	AssertTrue(model2->YawDerivativeError() == 2.22);
	AssertTrue(model2->YawIntegral() == 2.22);
	AssertTrue(model2->YawProportional() == 2.22);
	AssertTrue(model2->YawVelocityDegreesPerSecond() == 2.22);
	
	delete receivedMsg;
	
	
	//Send a signal to the other software indicating that this test passed.
	transmitMessage = new SystemTelemetryMessage();
	transmitMessage->MagYaw = 12;
	
	AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 7);
	
	delete transmitMessage;

	return 0;
}
//
//
//
//
///**
 //* Test that the system can reliably send and receive data repeatedly. 
 //*/
//int reliablyreceive_test(TestCase *test)
//{
//
	////////////////////////////////////////////////////////////////////////////
	//// Transmit a test message
	////////////////////////////////////////////////////////////////////////////
	//
	////note:for production, we'll want to set the variable to 'true'
	//Timer *timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,75);
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, timer, true, true);
	//serialDriver->initialize();
	//
	//
	//GroundControlStationInterface radioInterface(serialDriver);
	//
	////////////////////////////////////////////////////////////////////////////
	//// Test receiving a message
	////////////////////////////////////////////////////////////////////////////
	//int status = 0;
	//int timeoutErrors = 0;
	//int crcErrors = 0;
	//int msgTypeErrors = 0;
	//
	//SystemTelemetryMessage message;
	//message.MagX(12);
	//radioInterface.transmit(&message);
	//
	//for (int i = 0; i < 127; i++)
	//{
		//status = 0;
		//
		//Message *receiveMessage = NULL;
		//
		//status = radioInterface.receive(receiveMessage);
		//
		//switch (status)
		//{
			//case 0:
				//break;
			//case -1:
				//timeoutErrors++;
				//break;
			//case -2:
				//msgTypeErrors++;
				//break;
			//case -3:
				//crcErrors++;
				//break;
			//default:
				//break;
		//}
		//
		//AssertTrue2(((SystemTelemetryMessage*)receiveMessage)->MagX() == i, 1);
		//AssertTrue2(((SystemTelemetryMessage*)receiveMessage)->MagY() == i, 1);
		//AssertTrue2(((SystemTelemetryMessage*)receiveMessage)->MagZ() == i, 1);
		//AssertTrue(((SystemTelemetryMessage*)receiveMessage)->MagYaw() == i);
		//AssertTrue(((SystemTelemetryMessage*)receiveMessage)->YawDerivativeError() == i);
		//AssertTrue(((SystemTelemetryMessage*)receiveMessage)->YawIntegral() == i);
		//AssertTrue(((SystemTelemetryMessage*)receiveMessage)->YawProportional() == i);
		//AssertTrue(((SystemTelemetryMessage*)receiveMessage)->YawVelocityDegreesPerSecond() == i);
		//
		//AssertTrue2(((SystemTelemetryMessage*)receiveMessage)->ChecksumErrors() == i, 1);
		//AssertTrue2(((SystemTelemetryMessage*)receiveMessage)->Timeouts() == i, 1);
		//AssertTrue2(((SystemTelemetryMessage*)receiveMessage)->UnrecognizedMsgTypes() == i, 1);
		//
		//delete receiveMessage;
	//}
	//
//
	//SystemTelemetryMessage *transmitMessage = new SystemTelemetryMessage();
	//
	//transmitMessage->MagX(timeoutErrors);
	//transmitMessage->MagY(msgTypeErrors);
	//transmitMessage->MagZ(crcErrors);
	//transmitMessage->MagYaw(0);
	//transmitMessage->YawDerivativeError(0);
	//transmitMessage->YawIntegral(0);
	//transmitMessage->YawProportional(0);
	//transmitMessage->YawVelocityDegreesPerSecond(0);
	//
	//transmitMessage->ChecksumErrors(0);
	//transmitMessage->Timeouts(0);
	//transmitMessage->UnrecognizedMsgTypes(0);
	//
	//radioInterface.transmit(transmitMessage);
	//
	//AssertTrue2(timeoutErrors == 0, 1);
	//AssertTrue2(msgTypeErrors == 0, 2);
	//AssertTrue2(crcErrors == 0, 3);
	//
	//delete transmitMessage;
	//
	//return 0;
//}
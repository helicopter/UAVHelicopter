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
#include "SensorDataMessage.h"

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

int newmsgformat_test(TestCase *test)
{

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);

	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, t);
	serialDriver->init();
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	
	SensorDataMessage *transmitMessage = new SensorDataMessage();
	transmitMessage->PitchAngularVelocityRadsPerSecond = 3.14;
	transmitMessage->PressureMillibars = 44.313;
	transmitMessage->RollAngularVelocityRadsPerSecond = 4422.1;
	transmitMessage->XAccelFrdMss = 33.33;
	transmitMessage->XEcefCm = 331.2;
	transmitMessage->XMagFrd = 22.2;
	
	AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//////////////////////////////////////////////////////////////////////////
	// Test receiving a message
	//////////////////////////////////////////////////////////////////////////
	Message *receiveMessage = NULL;
	
	AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue2(receiveMessage->getType() == SensorDataMessage::MessageType, 4);
	
	SensorDataMessage *receivedMsg = (SensorDataMessage *)receiveMessage;
	
	
	AssertTrue(receivedMsg->PitchAngularVelocityRadsPerSecond == 3.14f);
	AssertTrue(receivedMsg->PressureMillibars == 44.313f);
	AssertTrue(receivedMsg->RollAngularVelocityRadsPerSecond == 4422.1f);
	AssertTrue(receivedMsg->XAccelFrdMss == 33.33f);
	AssertTrue(receivedMsg->XEcefCm == 331);
	AssertTrue(receivedMsg->XMagFrd == 22.2f);

	
	delete receivedMsg;
	
	
	//Send a signal to the other software indicating that this test passed.
	SensorDataMessage *transmitMessage2 = new SensorDataMessage();
	transmitMessage2->PressureMillibars = 12;
	
	AssertTrue2(radioInterface.transmit(transmitMessage2) == 0, 7);
	
	delete transmitMessage2;

	return 0;
}









int systemtelemetrytransmitandreceive_test(TestCase *test)
{

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);
	
	//note:for production, we'll want to set the variable to 'true'
	//SerialDriver *serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, t);
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, false, t);
	serialDriver->init();
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	SystemModel *model = new SystemModel();
	model->ChecksumErrors(2);
//	model->MagYawDegrees(2.22);
	model->Timeouts(2);
	model->UnrecognizedMsgTypes(2);
	model->YawDerivativeError(2.22);
	model->YawIntegral(2.22);
	model->YawProportional(2.22);
	model->YawAngularVelocityRadsPerSecond(2.22);
	
	
	//model->AltitudeMetersAgl(4.46);
	model->LateralControl(2.22);
	model->LateralControlBeforeServoLimitsAdjustment(2.22);
	model->LongitudeControl(2.22);
	model->MainRotorCollectiveControl(2.22);
	
	model->YawControl(3.22f);
	model->XProportional(4.22f);
	model->YProportional(1.22f);
	model->ZProportional(6.22f);
	model->ZIntegral(8.22f);
	model->ZNEDLocalFrameCm(9.22f);
	model->XNEDLocalFrameCm(19.22f);
	model->YNEDLocalFrameCm(21.33f);
	model->XVEcefCms(42);
	model->YVEcefCms(12);
	model->ZVEcefCms(32);
	
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
	//AssertTrue(model2->MagYawDegrees() == 2.22);
	AssertTrue(model2->Timeouts() == 2);
	AssertTrue(model2->UnrecognizedMsgTypes() == 2);
	AssertTrue(model2->YawControl() == 3.22);
	AssertTrue(model2->YawDerivativeError() == 2.22);
	AssertTrue(model2->YawIntegral() == 2.22);
	AssertTrue(model2->YawProportional() == 2.22);
	AssertTrue(model2->YawAngularVelocityRadsPerSecond() == 2.22);
	
	AssertTrue(model2->XVEcefCms() == 42);
	AssertTrue(model2->YVEcefCms() == 12);
	AssertTrue(model2->ZVEcefCms() == 32);

	/* Can't test these values because I don't convert them.
	AssertTrue(model2->YawControl() == 3.22f);
	AssertTrue(model2->XProportional() == 4.22f);
	AssertTrue(model2->YProportional() == 1.22f);
	AssertTrue(model2->ZProportional() == 6.22f);
	AssertTrue(model2->ZIntegral() == 8.22f);
	AssertTrue(model2->ZNEDLocalFrame() == 9.22f);
	AssertTrue(model2->XNEDLocalFrame() == 19.22f);
	*/
	
	delete receivedMsg;
	
	
	//Send a signal to the other software indicating that this test passed.
	transmitMessage = new SystemTelemetryMessage();
	transmitMessage->PressureMillibars = 12;
	
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
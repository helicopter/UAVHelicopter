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
#include "RadioControllerInterface.h"
#include "ServoControlTask.h"
#include "SimpleTelemetryMessage.h"
#include "MagnetometerSensor.h"
#include "GainsMessage.h"
#include "GPSSensor.h"

#include <string.h>
#include <util/delay.h>

using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::messages;
using namespace helicopter::sensors;


float getFloat(SerialDriver *serialDrive)
{
	byte b[4] = {0};
	float t = 0;
	
	serialDrive->receive(b[0]);
	serialDrive->receive(b[1]);
	serialDrive->receive(b[2]);
	serialDrive->receive(b[3]);
	
	memcpy(&t, b, sizeof(float));
	
	return t;
}


int gainsmessageasync_test(TestCase *test)
{
	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);

	SerialDriver *serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, true, t);
	serialDriver->init();
	
	
	
	
	
	
	SerialDriver *gpsSerialDriver = new SerialDriver(38400, SerialDriver::One, true, false, NULL);
	gpsSerialDriver->init();	
	
	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();	
	
	gpsSensor->start();
	
	
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	for (int i = 0; i < 1000; i++)
	{
		
		int status = gpsSensor->processSensorSolution();
		
		SystemTelemetryMessage *transmitMessage = new SystemTelemetryMessage();
		transmitMessage->LateralInnerLoopGain = 1.1f;
		transmitMessage->LongitudeInnerLoopGain = 1.2f;
		transmitMessage->PitchAngularVelocityGain = 1.3f;
		transmitMessage->RollAngularVelocityGain = 1.4f;
		transmitMessage->XAntiWindupGain = 1.5f;
		transmitMessage->XDerivativeGain = 1.6f;
		transmitMessage->XIntegralGain = 1.7f;
		transmitMessage->XProportionalGain = 1.8f;
		transmitMessage->YAntiWindupGain = 1.9f;
		transmitMessage->YawAntiWindupGain = 1.0f;
		transmitMessage->YawDerivativeGain = 1.11f;
		transmitMessage->YawIntegralGain = 1.12f;
		transmitMessage->YawProportionalGain = 1.13f;
		transmitMessage->YDerivativeGain = 1.14f;
		transmitMessage->YIntegralGain = 1.15f;
		transmitMessage->YProportionalGain = 1.16f;
		transmitMessage->ZAntiWindupGain = 1.17f;
		transmitMessage->ZDerivativeGain = 1.18f;
		transmitMessage->ZIntegralGain = 1.19f;
		transmitMessage->ZProportionalGain = i;
		transmitMessage->XRefSetpoint = 3.14159f;
		transmitMessage->AltitudeMetersAgl = 1.233f;
		transmitMessage->ChecksumErrors = 11;
		transmitMessage->LateralControl = 332.1f;
		transmitMessage->LatitudeDegrees = 12.11f;
		transmitMessage->LongitudeControl = 8.8f;
		transmitMessage->LongitudeDegrees = 9.9f;
		transmitMessage->MainRotorCollectiveControl = 8.23f;
		transmitMessage->NumOfBlownFrames = 8;
		transmitMessage->PitchAngularVelocityRadsPerSecond=8.2f;
		transmitMessage->PitchRads = 7.77f;
		transmitMessage->PressureMillibars = 3.2f;
		transmitMessage->RollAngularVelocityRadsPerSecond=21.3f;
		transmitMessage->RollRads = 7.1f;
		transmitMessage->Timeouts=98;
		transmitMessage->UnrecognizedMsgTypes=21;
		
		

		
		AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 1);
		
		delete transmitMessage;
		
		//////////////////////////////////////////////////////////////////////////
		// Test receiving a message
		//////////////////////////////////////////////////////////////////////////
		Message *receiveMessage = NULL;
		
		//_delay_ms(34);//minimum transmit time. 
		_delay_ms(50);
		
		
		
		
		
		int stat = radioInterface.receive(receiveMessage);
		
		if (stat == -4)
		{
					DDRA |= (1<<PA4);
					
					PORTA &= ~(1<<PA4);
		}
		
		
		AssertTrue2( stat== 0, 2);
		
		AssertTrue2(receiveMessage->getType() == SystemTelemetryMessage::MessageType, 7);
		
		SystemTelemetryMessage *receivedMsg = (SystemTelemetryMessage *)receiveMessage;
		
		
		
		AssertTrue(receivedMsg->LateralInnerLoopGain == 1.1f);
		AssertTrue(receivedMsg->LongitudeInnerLoopGain == 1.2f);
		AssertTrue(receivedMsg->PitchAngularVelocityGain == 1.3f);
		AssertTrue(receivedMsg->RollAngularVelocityGain == 1.4f);
		AssertTrue(receivedMsg->XAntiWindupGain == 1.5f);
		AssertTrue(receivedMsg->XDerivativeGain == 1.6f);
		AssertTrue(receivedMsg->XIntegralGain == 1.7f);
		AssertTrue(receivedMsg->XProportionalGain == 1.8f);
		AssertTrue(receivedMsg->YAntiWindupGain == 1.9f);
		AssertTrue(receivedMsg->YawAntiWindupGain == 1.0f);
		AssertTrue(receivedMsg->YawDerivativeGain == 1.11f);
		AssertTrue(receivedMsg->YawIntegralGain == 1.12f);
		AssertTrue(receivedMsg->YawProportionalGain == 1.13f);
		AssertTrue(receivedMsg->YDerivativeGain == 1.14f);
		AssertTrue(receivedMsg->YIntegralGain == 1.15f);
		AssertTrue(receivedMsg->YProportionalGain == 1.16f);
		AssertTrue(receivedMsg->ZAntiWindupGain == 1.17f);
		AssertTrue(receivedMsg->ZDerivativeGain == 1.18f);
		AssertTrue(receivedMsg->ZIntegralGain == 1.19f);
		AssertTrue(receivedMsg->ZProportionalGain == i);
		

		
		delete receivedMsg;
		
		
	}
	
	
	//Send a signal to the other software indicating that this test passed.
	SystemTelemetryMessage *transmitMessage2 = new SystemTelemetryMessage();
	transmitMessage2->ZProportionalGain = 12;
	
	AssertTrue2(radioInterface.transmit(transmitMessage2) == 0, 7);
	
	
	delete transmitMessage2;
	
	return 0;
}


//int gainsmessageasync_test(TestCase *test)
//{
	////////////////////////////////////////////////////////////////////////////
	//// Transmit a test message
	////////////////////////////////////////////////////////////////////////////
	//Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);
//
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true, t);
	//serialDriver->init();
	//
	//
	//GroundControlStationInterface radioInterface(serialDriver);
	//
	//for (int i = 0; i < 100; i++)
	//{
	//
	//
	//
		//GainsMessage *transmitMessage = new GainsMessage();
		//transmitMessage->LateralInnerLoopGain = 1.1f;
		//transmitMessage->LongitudeInnerLoopGain = 1.2f;
		//transmitMessage->PitchAngularVelocityGain = 1.3f;
		//transmitMessage->RollAngularVelocityGain = 1.4f;
		//transmitMessage->XAntiWindupGain = 1.5f;
		//transmitMessage->XDerivativeGain = 1.6f;
		//transmitMessage->XIntegralGain = 1.7f;
		//transmitMessage->XProportionalGain = 1.8f;
		//transmitMessage->YAntiWindupGain = 1.9f;
		//transmitMessage->YawAntiWindupGain = 1.0f;
		//transmitMessage->YawDerivativeGain = 1.11f;
		//transmitMessage->YawIntegralGain = 1.12f;
		//transmitMessage->YawProportionalGain = 1.13f;
		//transmitMessage->YDerivativeGain = 1.14f;
		//transmitMessage->YIntegralGain = 1.15f;
		//transmitMessage->YProportionalGain = 1.16f;
		//transmitMessage->ZAntiWindupGain = 1.17f;
		//transmitMessage->ZDerivativeGain = 1.18f;
		//transmitMessage->ZIntegralGain = 1.19f;
		//transmitMessage->ZProportionalGain = i;
		//transmitMessage->XRefSetpoint = 3.14159f;
	//
	//
//
	//
		//AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 1);
	//
		//delete transmitMessage;
	//
		////////////////////////////////////////////////////////////////////////////
		//// Test receiving a message
		////////////////////////////////////////////////////////////////////////////
		//Message *receiveMessage = NULL;
	//
		//_delay_ms(60);
	//
		//AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	//
		//AssertTrue2(receiveMessage->getType() == GainsMessage::MessageType, 7);
	//
		//GainsMessage *receivedMsg = (GainsMessage *)receiveMessage;
	//
	//
	//
		//AssertTrue(receivedMsg->LateralInnerLoopGain == 1.1f);
		//AssertTrue(receivedMsg->LongitudeInnerLoopGain == 1.2f);
		//AssertTrue(receivedMsg->PitchAngularVelocityGain == 1.3f);
		//AssertTrue(receivedMsg->RollAngularVelocityGain == 1.4f);
		//AssertTrue(receivedMsg->XAntiWindupGain == 1.5f);
		//AssertTrue(receivedMsg->XDerivativeGain == 1.6f);
		//AssertTrue(receivedMsg->XIntegralGain == 1.7f);
		//AssertTrue(receivedMsg->XProportionalGain == 1.8f);
		//AssertTrue(receivedMsg->YAntiWindupGain == 1.9f);
		//AssertTrue(receivedMsg->YawAntiWindupGain == 1.0f);
		//AssertTrue(receivedMsg->YawDerivativeGain == 1.11f);
		//AssertTrue(receivedMsg->YawIntegralGain == 1.12f);
		//AssertTrue(receivedMsg->YawProportionalGain == 1.13f);
		//AssertTrue(receivedMsg->YDerivativeGain == 1.14f);
		//AssertTrue(receivedMsg->YIntegralGain == 1.15f);
		//AssertTrue(receivedMsg->YProportionalGain == 1.16f);
		//AssertTrue(receivedMsg->ZAntiWindupGain == 1.17f);
		//AssertTrue(receivedMsg->ZDerivativeGain == 1.18f);
		//AssertTrue(receivedMsg->ZIntegralGain == 1.19f);
		//AssertTrue(receivedMsg->ZProportionalGain == i);
	//
//
	//
		//delete receivedMsg;
	//
			//
	//}
	//
	//
	////Send a signal to the other software indicating that this test passed.
	//GainsMessage *transmitMessage2 = new GainsMessage();
	//transmitMessage2->ZProportionalGain = 12;
	//
	//AssertTrue2(radioInterface.transmit(transmitMessage2) == 0, 7);
	//
	//
	//delete transmitMessage2;
	//
	//return 0;
//}















int asyncserialdriver_test(TestCase *test)
{
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);
	
	
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true, t);//true flag for asynchronous receive
	serialDriver->init();
	
	byte b;
	int success = -1;
	

	while (success != 0)
	{
		success = serialDriver->receive(b);
	}
	
	AssertTrue(b == 'T');

	success = -1;
	
	while (success != 0)
	{
		success = serialDriver->receive(b);
	}
	
	AssertTrue(b == 'a');
	
	_delay_ms(5000); // wait for data to get backed up
	
	AssertTrue(serialDriver->receive(b) == 0);
	
	AssertTrue(b == 'f');
	
	AssertTrue(serialDriver->receive(b) == 0);
	
	AssertTrue(b == 'y');	
	
	AssertTrue(serialDriver->receive(b) == 0);
	
	AssertTrue(b == 'a');
	
	serialDriver->transmit((byte) 'L');
	
	return 0;	
}



int asyncmessages_test(TestCase *test)
{

	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);

	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true, t);
	serialDriver->init();
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	
	
	Message *receiveMessage = NULL;
	
	AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue2(receiveMessage->getType() == GainsMessage::MessageType, 7);
	
	GainsMessage *receivedMsg = (GainsMessage *)receiveMessage;
	
	
	
	AssertTrue(receivedMsg->LateralInnerLoopGain == 1.1f);	

	return 0;
}




int gainsmessage_test(TestCase *test)
{
	

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);

	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, t);
	serialDriver->init();
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	
	GainsMessage *transmitMessage = new GainsMessage();
	transmitMessage->LateralInnerLoopGain = 1.1f;
	transmitMessage->LongitudeInnerLoopGain = 1.2f;
	transmitMessage->PitchAngularVelocityGain = 1.3f;
	transmitMessage->RollAngularVelocityGain = 1.4f;
	transmitMessage->XAntiWindupGain = 1.5f;
	transmitMessage->XDerivativeGain = 1.6f;
	transmitMessage->XIntegralGain = 1.7f;
	transmitMessage->XProportionalGain = 1.8f;
	transmitMessage->YAntiWindupGain = 1.9f;
	transmitMessage->YawAntiWindupGain = 1.0f;
	transmitMessage->YawDerivativeGain = 1.11f;
	transmitMessage->YawIntegralGain = 1.12f;
	transmitMessage->YawProportionalGain = 1.13f;
	transmitMessage->YDerivativeGain = 1.14f;
	transmitMessage->YIntegralGain = 1.15f;
	transmitMessage->YProportionalGain = 1.16f;
	transmitMessage->ZAntiWindupGain = 1.17f;
	transmitMessage->ZDerivativeGain = 1.18f;
	transmitMessage->ZIntegralGain = 1.19f;
	transmitMessage->ZProportionalGain = 1.20f;
	
	AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//////////////////////////////////////////////////////////////////////////
	// Test receiving a message
	//////////////////////////////////////////////////////////////////////////
	Message *receiveMessage = NULL;
	
	AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue2(receiveMessage->getType() == GainsMessage::MessageType, 7);
	
	GainsMessage *receivedMsg = (GainsMessage *)receiveMessage;
	
	
	
	AssertTrue(receivedMsg->LateralInnerLoopGain == 1.1f);
	AssertTrue(receivedMsg->LongitudeInnerLoopGain == 1.2f);
	AssertTrue(receivedMsg->PitchAngularVelocityGain == 1.3f);
	AssertTrue(receivedMsg->RollAngularVelocityGain == 1.4f);
	AssertTrue(receivedMsg->XAntiWindupGain == 1.5f);
	AssertTrue(receivedMsg->XDerivativeGain == 1.6f);
	AssertTrue(receivedMsg->XIntegralGain == 1.7f);
	AssertTrue(receivedMsg->XProportionalGain == 1.8f);
	AssertTrue(receivedMsg->YAntiWindupGain == 1.9f);
	AssertTrue(receivedMsg->YawAntiWindupGain == 1.0f);
	AssertTrue(receivedMsg->YawDerivativeGain == 1.11f);
	AssertTrue(receivedMsg->YawIntegralGain == 1.12f);
	AssertTrue(receivedMsg->YawProportionalGain == 1.13f);
	AssertTrue(receivedMsg->YDerivativeGain == 1.14f);
	AssertTrue(receivedMsg->YIntegralGain == 1.15f);
	AssertTrue(receivedMsg->YProportionalGain == 1.16f);
	AssertTrue(receivedMsg->ZAntiWindupGain == 1.17f);
	AssertTrue(receivedMsg->ZDerivativeGain == 1.18f);
	AssertTrue(receivedMsg->ZIntegralGain == 1.19f);
	AssertTrue(receivedMsg->ZProportionalGain == 1.20f);
	

	
	delete receivedMsg;
	
	
	//Send a signal to the other software indicating that this test passed.
	GainsMessage *transmitMessage2 = new GainsMessage();
	transmitMessage2->ZProportionalGain = 12;
	
	AssertTrue2(radioInterface.transmit(transmitMessage2) == 0, 7);
	
	delete transmitMessage2;

	return 0;
}




int radiocontrollerservooutput_test(TestCase *test)
{
	DDRA |= (1<<PA5);
	
	
	SystemModel *model = new SystemModel();
	RadioControllerInterface *rcInterface = RadioControllerInterface::getRadioControllerInterface();
	
	rcInterface->SetSystemModel(model);
	
	model->LateralControl(-1);
	model->LongitudeControl(-1);
	model->MainRotorCollectiveControl(-1);
	model->YawControl(-1);
	//model->OperationalState(SystemModel::AutoPilot);
	model->OperationalState(SystemModel::ManualControl);
	
	rcInterface->init();
	rcInterface->start();
	
	
	SerialDriver *serialDriver = new SerialDriver(115200, SerialDriver::Zero, true, NULL);
	serialDriver->init();
	

//rcInterface->ScaleValue(4000);
							
	ServoControlTask *servoctask = new ServoControlTask(model, rcInterface,0,0);
	servoctask->init();
	
	/*
	float a = 0;
	float b = 0;
	float c = 0;
	rcInterface->CCPM(.42,.42,.42,a,b,c);
	*/
	
	//int counter = 0;
	
	while(true)
	{
		//I'LL WANT TO RUN THIS AGAIN WITH DIFFERENT VALUES FOR EACH SERVO TO MAKE SURE EACH INDIVIDUAL SERVO IS RECEIVING THE CORRECT VALUE. 
		/*switch(counter++)
		{
			case 0:
				model->LateralControl(0);
				model->LongitudeControl(0);
				model->MainRotorCollectiveControl(0);
				model->YawControl(0);
				break;
			case 1:
				model->LateralControl(1);
				model->LongitudeControl(1);
				model->MainRotorCollectiveControl(1);
				model->YawControl(1);
				break;
			case 2:
				model->LateralControl(-1);
				model->LongitudeControl(-1);
				model->MainRotorCollectiveControl(-1);
				model->YawControl(-1);
				counter = 0;
				break;
			default:
				counter = 0;
		}
		
		servoctask->runTaskImpl();
		
		_delay_ms(5000);	*/
		//
		//switch(counter++)
		//{
			//case 0:
			///*model->LateralControl(.5);
			//model->LongitudeControl(0);
			//model->MainRotorCollectiveControl(.75);
			//model->YawControl(.5);
			//break;*/
				//model->LateralControl(.5);
				//model->LongitudeControl(0);
				//model->MainRotorCollectiveControl(.75);
				//model->YawControl(.5);
				//break;
			//case 1:
			///*
			//model->LateralControl(0);
			//model->LongitudeControl(0);
			//model->MainRotorCollectiveControl(.75);
			//model->YawControl(.5);
			//*/
					//model->LateralControl(0);
					//model->LongitudeControl(0);
					//model->MainRotorCollectiveControl(.25);
					//model->YawControl(.5);
			//counter = 0;
			//break;
			//default:
			//counter = 0;
		//}
		





		serialDriver->transmit((byte)'T');
		
		byte bb = 0;
		
		while (bb != 'S')
		{
			serialDriver->receive(bb);
		}
		
		model->LateralControl(getFloat(serialDriver));
		model->LongitudeControl(getFloat(serialDriver));
		model->MainRotorCollectiveControl(getFloat(serialDriver));
		model->YawControl(getFloat(serialDriver));
						
						
		serialDriver->transmit((float)model->LateralControl());			
		
		
		
		serialDriver->transmit((int)OCR1A);		//ail
		serialDriver->transmit((int)OCR4C);		//ele
		serialDriver->transmit((int)OCR3C);		//pitch
		
						
						
		servoctask->runTaskImpl();

		if (model->OperationalState() == SystemModel::ManualControl)
		{
					
					
					PORTA &= ~(1<<PA5);
			//_delay_ms(500);
		//	_delay_ms(1);
		}else
		{
					PORTA |= (1<<PA5);
			//_delay_ms(10000);
			_delay_ms(2000);
		}
		
	}
	
	return 0;
}
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





int newmsgformat2_test(TestCase *test)
{
	
	
		TWIDriver *twiDriver = new TWIDriver();
		//twiDriver->init();
		MagnetometerSensor *magSensor = new MagnetometerSensor(twiDriver);
		//magSensor->init();
	
	magSensor->learn_offsets();
	
	
	
	
	
	
	
	
	
	
	
	

	//////////////////////////////////////////////////////////////////////////
	// Transmit a test message
	//////////////////////////////////////////////////////////////////////////
	Timer *t = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 200);

	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, t);
	serialDriver->init();
	
	
	GroundControlStationInterface radioInterface(serialDriver);
	
	
	SimpleTelemetryMessage *transmitMessage = new SimpleTelemetryMessage();
	transmitMessage->PitchAngularVelocityRadsPerSecond = 3.14;
	transmitMessage->PressureMillibars = 44.313;
	transmitMessage->RollAngularVelocityRadsPerSecond = 4422.1;
	transmitMessage->XAccelFrdMss = 33.33;
	transmitMessage->XEcefCm = 331.2;
	transmitMessage->XMagFrd = 22.2;
	transmitMessage->YawRads = 231.23f;
	transmitMessage->ZVelocityFRDCms = .0123f;
	
	AssertTrue2(radioInterface.transmit(transmitMessage) == 0, 1);
	
	delete transmitMessage;
	
	//////////////////////////////////////////////////////////////////////////
	// Test receiving a message
	//////////////////////////////////////////////////////////////////////////
	Message *receiveMessage = NULL;
	
	AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	
	AssertTrue2(receiveMessage->getType() == SimpleTelemetryMessage::MessageType, 4);
	
	SimpleTelemetryMessage *receivedMsg = (SimpleTelemetryMessage *)receiveMessage;
	
	
	AssertTrue(receivedMsg->PitchAngularVelocityRadsPerSecond == 3.14f);
	AssertTrue(receivedMsg->PressureMillibars == 44.313f);
	AssertTrue(receivedMsg->RollAngularVelocityRadsPerSecond == 4422.1f);
	AssertTrue(receivedMsg->XAccelFrdMss == 33.33f);
	AssertTrue(receivedMsg->XEcefCm == 331);
	AssertTrue(receivedMsg->XMagFrd == 22.2f);
	AssertTrue(receivedMsg->YawRads ==  231.23f);
	AssertTrue(receivedMsg->ZVelocityFRDCms == .0123f);

	
	delete receivedMsg;
	
	
	//Send a signal to the other software indicating that this test passed.
	SimpleTelemetryMessage *transmitMessage2 = new SimpleTelemetryMessage();
	transmitMessage2->PressureMillibars = 12;
	
	AssertTrue2(radioInterface.transmit(transmitMessage2) == 0, 7);
	
	delete transmitMessage2;

	return 0;
}





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
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, false, false, t);
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, false, true, NULL);
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
	
	//model->YawControl(3.22f);
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

/*
	int status = radioInterface.receive(receiveMessage);
	
	
	if (status == -1 || status == -2)
	{
		DDRA |= (1<<PA4);
		PORTA &= ~(1<<PA4);
	}
	AssertTrue2(status == 0, 2);
	*/
	/*
		if (status == -4)
		{
			DDRA |= (1<<PA3);
			PORTA &= ~(1<<PA3);
		}else if (status == -1 || status == -2)
		{
			DDRA |= (1<<PA4);
			PORTA &= ~(1<<PA4);
		}
		*/
		
		
	_delay_ms(300);
	
	AssertTrue2(radioInterface.receive(receiveMessage) == 0, 2);
	

	
	AssertTrue2(receiveMessage->getType() == SystemTelemetryMessage::MessageType, 3);
	
	SystemTelemetryMessage *receivedMsg = (SystemTelemetryMessage *) receiveMessage;
	
	
	
	
	SystemModel *model2 = new SystemModel();
	
	receivedMsg->updateModelFromMessage(model2);
	
	
	
	AssertTrue(model2->ChecksumErrors() == 2);
	//AssertTrue(model2->MagYawDegrees() == 2.22);
	AssertTrue(model2->Timeouts() == 2);
	AssertTrue(model2->UnrecognizedMsgTypes() == 2);
	//AssertTrue(model2->YawControl() == 3.22);
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
	if (SerialDriver::transmitBuffer.getBytesInQueue() > 0)
	{
		DDRA |= (1<<PA4);
		
		PORTA &= ~(1<<PA4);
	}	
	delete transmitMessage;
	
	_delay_ms(200);
	


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
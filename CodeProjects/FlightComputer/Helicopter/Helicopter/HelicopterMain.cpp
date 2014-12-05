/*
 * Helicopter.cpp
 *
 * Created: 2/11/2013 10:33:54 PM
 *  Author: HP User
 */ 
#include "CommonHeader.h"
#include "FlashLEDTask.h"
#include "Scheduler.h"
#include "SerialDriver.h"
#include "GroundControlStationInterface.h"
#include "SystemModel.h"
#include "SimTelemetryTask.h"
#include "TransmitTelemetryTask.h"
#include "PIDController.h"
#include "PIDOuterLoopTask.h"
#include "PIDInnerLoopTask.h"
#include "CoordinateUtil.h"
#include "RadioControllerInterface.h"
#include "ReadIMUSensorTask.h"
#include "IMUSensor.h"
#include "TWIDriver.h"
#include "NavigationTask.h"
#include "AHRS.h"
#include "ReadGPSSensorTask.h"
#include "BarometerSensor.h"
#include "ReadBarometerSensorTask.h"
#include "MagnetometerSensor.h"
#include "ReadMagnetometerSensorTask.h"
#include "ServoControlTask.h"
#include "TWIDriver.h"
#include "SystemTelemetryMessage.h"
#include "SensorDataMessage.h"
#include "PVNavigationTask.h"


#include <avr/io.h>
#include <util/delay.h>

using namespace helicopter::tasks;
using namespace helicopter::scheduler;
using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::model;
using namespace helicopter::controller;
using namespace helicopter::messages;
using namespace helicopter::util;
using namespace helicopter::sensors;
using namespace helicopter::navigation;

void setupDefaultsandReferencePosition(SystemModel *model, PIDController *pidController)
{	
	/**
	 * Set reference position of the helicopter.
	 * These are the setpoints that the helicopter to navigate/orient to.
	 * This includes the final location that the helicopter should travel to.
	 */
//	model->ReferenceMagYawRads(0.0); //point north
	model->ReferenceMagYawRads(1.52f); //point north
	//model->ReferenceMagYawRads(1.9722); //113* south east
	model->ReferenceYawVelocityRadsPerSecond(0.0);
	
	//Negative values because positive values are 'down' in NED. So we want a negative altitude setpoint.
	//model->ReferenceZNEDLocalFrameCm(-3048.0);
	//model->ReferenceZNEDLocalFrameCm(-304.8);
	model->ReferenceZNEDLocalFrameCm(-914.4f); // 30 feet.
	model->ReferenceZVelocityCms(0);
	model->ReferenceXNEDLocalFrameCm(0);
	//model->ReferenceXNEDLocalFrameCm(15000);
	
	model->ReferenceXVelocityCms(0);
	model->ReferenceYNEDLocalFrameCm(0);
	model->ReferenceYVelocityCms(0);
	
	/*latest 6/3/2014
	pidController->setYawProportionalGain(1.116);
	pidController->setYawIntegralGain(.806);
	pidController->setYawDerivativeGain(.391);
	pidController->setYawAntiWindupGain(0.135);
	*/
	
	/*
	pidController->setYawProportionalGain(.1116);
	pidController->setYawIntegralGain(.0806);
	pidController->setYawDerivativeGain(.0391);
	pidController->setYawAntiWindupGain(0.0135);
	*/
	
	
	
	/*latest 5/31/2014
	pidController->setXProportionalGain(.0151f);
	pidController->setXIntegralGain(0);
	pidController->setXDerivativeGain(.052f);
	pidController->setXAntiWindupGain(0);
	pidController->setLongitudeInnerLoopGain(1.7081);
	pidController->setPitchAngularVelocityGain(1.77509);
	
	pidController->setYProportionalGain(.015f);
	pidController->setYIntegralGain(0);
	pidController->setYDerivativeGain(.049f);
	pidController->setYAntiWindupGain(0);
	pidController->setLateralInnerLoopGain(1.73f);	
	pidController->setRollAngularVelocityGain(0.768f);
	
	pidController->setZProportionalGain(0.004f);
	pidController->setZIntegralGain(.001f);
	pidController->setZDerivativeGain(.430435);
	pidController->setZAntiWindupGain(.300874);	
	
	*/
	
	
	//old gains 10/25/2014
	/*
	pidController->setXProportionalGain(.0151f);
	pidController->setXIntegralGain(0);
	pidController->setXDerivativeGain(.052f);
	pidController->setXAntiWindupGain(0);
	pidController->setLongitudeInnerLoopGain(1.7081);
	pidController->setPitchAngularVelocityGain(.043);
	
	pidController->setYProportionalGain(.015f);
	pidController->setYIntegralGain(0);
	pidController->setYDerivativeGain(.049f);
	pidController->setYAntiWindupGain(0);
	pidController->setLateralInnerLoopGain(1.73f);
	pidController->setRollAngularVelocityGain(0.014);
	
	pidController->setZProportionalGain(0.004f);
	pidController->setZIntegralGain(.001f);
	pidController->setZDerivativeGain(.430435f);
	pidController->setZAntiWindupGain(.300874f);
	
	
	
	
	
		pidController->setYawProportionalGain(2.0);
		pidController->setYawIntegralGain(.806);
		pidController->setYawDerivativeGain(.391);
		pidController->setYawAntiWindupGain(0.135);
	*/
	
	pidController->setXProportionalGain(0.000071f);
	pidController->setXIntegralGain(0);
	pidController->setXDerivativeGain(.00025f);
	pidController->setXAntiWindupGain(0);
	pidController->setLongitudeInnerLoopGain(.272);
	pidController->setPitchAngularVelocityGain(.972);
	
	pidController->setYProportionalGain(0.000114f);
	pidController->setYIntegralGain(0);
	pidController->setYDerivativeGain(.049f);
	pidController->setYAntiWindupGain(0);
	pidController->setLateralInnerLoopGain(.43f);
	pidController->setRollAngularVelocityGain(.014f);
	
	pidController->setZProportionalGain(.0015f);
	pidController->setZIntegralGain(.00f);
	pidController->setZDerivativeGain(.22f);
	pidController->setZAntiWindupGain(.6685f);
	
	
	pidController->setYawProportionalGain(.797f);
	pidController->setYawIntegralGain(.263f);
	pidController->setYawDerivativeGain(.172);
	pidController->setYawAntiWindupGain(0.11f);
	
	
	
	
	
	
	
	//TODO: Don't forget that there is a difference between how often the sensors
	//are read and how often the control algorithm runs. 
	pidController->setIntervalPeriodSecs(PID_OUTER_LOOP_PERIOD);

/*	
	pidController->setMinYawServoControl (-.8);
	pidController->setMaxYawServoControl(.8);
	*/

//pidController->setMinYawServoControl (-10);
//pidController->setMaxYawServoControl(10);



	pidController->setControlMaxValue(1.0d);
	pidController->setControlMinValue(-1.0d);
	
	pidController->setMinYawServoControl (-.96d);
	pidController->setMaxYawServoControl (.96d);
	
	//due to bad design, if these servo control values change, so does the
	//radiocontrollerinterface method for converting control values to control signals.
	pidController->setMaxLongitudeServoControlValue(1.0d);
	pidController->setMinLongitudeServoControlValue(-1.0d);
	pidController->setMaxLateralServoControlValue(1.0d);
	pidController->setMinLateralServoControlValue(-1.0d);
	
	//just changed
	pidController->setMaxMainRotorServoControlValue(.5d);
	pidController->setMinMainRotorServoControlValue(0.0d);
	
	////5 degrees
	pidController->setMaxRollSetpointRads(0.0872664626);
	pidController->setMinRollSetpointRads(-0.0872664626);
//10 degrees
//pidController->setMaxRollSetpointRads(0.1745328);
//pidController->setMinRollSetpointRads(-0.1745328);	
	
	//13 degrees
	pidController->setMaxPitchSetpointRads(0.226892803);
	pidController->setMinPitchSetpointRads(-0.226892803);
	
	//lets try 7 degree max pitch
	//pidController->setMaxPitchSetpointRads(0.122173048);
	//pidController->setMinPitchSetpointRads(-0.122173048);
}


int main5(void)
{
	
	//AHRS *ahrs = new AHRS(1.0f/20.0f);
	
	
	//ahrs->update(-1.94171476,3.34171748,-9.517631,-0.05545767,0.3717419,-0.3282723,-0.2521546,-0.199393168,-0.9469216);
	
	
	SystemModel *model2 = new SystemModel();
	
	model2->OperationalState(SystemModel::AutoPilot);
	
	PIDController *pidController = new PIDController(model2);
	PIDOuterLoopTask *pidOuterLoop = new PIDOuterLoopTask(pidController, 3, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	PIDInnerLoopTask *pidInnerLoop = new PIDInnerLoopTask(pidController, 4, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	
	MatrixUtil::createLookupTables();
	
	pidOuterLoop->runTaskImpl();
	pidInnerLoop->runTaskImpl();
	
	
	
	
	
	
	
	
	
	
	SystemModel *model = new SystemModel();
	model->FlightMode(SystemModel::RealFlight);
	
	
	SerialDriver *serialDriver = NULL;
	unsigned long serialDriverBaudRate = 250000;
		
	serialDriver = new SerialDriver(serialDriverBaudRate, SerialDriver::Zero, false, false, NULL);
	serialDriver->init();	
	
	
	GroundControlStationInterface *gcsInterface = new GroundControlStationInterface(serialDriver);
	
	byte b = 0;
			
	while(b != 't')
	{
		serialDriver->receive(b);
	}
	
	byte t[4];
	
	int numOfRecords=0;
	
	serialDriver->receive(b);
	t[0] = b;
	serialDriver->receive(b);
	t[1] = b;
	serialDriver->receive(b);
	t[2] = b;
	serialDriver->receive(b);
	t[3] = b;
	
	memcpy(&numOfRecords,t,sizeof(int));
	
	
	
	while (true)
	{
		
		b = 0;
			
		while(b != 't')
		{
			serialDriver->receive(b);
		}
		
		


		
		
		float mag = 0;
		float accel = 0;
		
		byte temp[4];
		
		serialDriver->receive(b);
		temp[0] = b;
		serialDriver->receive(b);
		temp[1] = b;
		serialDriver->receive(b);
		temp[2] = b;
		serialDriver->receive(b);
		temp[3] = b;	
		
		memcpy(&mag,temp,sizeof(float));
		
	

		serialDriver->receive(b);
		temp[0] = b;
		serialDriver->receive(b);
		temp[1] = b;
		serialDriver->receive(b);
		temp[2] = b;
		serialDriver->receive(b);
		temp[3] = b;
		
		memcpy(&accel,temp,sizeof(float));
			
		
		if (accel > .01f)		
		{
			DDRA |= (1<<PA5);
			PORTA &= ~(1<<PA5);
			
		}

		
		AHRS *ahrs = new AHRS(1.0f/20.0f);
		ahrs->MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = mag;
		ahrs->ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = accel;
		
		
		//for (int i = 0; i < 544; i++)
		for (int i = 0; i < numOfRecords; i++)
		{
			int status = 0;

			//Use the radio interface to get the telemetry message from the simulator
			Message *message = NULL;
				
			status = gcsInterface->receive(message);
			
			SensorDataMessage *telemMsg = NULL;
			
			if (status == 0 && message != NULL)
			{

				telemMsg = (SensorDataMessage*) message;
			}
			
			if(message != NULL) 
			{
				delete message;
			}
			
			ahrs->update(telemMsg->XAccelFrdMss, telemMsg->YAccelFrdMss, telemMsg->ZAccelFrdMss,
			telemMsg->RollAngularVelocityRadsPerSecond, telemMsg->PitchAngularVelocityRadsPerSecond, telemMsg->YawAngularVelocityRadsPerSecond,
			telemMsg->XMagFrd, telemMsg->YMagFrd, telemMsg->ZMagFrd);
			
			/*model->YawRads(ahrs->getYawRads());
			model->PitchRads(ahrs->getPitchRads());
			model->RollRads(ahrs->getRollRads());
			
			SystemTelemetryMessage *message2 = SystemTelemetryMessage::buildMessageFromModel(model);
					
			//TODO add some error handling in here
			gcsInterface->transmit(message2);
			delete message2;
			*/
			
			serialDriver->transmit(ahrs->getYawRads());
			serialDriver->transmit(ahrs->getPitchRads());
			serialDriver->transmit(ahrs->getRollRads());
					
			
			
		}
		
		
		delete ahrs;
		
	}
	
	

		return 0;

}


//SystemModel *publicModel = NULL;
//static SystemModel *publicModel = new SystemModel();

int main(void)
{	
	DDRA |= (1<<PA4);
	PORTA &= ~(1<<PA4);
	
	MatrixUtil::createLookupTables();

	
	bool sendControlToServos = false;
	bool receiveGains = false;
	
	Scheduler *scheduler = Scheduler::getScheduler();
	
	SystemModel *model = new SystemModel();
	SystemModel::publicModel = model;
	//publicModel = model;
	
	//SystemModel *model = publicModel;
	
	//model->FlightMode(SystemModel::HardwareInLoopSimulatedFlight);
	model->FlightMode(SystemModel::SimulatedFlight);
	
	/**
	 * Checklist:
	 * turn off gains
	 * modify start up parameters to read gps and baro data longer before start. 
	 */
	//model->FlightMode(SystemModel::RealFlight);
	
	
	
	
	
	
	if (model->FlightMode() == SystemModel::SimulatedFlight)
	{
		model->SensorInput(SystemModel::SimulatedSensors);
		//model->SensorInput(SystemModel::RealSensors);
		
		model->CommunicationMethod(SystemModel::USB);
		//model->CommunicationMethod(SystemModel::Radio);
		
		sendControlToServos = false;
//sendControlToServos = true;
	

		
	}else if (model->FlightMode() == SystemModel::RealFlight)
	{
		model->SensorInput(SystemModel::RealSensors);
		//model->SensorInput(SystemModel::SimulatedSensors);
		
		
		model->CommunicationMethod(SystemModel::Radio);
//model->CommunicationMethod(SystemModel::USB);

		sendControlToServos = true;
		
		//receiveGains = false;
receiveGains = true;		
	}else if (model->FlightMode() == SystemModel::HardwareInLoopSimulatedFlight)
	{
		model->SensorInput(SystemModel::SimulatedSensors);
		model->CommunicationMethod(SystemModel::Radio);
//model->CommunicationMethod(SystemModel::USB);
		sendControlToServos = true;
		receiveGains = true;
	}
	
	

	
	PIDController *pidController = new PIDController(model);
	
	setupDefaultsandReferencePosition(model, pidController);
	
	//Create a driver for communicating with the Ground Control Station (GCS).
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, timer, true, true);
	//SerialDriver *serialDriver = new SerialDriver(76800, SerialDriver::Zero, true);
	SerialDriver *serialDriver = NULL;
	Timer *timer = NULL;
	
	unsigned long serialDriverBaudRate = 0;
	
		
	if (model->CommunicationMethod() == SystemModel::USB)
	{
		serialDriverBaudRate = 250000;
		
		//Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
		timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 50); //Good timeout when using the USB
		//serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, timer);//MOSTRECENT
		serialDriver = new SerialDriver(serialDriverBaudRate, SerialDriver::Zero, false, true, timer); //latest 12/3/2014
		//serialDriver = new SerialDriver(serialDriverBaudRate, SerialDriver::Zero, false, false, timer);
		//serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, NULL);
	}else if (model->CommunicationMethod() == SystemModel::Radio)
	{
		//serialDriverBaudRate = 57600;
		serialDriverBaudRate = 115200;
		//timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,75); //Good timeout when using the radio
		
		timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,200);//unfortunately for radio, the initial setup is a huge amount of data which takes a long time.
			
		//Use a slower baud rate because the real helicopter uses the radio for communication
		//which is slower than USB.
		serialDriver = new SerialDriver(serialDriverBaudRate, SerialDriver::Zero, true, true, timer);
		
		//serialDriver = new SerialDriver(serialDriverBaudRate, SerialDriver::Zero, true, false, timer);
	}
	
	serialDriver->init();
	
	
	//Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 400);//latest
	//Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100);
	
	//SerialDriver *gpsSerialDriver = new SerialDriver(38400, SerialDriver::One, false, gpsTimer);
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);//LATEST
	
	//Don't care about gps timer anymore since it's only used on initialization. 
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, NULL);
	SerialDriver *gpsSerialDriver = new SerialDriver(38400, SerialDriver::One, true, false, NULL);
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, false, gpsTimer);
	gpsSerialDriver->init();
		
		
	

	
	
	
	
	TWIDriver *twiDriver = new TWIDriver();
	twiDriver->init();

	
	
	GroundControlStationInterface *gcsInterface = new GroundControlStationInterface(serialDriver);
	
	//This frequency works well for radio.
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(radioInterface, model, 0, 10);//starting at tick 1, execute 20 times a second
//	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(radioInterface, model, 1, 10);//starting at tick 2, execute 20 times a second

	//this frequency works well for USB.
/*	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,0, 4);//starting at tick 0, execute 50 times a second
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 1, 4);//starting at tick 1, execute 50 times a second
	*/
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .02));//execute 50 hz
	
	
	//SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::ALLDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
	//SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
	//TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::ALLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));
	//TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::SIMPLEDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));

	SimTelemetryTask *simTelemTask = NULL;
	//SimTelemetryTask *gainsTelemTask = NULL;
	TransmitTelemetryTask *transTelemTask = NULL;

	if (model->FlightMode() == SystemModel::RealFlight)
	{
		//transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::SIMPLEDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));
		transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::SIMPLEDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .5));
		//transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::SIMPLEDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ));
		
		//not actually used
		//simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
		
		/*
		if (receiveGains)
		{
			gainsTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::GAINSDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ)); //execute once a sec
		}*/
		
		if (receiveGains)
		{
			simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::GAINSDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ)); //execute once a sec
		}else
		{
			simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .5));
		}
		
		
	}else if (model->FlightMode() == SystemModel::SimulatedFlight)
	{
		transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::ALLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05)); //latest 12/3/2014
		//transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::CONTROLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));
		//transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::ALLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .02));
		
		simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::ALLDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
		//simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::ALLDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .02));
		
	}else if (model->FlightMode() == SystemModel::HardwareInLoopSimulatedFlight)
	{
		transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::CONTROLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));
		simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
		
		/*if (receiveGains)
		{
			gainsTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::GAINSDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ)); //execute once a sec
		}*/
	}

	//SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
	//TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::CONTROLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));

	
/*
SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,0, SCHEDULER_TICK_FREQUENCY_HZ);//execute 1 hz
TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 0, SCHEDULER_TICK_FREQUENCY_HZ);
*/
	
	
	FlashLEDTask *flashTask = new FlashLEDTask(2, SCHEDULER_TICK_FREQUENCY_HZ);//starting at tick 2, execute once a second
	//flashTask->init();
		
//	SensorProcessingTask *sensorProcessingTask = new SensorProcessingTask(model, 5, 4);
		
	//execute the pid outer loop at the PID_OUTER_LOOP_PERIOD rate. The division is to convert the period into ticks for the scheduler.

	PIDOuterLoopTask *pidOuterLoop = new PIDOuterLoopTask(pidController, 3, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	PIDInnerLoopTask *pidInnerLoop = new PIDInnerLoopTask(pidController, 4, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_INNER_LOOP_PERIOD));
	
	
//	PIDInnerLoopTask *pidInnerLoop = new PIDInnerLoopTask(pidController, 4, 1);
	
	float barometerSensorReadPeriod = 1/20.0f; //will be 1/50 for production (or will it? because the ahrs uses this too. //latest 12/3/2014
	//float barometerSensorReadPeriod = 1/50.0f;
	float simulatorSensorReadPeriod = barometerSensorReadPeriod;
	
	float sensorReadPeriod = 0;
		
	if (model->SensorInput() == SystemModel::SimulatedSensors)
	{
		sensorReadPeriod = simulatorSensorReadPeriod;
	}else
	{
		barometerSensorReadPeriod = 1/50.0f;
		sensorReadPeriod = GYRO_SENSOR_READ_PERIOD;
	}
	
		
	//AHRS *ahrs = new AHRS(GYRO_SENSOR_READ_PERIOD);
	//AHRS *ahrs = new AHRS(simulatorSensorReadPeriod); //for simulator angular velocity reads.
	AHRS *ahrs = new AHRS(sensorReadPeriod);
	
	
	model->Ahrs(ahrs);
	
	//NavigationTask *navTask = new NavigationTask(BAROMETER_SENSOR_READ_PERIOD, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz. //used for the real helicopter
	//NavigationTask *navTask = new NavigationTask(barometerSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz. most recent 4/9/2014 //6/14/2014 most recent
	
//	NavigationTask *navTask = new NavigationTask(simulatorSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .01)); //run at 100 hz.

	//NavigationTask *navTask = new NavigationTask(barometerSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .01)); //run at 100 hz. experimental latest working 9/30/2014
	NavigationTask *navTask = NULL;
	PVNavigationTask *pvNavTask = NULL;
	

	//navTask = new NavigationTask(barometerSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * sensorReadPeriod)); //run at 98 hz.
	navTask = new NavigationTask(barometerSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .01f)); //run at 100 hz.
	pvNavTask = new PVNavigationTask(barometerSensorReadPeriod, ahrs, model, 6, (SCHEDULER_TICK_FREQUENCY_HZ * barometerSensorReadPeriod)); //run at 50 hz



	SPIDriver *spiDriver = new SPIDriver();
	spiDriver->init();
	
	SPIDriver *baroSpiDriver = new SPIDriver(SPIDriver::SS_G);
	baroSpiDriver->init();
	
	IMUSensor *imuSensor = new IMUSensor(spiDriver);
	imuSensor->init();

	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();

	BarometerSensor *baroSensor = new BarometerSensor(baroSpiDriver);
	baroSensor->init();
	
	MagnetometerSensor *magSensor = new MagnetometerSensor(twiDriver);
	magSensor->init();
	
	/*
	_delay_ms(200);
	magSensor->init();
	_delay_ms(200);
	*/

	
	//ReadGPSSensorTask *gpsSensorTask = new ReadGPSSensorTask(model, gpsSensor, 7, SCHEDULER_TICK_FREQUENCY_HZ * .25); //run at 4 hz
	ReadGPSSensorTask *gpsSensorTask = new ReadGPSSensorTask(model, gpsSensor, 7, SCHEDULER_TICK_FREQUENCY_HZ * .1); //run at 10 hz
	//ReadIMUSensorTask *imuSensorTask = new ReadIMUSensorTask(model, imuSensor, 8,  (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz.
	ReadIMUSensorTask *imuSensorTask = new ReadIMUSensorTask(model, imuSensor, 8,  (SCHEDULER_TICK_FREQUENCY_HZ * .01)); //run at 100 hz.
	//ReadIMUSensorTask *imuSensorTask = new ReadIMUSensorTask(model, imuSensor, 8,  2); //run at 100 hz.
	ReadBarometerSensorTask *barometerSensorTask = new ReadBarometerSensorTask(model, baroSensor, 9, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz. needs to run so that at least 10 ms is between each operation. since it's a 3 step process, this is really only executes hz/3 for a complete cycle (*** this will probably cause huge timeouts since it takes like 8ms to complete.)
	//ReadMagnetometerSensorTask *magSensorTask = new ReadMagnetometerSensorTask(model, magSensor, 10, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz, although the sensor is reading at 75 hz.
	ReadMagnetometerSensorTask *magSensorTask = new ReadMagnetometerSensorTask(model, magSensor, 10, (SCHEDULER_TICK_FREQUENCY_HZ * .01)); //run at 100 hz, although the sensor is reading at 75 hz.
	

	RadioControllerInterface *rcInterface = RadioControllerInterface::getRadioControllerInterface();
	
	rcInterface->SetSystemModel(model);

	ServoControlTask *servoControlTask = new ServoControlTask(model, rcInterface, 11,  (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz.
	
	
	scheduler->addTask(flashTask);
	
	
	scheduler->addTask(gpsSensorTask);
	scheduler->addTask(imuSensorTask);
	scheduler->addTask(barometerSensorTask);
	scheduler->addTask(magSensorTask);


	if (model->SensorInput() == SystemModel::SimulatedSensors || receiveGains == true)
	//if (model->SensorInput() == SystemModel::SimulatedSensors)
	{
		//Add a task to read simulator data if setup to receive sensor data from the simulator.
		scheduler->addTask(simTelemTask);
	}
	
	/*
	if (receiveGains == true)
	{
		scheduler->addTask(gainsTelemTask);
	}
	*/
	
	
	
	scheduler->addTask(transTelemTask);
	
	scheduler->addTask(pidOuterLoop);
	
	scheduler->addTask(pidInnerLoop);
	
	
	scheduler->addTask(navTask);
	scheduler->addTask(pvNavTask);
	
	
	
	if (sendControlToServos)
	{
		scheduler->addTask(servoControlTask);
	}
	
	




	if (sendControlToServos)
	{
		//If you are sending controls to the servos, then manual control has to be enabled by default so the helicopter
		//doesn't take off by itself.
		model->OperationalState(SystemModel::ManualControl);
	
		//schedule servo control task
	}else
	{
		model->OperationalState(SystemModel::AutoPilot);
	}
	
	

	PORTA |= (1<<PA4);
	
	//Set the red led to 'on' to indicate the system is initializing. I have this down here because there is a bug which might lock up the system when the GPS initializes, so
	//we turn on the red led after that bug can take place so we know we past that point in the init sequence. 
	DDRA |= (1<<PA5);
	PORTA &= ~(1<<PA5);
		
		
		

	//Wait until we receive location data before starting the scheduler
	//TODO rework this
	bool isInitialized = false;
	
	
	/**
	 * Set initial position.
	 */
	if (model->SensorInput() == SystemModel::RealSensors)
	{		
	
		//Initialize GPS readings and position
		//while (!gpsSensor->isGpsReady() || gpsSensor->getPositionAccuracyEstimateEcefCm() > 400)
while (!gpsSensor->isGpsReady() || gpsSensor->getPositionAccuracyEstimateEcefCm() > 20000)		
		{
			//gpsSensor->processSensorSolution();
			gpsSensor->readSensorLLH();
			gpsSensor->readSensorNavStatus();
			long status = gpsSensor->readSensorSolution();
			
					/*gpsSensor->readSensorECEF();
					gpsSensor->readSensorLLH();
					//long status = gpsSensor->readSensorNavStatus();
					long status = gpsSensor->readSensorSolution();*/
			
			//Wait until new data is received from the GPS
			_delay_ms(250);
			serialDriver->transmit((byte) 'T');
					//serialDriver->transmit(gpsSensor->getLatitudeDegE7());
					serialDriver->transmit(status);
					serialDriver->transmit(gpsSensor->getLongitudeDegE7());
					serialDriver->transmit(gpsSensor->getPositionAccuracyEstimateEcefCm());
					serialDriver->transmit(gpsSensor->getPositionFixStatus());
					serialDriver->transmit(gpsSensor->getXEcefCm());
					serialDriver->transmit(gpsSensor->getYEcefCm());
					serialDriver->transmit(gpsSensor->getZEcefCm());
			
		}




		
		//Turn off interrupts so that the gps sensor doesn't auto receive gps data which would
		//mess up the manual read.
		//cli();
		
		while(gpsSensor->getLatitudeDegE7() == 0 || gpsSensor->getLongitudeDegE7() == 0)
		{
			gpsSensor->readSensorLLH();
			_delay_ms(250);
		}
	
		//turn interrupts back on.
		//sei();
	
		
		//set initial position			
		model->InitialXPositionEcef(gpsSensor->getXEcefCm());
		model->InitialYPositionEcef(gpsSensor->getYEcefCm());
		model->InitialZPositionEcef(gpsSensor->getZEcefCm());
		
		//generate ecef to ned rotation matrix based on current lat/long			
		CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(model->LatitudeDegrees(), model->LongitudeDegrees(), model->EcefToLocalNEDRotationMatrix);
		
		//Delay half a second to allow the gps to read more position information. When the interrupts were disabled, it could have
		//messed up the gps data. This gives it time to clear back up.
		_delay_ms(500);
		
		//Initialize barometer readings. The barometer needs lots of time to stabilize. 
		//for (int i = 0; i < 1000; i++)
for (int i = 0; i < 5; i++)		
		{
			//baro task is a 3 step process, so run 3 times.
			for (int i = 0; i < 3; i++)
			{
				barometerSensorTask->runTaskImpl();
				_delay_ms(BarometerSensor::ADC_PROCESSING_TIME_MS);
			}
		}
		
		//set the initial altitude based on sensor readings.
		model->InitialAltitudeCm((((pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1)/ (-6.8755856 * 0.000001)) / 3.28084) * -100);
		
		
		
		float accelGain = ahrs->ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT;
		float magGain = ahrs->MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT;
		
		ahrs->ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .1f;
		ahrs->MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .1f;
		
		
		//execute all the senor tasks a bunch of times to initialize the ahrs and nav systems.
		//for (int i = 0; i < 500; i++)
for (int i = 0; i < 5; i++)		
		{
			magSensorTask->runTaskImpl();
			gpsSensorTask->runTaskImpl();
			
			//baro task is a 3 step process, so run 3 times.
			for (int i = 0; i < 3; i++)
			{
				barometerSensorTask->runTaskImpl();
				_delay_ms(BarometerSensor::ADC_PROCESSING_TIME_MS);
			}
			
			imuSensorTask->runTaskImpl();
			
			navTask->runTaskImpl();
			pvNavTask->runTaskImpl();
			
			_delay_ms(10);
		}
		
		ahrs->ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = accelGain;
		ahrs->MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = magGain;

		isInitialized = true;
	}else
	{

		//SerialDriver *initSerialDriver = new SerialDriver(serialDriverBaudRate, SerialDriver::Zero, true, true, NULL);
		//GroundControlStationInterface *initGcsInterface = new GroundControlStationInterface(initSerialDriver);
		GroundControlStationInterface *initGcsInterface = new GroundControlStationInterface(serialDriver);
		SimTelemetryTask *initSimTelemTask = new SimTelemetryTask(initGcsInterface, model, pidController,SimTelemetryTask::ALLDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
	
		while (!isInitialized)
		{
			initSimTelemTask->runTaskImpl();
		
			if (model->LatitudeDegrees() != 0 && model->LongitudeDegrees() != 0)
			{
				isInitialized = true;
			
				model->InitialXPositionEcef(model->XEcefCm());
				model->InitialYPositionEcef(model->YEcefCm());
				model->InitialZPositionEcef(model->ZEcefCm());
			
			
				////ecefReferenceX, ecefReferenceY, ecefReferenceZ,ecefToLocalNEDRotationMatrix,
				CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(model->LatitudeDegrees(), model->LongitudeDegrees(), model->EcefToLocalNEDRotationMatrix);
		//
				//float initialXPositionEcef = 0;
				//float initialYPositionEcef = 0;
				//float initialZPositionEcef = 0;
				//CoordinateUtil::ConvertFromGeodeticToECEF(model->LatitudeDegrees(), model->LongitudeDegrees(), model->AltitudeMetersAgl(), initialXPositionEcef, initialYPositionEcef, initialZPositionEcef);
				//
				//model->InitialXPositionEcef(initialXPositionEcef);
				//model->InitialYPositionEcef(initialYPositionEcef);
				//model->InitialZPositionEcef(initialZPositionEcef);
			
				//Calculate initial altitude
				//altitude equation from: http://www.barnardmicrosystems.com/L4E_FMU_sensors.htm#Pressure
				//multiply by -1 because in NED/FRD frame, down is positive. 
				//model->InitialAltitudeMeters(((288.15/(6.5/1000.0))*(1-(pow((model->PressureMillibars()/101325.0),(6.5/1000.0)*(287.052/9.78))))) * -1);
			
				//https://www.brisbanehotairballooning.com.au/faqs/education/113-pressure-altitude-conversion.html
				model->InitialAltitudeCm((((pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1)/ (-6.8755856 * 0.000001)) / 3.28084) * -100);
			}
			_delay_ms(100);
		}
		
		delete initSimTelemTask;
		delete initGcsInterface;
	//	delete initSerialDriver;
	}
	
	
	if (isInitialized)
	{
		//turn red LED off to indicate initialized.
		PORTA |= (1<<PA5);
	}
	
	gpsSensor->start();

	scheduler->init(); //Sets up the timer registers, inits all tasks,
	
	scheduler->start();
	

	if (sendControlToServos)
	{
		rcInterface->init();
		rcInterface->start();
	}
	
	while(1)
	{

	
		//Checks to see if any tasks are ready to run an executes them.
		//goes to sleep (until the next timer tick) after processing all available tasks.
		scheduler->dispatch();
		
		/**
		 * Determine if the last processing frame was blown.
		 */
		if (scheduler->hasBlownFrame())
		{
			pidController->addBlownFrame();
		}
	}
	
	
	
	
	
	return 0;	
}
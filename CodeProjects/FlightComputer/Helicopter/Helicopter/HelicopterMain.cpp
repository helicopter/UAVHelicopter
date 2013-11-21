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
#include "ServoDriver.h"
#include "PIDOuterLoopTask.h"

#include <avr/io.h>

using namespace helicopter::tasks;
using namespace helicopter::scheduler;
using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::model;
using namespace helicopter::controller;

void setupDefaultsandReferencePosition(SystemModel *model, PIDController *pidController)
{	
	model->ReferenceMagYawDegrees(0.0);
	model->ReferenceYawVelocityDegreesPerSecond(0.0);
	
	pidController->setYawProportionalGain(3.0);
	pidController->setYawIntegralGain(.008);
	pidController->setYawDerivativeGain(.85);

	//TODO: Don't forget that there is a difference between how often the sensors
	//are read and how often the control algorithm runs. 
	pidController->setIntervalPeriodSecs(PID_OUTER_LOOP_PERIOD);
	pidController->setYawAntiWindupGain(.1);
/*	
	pidController->setMinTailRotorCollectiveControlValue (-.8);
	pidController->setMaxTailRotorCollectiveValue(.8);
	*/

pidController->setMinTailRotorCollectiveControlValue (-10);
pidController->setMaxTailRotorCollectiveValue(10);
	pidController->setControlMaxValue(1.0);
	pidController->setControlMinValue(-1.0);
}


int main(void)
{	
	SystemModel *model = new SystemModel();
	
	//TODO: This somehow needs to be a simulator servo driver
	//TODO: Setup the code so that it can be setup for a simulator or not setup for a simulator.
	ServoDriver *servoDriver = new ServoDriver();
	
	PIDController *pidController = new PIDController(model, servoDriver);
	
	setupDefaultsandReferencePosition(model, pidController);
	
	//Timer *timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,75); //Good timeout when using the radio
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 10); //Good timeout when using the USB
	
	//Create a driver for communicating with the radio.
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, timer, true, true);  
	serialDriver->initialize();
	
	GroundControlStationInterface *gcsInterface = new GroundControlStationInterface(serialDriver);
	
	//This frequency works well for radio.
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(radioInterface, model, 0, 10);//starting at tick 1, execute 20 times a second
//	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(radioInterface, model, 1, 10);//starting at tick 2, execute 20 times a second

	//this frequency works well for USB.
	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, 0, 4);//starting at tick 0, execute 50 times a second
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 1, 4);//starting at tick 1, execute 50 times a second
	
	FlashLEDTask *flashTask = new FlashLEDTask(2, SCHEDULER_TICK_FREQUENCY_HZ);//starting at tick 2, execute once a second
		
		
	//execute the pid outer loop at the PID_OUTER_LOOP_PERIOD rate. The division is to convert the period into ticks for the scheduler.
	PIDOuterLoopTask *pidOuterLoop = new PIDOuterLoopTask(pidController, 3, (SCHEDULER_TICK_FREQUENCY_HZ / (1/PID_OUTER_LOOP_PERIOD)));
	
	

	Scheduler *scheduler = Scheduler::getScheduler();
	
	scheduler->addTask(flashTask);
	
	scheduler->addTask(simTelemTask);
	
	scheduler->addTask(transTelemTask);
	
	scheduler->addTask(pidOuterLoop);
	
	scheduler->init(); //Sets up the timer registers, inits all tasks,
	
	scheduler->start();
	
	
	while(1)
	{
		//Checks to see if any tasks are ready to run an executes them.
		//goes to sleep (until the next timer tick) after processing all available tasks.
		scheduler->dispatch();
	}
	
	return 0;	
}
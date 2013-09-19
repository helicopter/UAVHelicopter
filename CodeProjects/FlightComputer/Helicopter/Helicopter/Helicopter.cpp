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

#include <avr/io.h>

using namespace helicopter::tasks;
using namespace helicopter::scheduler;
using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::model;

int main(void)
{	
	SystemModel *model = new SystemModel();
	
	
	//Timer *timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,75); //Good timeout when using the radio
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 10); //Good timeout when using the USB
	
	//Create a driver for communicating with the radio.
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, timer, true, true);  
	serialDriver->initialize();
	
	GroundControlStationInterface *gcsInterface = new GroundControlStationInterface(serialDriver);
	
	//This frequency works well for radio.
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(radioInterface, model, 0, 10);//starting at tick 1, execute 50 times a second
//	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(radioInterface, model, 1, 10);//starting at tick 2, execute 50 times a second

	//this frequency works well for USB.
	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, 0, 4);//starting at tick 0, execute 50 times a second
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 1, 4);//starting at tick 1, execute 50 times a second
	
	FlashLEDTask *flashTask = new FlashLEDTask(2, SCHEDULER_TICK_FREQUENCY_HZ);//starting at tick 2, execute once a second
		

	Scheduler *scheduler = Scheduler::getScheduler();
	
	scheduler->addTask(flashTask);
	
	scheduler->addTask(simTelemTask);
	
	scheduler->addTask(transTelemTask);
	
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
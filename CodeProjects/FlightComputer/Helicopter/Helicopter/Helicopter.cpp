/*
 * Helicopter.cpp
 *
 * Created: 2/11/2013 10:33:54 PM
 *  Author: HP User
 */ 


#include <avr/io.h>

#include "FlashLEDTask.h"
#include "Scheduler.h"
#include "commonheader.h"
#include "SerialDriver.h"
#include "RadioInterface.h"
#include "SystemModel.h"
#include "SimTelemetryTask.h"
#include "TransmitTelemetryTask.h"

using namespace helicopter::tasks::misctasks;
using namespace helicopter::tasks;
using namespace helicopter::scheduler;
using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::model;

int main(void)
{	
	SystemModel *model = new SystemModel();
	
	MessageBuilder *messageBuilder = new MessageBuilder();
	
	//Create a driver for communicating with the radio.
	SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, true, true);
	serialDriver->initialize();
	
	
	RadioInterface *radioInterface = new RadioInterface(serialDriver, messageBuilder);
	
	SimTelemetryTask *simTelemTask = new SimTelemetryTask(radioInterface, model, 0, TIMER_FREQUENCY_HZ / 4);//starting at tick 1, execute 50 times a second

	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(radioInterface, model, 1, TIMER_FREQUENCY_HZ * 2);//starting at tick 2, execute 50 times a second
	
	FlashLEDTask *flashTask = new FlashLEDTask(2, TIMER_FREQUENCY_HZ);//starting at tick 3, execute once a second
		
	
	
	Scheduler *scheduler = Scheduler::getScheduler();
	
	scheduler->addTask(flashTask);
	
//	scheduler->addTask(simTelemTask);
	
	scheduler->addTask(transTelemTask);
	
	scheduler->init(); //Sets up the timer registers, inits all tasks,
	
	scheduler->start();
	
	
	while(1)
	{
		scheduler->dispatch(); //ensure it includes processor sleeping.
	}
	
	return 0;	
}
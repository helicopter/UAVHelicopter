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
#include "SensorProcessingTask.h"
#include "CoordinateUtil.h"
#include "RadioControllerInterface.h"
#include "ReadIMUSensorTask.h"
#include "IMUSensor.h"
#include "TWIDriver.h"
#include "NavigationTask.h"
#include "AHRS.h"

#include <avr/io.h>
#include <util/delay.h>

using namespace helicopter::tasks;
using namespace helicopter::scheduler;
using namespace helicopter::drivers;
using namespace helicopter::interfaces;
using namespace helicopter::model;
using namespace helicopter::controller;
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
	model->ReferenceMagYawRads(0.0); //point north
	model->ReferenceYawVelocityRadsPerSecond(0.0);
	
	//Negative values because positive values are 'down' in NED. So we want a negative altitude setpoint.
	model->ReferenceZNEDLocalFrameMeters(-30.48);
	model->ReferenceZVelocityMetersPerSecond(0);
	model->ReferenceXNEDLocalFrame(0);
	model->ReferenceXVelocityMetersPerSecond(0);
	model->ReferenceYNEDLocalFrame(0);
	model->ReferenceYVelocityMetersPerSecond(0);
	
	
	pidController->setYawProportionalGain(3.0);
	pidController->setYawIntegralGain(.008);
	pidController->setYawDerivativeGain(.85);
	pidController->setYawAntiWindupGain(.1);
	
	pidController->setXProportionalGain(.289);
	pidController->setXIntegralGain(0);
	pidController->setXDerivativeGain(1.859);
	pidController->setXAntiWindupGain(0);
	pidController->setLongitudeInnerLoopGain(.031);
	pidController->setPitchAngularVelocityGain(.03);
	
	pidController->setYProportionalGain(1.437);
	pidController->setYIntegralGain(0);
	pidController->setYDerivativeGain(2.8363);
	pidController->setYAntiWindupGain(0);
	pidController->setLateralInnerLoopGain(.092);	
	pidController->setRollAngularVelocityGain(.048);
	
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
	
	pidController->setMaxLongitudeServoControlValue(1.0d);
	pidController->setMinLongitudeServoControlValue(-1.0d);
	
	pidController->setMaxLateralServoControlValue(1.0d);
	pidController->setMinLateralServoControlValue(-1.0d);
	
	//just changed
	pidController->setMaxMainRotorServoControlValue(.5d);
	pidController->setMinMainRotorServoControlValue(0.0d);
	
	//5 degrees
	pidController->setMaxRollSetpointRads(0.0872664626);
	pidController->setMinRollSetpointRads(-0.0872664626);
	
	pidController->setMaxPitchSetpointRads(0.226892803);
	pidController->setMinPitchSetpointRads(-0.226892803);
}


int main(void)
{	
	SystemModel *model = new SystemModel();
	
	PIDController *pidController = new PIDController(model);
	
	setupDefaultsandReferencePosition(model, pidController);
	
	//Timer *timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,75); //Good timeout when using the radio
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	
	//Create a driver for communicating with the Ground Control Station (GCS).
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, timer, true, true);  
	//SerialDriver *serialDriver = new SerialDriver(76800, SerialDriver::Zero, true);  
	SerialDriver *serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, timer);  
	serialDriver->init();
	
	GroundControlStationInterface *gcsInterface = new GroundControlStationInterface(serialDriver);
	
	//This frequency works well for radio.
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(radioInterface, model, 0, 10);//starting at tick 1, execute 20 times a second
//	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(radioInterface, model, 1, 10);//starting at tick 2, execute 20 times a second

	//this frequency works well for USB.
/*	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,0, 4);//starting at tick 0, execute 50 times a second
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 1, 4);//starting at tick 1, execute 50 times a second
	*/
	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));
	
	
	
	FlashLEDTask *flashTask = new FlashLEDTask(2, SCHEDULER_TICK_FREQUENCY_HZ);//starting at tick 2, execute once a second
		
		
//	SensorProcessingTask *sensorProcessingTask = new SensorProcessingTask(model, 5, 4);
		
	//execute the pid outer loop at the PID_OUTER_LOOP_PERIOD rate. The division is to convert the period into ticks for the scheduler.
	PIDOuterLoopTask *pidOuterLoop = new PIDOuterLoopTask(pidController, 3, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	PIDInnerLoopTask *pidInnerLoop = new PIDInnerLoopTask(pidController, 4, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	
	
	//AHRS *ahrs = new AHRS(GYRO_SENSOR_READ_PERIOD);
	AHRS *ahrs = new AHRS(1/20.0f); //for simulator angular velocity reads.
	
//	NavigationTask *navTask = new NavigationTask(ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz.

	//it looks like navtask really needs to run at 200hz.
	NavigationTask *navTask = new NavigationTask(ahrs, model, 0, 1); //Run at SCHEDULER_TICK_FREQUENCY_HZ hz. (in this case, 200 hz).
	
	
	
	
	SPIDriver *spiDriver = new SPIDriver();
	spiDriver->init();
	
	IMUSensor *imuSensor = new IMUSensor(spiDriver);
	imuSensor->init();

	RadioControllerInterface *rcInterface = RadioControllerInterface::getRadioControllerInterface();
	
	rcInterface->SetSystemModel(model);


	Scheduler *scheduler = Scheduler::getScheduler();
	
	scheduler->addTask(flashTask);
	
	scheduler->addTask(simTelemTask);
	
	scheduler->addTask(transTelemTask);
	
	scheduler->addTask(pidOuterLoop);
	
	scheduler->addTask(pidInnerLoop);
	
	scheduler->addTask(navTask);
	
//	scheduler->addTask(sensorProcessingTask); //not used since nav task creates localNED stuff.
	

	
//	scheduler->addTask(imuSensorTask);
	

	//Wait until we receive location data before starting the scheduler
	//TODO rework this
	bool isInitialized = false;
	
	while (!isInitialized)
	{
		simTelemTask->runTaskImpl();
		
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
			model->InitialAltitudeMeters(((pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1)/ (-6.8755856 * 0.000001)) / 3.28084);
		}
		_delay_ms(100);
	}
	
	
	
	
	
	
	//TODO set this to manual by default and have some sort of flag for what build we are making to know if it should be autopilot or not.
	model->OperationalState(SystemModel::AutoPilot);
	
	scheduler->init(); //Sets up the timer registers, inits all tasks,
	
	scheduler->start();
	
	rcInterface->init();
	
	rcInterface->start();
	
	
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
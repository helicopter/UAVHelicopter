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
#include "ReadGPSSensorTask.h"
#include "BarometerSensor.h"
#include "ReadBarometerSensorTask.h"
#include "MagnetometerSensor.h"
#include "ReadMagnetometerSensorTask.h"

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
	//model->ReferenceMagYawRads(0.0); //point north
	model->ReferenceMagYawRads(1.9722); //113* south east
	model->ReferenceYawVelocityRadsPerSecond(0.0);
	
	//Negative values because positive values are 'down' in NED. So we want a negative altitude setpoint.
	//model->ReferenceZNEDLocalFrameCm(-3048.0);
	model->ReferenceZNEDLocalFrameCm(-304.8);
	model->ReferenceZVelocityCms(0);
	model->ReferenceXNEDLocalFrameCm(0);
	//model->ReferenceXNEDLocalFrameCm(15000);
	
	model->ReferenceXVelocityCms(0);
	model->ReferenceYNEDLocalFrameCm(0);
	model->ReferenceYVelocityCms(0);
	
	
	pidController->setYawProportionalGain(1.116);
	pidController->setYawIntegralGain(.806);
	pidController->setYawDerivativeGain(.391);
	pidController->setYawAntiWindupGain(0.135);
	
	pidController->setXProportionalGain(.00019);
	pidController->setXIntegralGain(0);
	pidController->setXDerivativeGain(.00153);
	pidController->setXAntiWindupGain(0);
	pidController->setLongitudeInnerLoopGain(1.7081);
	pidController->setPitchAngularVelocityGain(1.77509);
	
	pidController->setYProportionalGain(.04577);
	pidController->setYIntegralGain(0);
	pidController->setYDerivativeGain(.13043);
	pidController->setYAntiWindupGain(0);
	pidController->setLateralInnerLoopGain(.36102);	
	pidController->setRollAngularVelocityGain(.04348);
	
	pidController->setZProportionalGain(0.00445);
	pidController->setZIntegralGain(.000874);
	pidController->setZDerivativeGain(.430435);
	pidController->setZAntiWindupGain(.000874);	
	
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
	
	//lets try 7 degree max pitch
	//pidController->setMaxPitchSetpointRads(0.122173048);
	//pidController->setMinPitchSetpointRads(-0.122173048);
}


int main(void)
{	
	SystemModel *model = new SystemModel();
	
	PIDController *pidController = new PIDController(model);
	
	setupDefaultsandReferencePosition(model, pidController);
	
	//Timer *timer = new Timer(F_CPU,PRESCALE_BY_TENTWENTYFOUR,75); //Good timeout when using the radio
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB
	
	//Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 250);
	Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100);
	
	//SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);
	gpsSerialDriver->init();
		
		
	
	//Create a driver for communicating with the Ground Control Station (GCS).
	//SerialDriver *serialDriver = new SerialDriver(57600, SerialDriver::Zero, timer, true, true);  
	//SerialDriver *serialDriver = new SerialDriver(76800, SerialDriver::Zero, true);  
	SerialDriver *serialDriver = new SerialDriver(250000, SerialDriver::Zero, true, timer);  
	serialDriver->init();
	
	TWIDriver *twiDriver = new TWIDriver();
	
	
	GroundControlStationInterface *gcsInterface = new GroundControlStationInterface(serialDriver);
	
	//This frequency works well for radio.
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(radioInterface, model, 0, 10);//starting at tick 1, execute 20 times a second
//	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(radioInterface, model, 1, 10);//starting at tick 2, execute 20 times a second

	//this frequency works well for USB.
/*	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,0, 4);//starting at tick 0, execute 50 times a second
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 1, 4);//starting at tick 1, execute 50 times a second
	*/


	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::ALLDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
//	SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::SENSORDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .02));//execute 50 hz
	TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, TransmitTelemetryTask::ALLDATA, 1, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));
	
/*
SimTelemetryTask *simTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,0, SCHEDULER_TICK_FREQUENCY_HZ);//execute 1 hz
TransmitTelemetryTask *transTelemTask = new TransmitTelemetryTask(gcsInterface, model, 0, SCHEDULER_TICK_FREQUENCY_HZ);
*/
	
	
	FlashLEDTask *flashTask = new FlashLEDTask(2, SCHEDULER_TICK_FREQUENCY_HZ);//starting at tick 2, execute once a second
		
		
//	SensorProcessingTask *sensorProcessingTask = new SensorProcessingTask(model, 5, 4);
		
	//execute the pid outer loop at the PID_OUTER_LOOP_PERIOD rate. The division is to convert the period into ticks for the scheduler.

	PIDOuterLoopTask *pidOuterLoop = new PIDOuterLoopTask(pidController, 3, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	PIDInnerLoopTask *pidInnerLoop = new PIDInnerLoopTask(pidController, 4, (SCHEDULER_TICK_FREQUENCY_HZ  * PID_OUTER_LOOP_PERIOD));
	
	
//	PIDInnerLoopTask *pidInnerLoop = new PIDInnerLoopTask(pidController, 4, 1);
	
	float simulatorSensorReadPeriod = 1/20.0f;
	//float simulatorSensorReadPeriod = 1/50.0f;
		
	//AHRS *ahrs = new AHRS(GYRO_SENSOR_READ_PERIOD);
	AHRS *ahrs = new AHRS(simulatorSensorReadPeriod); //for simulator angular velocity reads.
	
	//NavigationTask *navTask = new NavigationTask(BAROMETER_SENSOR_READ_PERIOD, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz. //used for the real helicopter
	NavigationTask *navTask = new NavigationTask(simulatorSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz. most recent 4/9/2014
//	NavigationTask *navTask = new NavigationTask(simulatorSensorReadPeriod, ahrs, model, 5, (SCHEDULER_TICK_FREQUENCY_HZ * .01)); //run at 100 hz.

	
	
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
	
	
	//ReadGPSSensorTask *gpsSensorTask = new ReadGPSSensorTask(model, gpsSensor, 7, SCHEDULER_TICK_FREQUENCY_HZ * .25); //run at 4 hz
	ReadGPSSensorTask *gpsSensorTask = new ReadGPSSensorTask(model, gpsSensor, 7, SCHEDULER_TICK_FREQUENCY_HZ * .1); //run at 10 hz
	ReadIMUSensorTask *imuSensorTask = new ReadIMUSensorTask(model, imuSensor, 8,  (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz.
	ReadBarometerSensorTask *barometerSensorTask = new ReadBarometerSensorTask(model, baroSensor, 9, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz. (*** this will probably cause huge timeouts since it takes like 8ms to complete.
	ReadMagnetometerSensorTask *magSensorTask = new ReadMagnetometerSensorTask(model, magSensor, 10, (SCHEDULER_TICK_FREQUENCY_HZ * .02)); //run at 50 hz, although the sensor is reading at 75 hz.
	

	RadioControllerInterface *rcInterface = RadioControllerInterface::getRadioControllerInterface();
	
	rcInterface->SetSystemModel(model);


	Scheduler *scheduler = Scheduler::getScheduler();
	
	
	scheduler->addTask(gpsSensorTask);
	scheduler->addTask(imuSensorTask);
	//scheduler->addTask(barometerSensorTask);
	scheduler->addTask(magSensorTask);
	
	
	
	scheduler->addTask(flashTask);
	
//	scheduler->addTask(simTelemTask);
	
	scheduler->addTask(transTelemTask);
	
	scheduler->addTask(pidOuterLoop);
	
	scheduler->addTask(pidInnerLoop);
	
	scheduler->addTask(navTask);
	
//	scheduler->addTask(sensorProcessingTask); //not used since nav task creates localNED stuff.
	

	
//	scheduler->addTask(imuSensorTask);
	

	//Wait until we receive location data before starting the scheduler
	//TODO rework this
	bool isInitialized = false;
	
	SimTelemetryTask *initSimTelemTask = new SimTelemetryTask(gcsInterface, model, pidController,SimTelemetryTask::ALLDATA, 0, (SCHEDULER_TICK_FREQUENCY_HZ  * .05));//execute 20 hz
	
	
	
/*	
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
*/	
	
	
	
	
	
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
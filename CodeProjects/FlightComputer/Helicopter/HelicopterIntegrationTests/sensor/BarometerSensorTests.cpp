
#include <math.h>
#include <util/delay.h>

#include "BarometerSensorTests.h"
#include "UnitTestUtils.h"
#include "ImuSensorTests.h"
#include "SPIDriver.h"
#include "IMUSensor.h"
#include "Timer.h"
#include "SerialDriver.h"
#include "BarometerSensor.h"
#include "SystemModel.h"
#include "ReadBarometerSensorTask.h"
#include "AHRS.h"
#include "NavigationTask.h"

using namespace helicopter::drivers;
using namespace helicopter::sensors;
using namespace helicopter::util;

int readbaro_test(TestCase *test)
{
	Timer *timer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 100); //Good timeout when using the USB

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true, timer);
	serialDriver->init();

	SPIDriver *driver = new SPIDriver(SPIDriver::SS_G);
	driver->init();
	
	BarometerSensor *baroSensor = new BarometerSensor(driver);
	baroSensor->init();
	
	SystemModel *model = new SystemModel();
	model->SensorInput(SystemModel::RealSensors);
	
	ReadBarometerSensorTask *task = new ReadBarometerSensorTask(model,baroSensor,10, 10);
	
	AHRS *ahrs = new AHRS(.025);
	NavigationTask *navTask = new NavigationTask(.025, ahrs, model, 10, 10);
	
	AssertTrue(baroSensor->getRawTemperature() == 0);
	AssertTrue(baroSensor->getRawPressure() == 0);
	AssertTrue(baroSensor->getTemperatureCelcius() == 0);
	AssertTrue(baroSensor->getPressureMillibars() == 0);

	
	baroSensor->readSensor();
	
	AssertTrue(baroSensor->getRawTemperature() != 0);
	AssertTrue(baroSensor->getRawPressure() != 0);
	AssertTrue(baroSensor->getTemperatureCelcius() != 0);
	AssertTrue(baroSensor->getPressureMillibars() != 0);
	
	
	int counter = 0;
	bool isReady = false;
	bool notInitialized = true;
	while (true)
	{
		_delay_ms(50);
		baroSensor->readSensor();
		
		serialDriver->transmit('S');

		serialDriver->transmit(baroSensor->getRawTemperature());
		serialDriver->transmit(baroSensor->getRawPressure());		
		serialDriver->transmit(baroSensor->getTemperatureCelcius());
		serialDriver->transmit(baroSensor->getPressureMillibars());
		
		_delay_ms(100);
		
		for (int i = 0; i < 3; i++)
		{
			task->runTaskImpl();
			_delay_ms(BarometerSensor::ADC_PROCESSING_TIME_MS);
		}
		serialDriver->transmit(model->PressureMillibars());
		
		if (counter > 20)
		{
			isReady = true;
		}else
		{
			counter++;
		}
		
		if (isReady && notInitialized)
		{
			model->InitialAltitudeCm((((pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1)/ (-6.8755856 * 0.000001)) / 3.28084) * -100);
			notInitialized = false;
		}
		navTask->runTaskImpl();
		serialDriver->transmit(model->ZNEDLocalFrameCm());
	}
	
	return 0;
}



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
	
	AssertTrue(baroSensor->getRawTemperature() == 0);
	AssertTrue(baroSensor->getRawPressure() == 0);
	AssertTrue(baroSensor->getTemperatureCelcius() == 0);
	AssertTrue(baroSensor->getPressureMillibars() == 0);

	
	baroSensor->readSensor();
	
	AssertTrue(baroSensor->getRawTemperature() != 0);
	AssertTrue(baroSensor->getRawPressure() != 0);
	AssertTrue(baroSensor->getTemperatureCelcius() != 0);
	AssertTrue(baroSensor->getPressureMillibars() != 0);
	
	while (true)
	{
		_delay_ms(50);
		baroSensor->readSensor();
		
		serialDriver->transmit('S');

		serialDriver->transmit(baroSensor->getRawTemperature());
		serialDriver->transmit(baroSensor->getRawPressure());		
		serialDriver->transmit(baroSensor->getTemperatureCelcius());
		serialDriver->transmit(baroSensor->getPressureMillibars());
		
	}
	
	return 0;
}


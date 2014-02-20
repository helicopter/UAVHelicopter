#include "BarometerSensorTests.h"


#include <math.h>
#include <util/delay.h>

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

	SerialDriver *serialDriver = new SerialDriver(9600, SerialDriver::Zero, true);
	serialDriver->initialize();

	SPIDriver *driver = new SPIDriver(SPIDriver::SS_G);
	
	BarometerSensor *baroSensor = new BarometerSensor(driver);
	
	baroSensor->init();
	
	AssertTrue(baroSensor->getRawTemperature() == 0);
	AssertTrue(baroSensor->getRawPressure() == 0);
	AssertTrue(baroSensor->getTemperature() == 0);
	AssertTrue(baroSensor->getPressure() == 0);

	
	baroSensor->readSensor();
	
	AssertTrue(baroSensor->getRawTemperature() != 0);
	AssertTrue(baroSensor->getRawPressure() != 0);
	AssertTrue(baroSensor->getTemperature() != 0);
	AssertTrue(baroSensor->getPressure() != 0);
	
	AssertTrue(baroSensor->getRawTemperature() != baroSensor->getTemperature());
	
	while (true)
	{
		_delay_ms(700);
		baroSensor->readSensor();
		
		serialDriver->transmit('S',timer);
		
		serialDriver->transmit(baroSensor->getTemperature(), timer);
		serialDriver->transmit(baroSensor->getPressure(), timer);
	}
	
	return 0;
}


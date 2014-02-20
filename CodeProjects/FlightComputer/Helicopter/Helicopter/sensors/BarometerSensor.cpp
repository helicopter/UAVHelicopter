#include <util/delay.h>

#include "BarometerSensor.h"


using namespace helicopter::sensors;

void BarometerSensor::init()
{
	
	//Per page 9 of MS5611-01Ba03.PDF Reset the barometer to ensure calibration PROM gets loaded.
	spiDriver->transactionWrite(RESET_COMMAND);
	
	_delay_ms(RESET_PROCESSING_TIME_MS);
	
	spiDriver->beginTransaction();
	
	
	/**
	 * Each barometer is individually calibrated at the factory. This reads those calibration
	 * values for our calculations to compensate temperature and pressure values. 
	 * Those values are stored in the programmable read only memory (PROM) of the barometer
	 */
	spiDriver->write(READ_PROM_C1_COMMAND);
	pressureSensitivity_SENSt1_C1 = spiDriver->readInt();
	
	spiDriver->write(READ_PROM_C2_COMMAND);
	pressureSensitivityOffset_OFFt1_C2 = spiDriver->readInt();
	
	spiDriver->write(READ_PROM_C3_COMMAND);
	temperatureCoefficientOfPressureSensitivity_TCS_C3 = spiDriver->readInt();
	
	spiDriver->write(READ_PROM_C4_COMMAND);
	temperatureCoefficientOfPressureOffset_TCO_C4 = spiDriver->readInt();
	
	spiDriver->write(READ_PROM_C5_COMMAND);
	referenceTemperature_Tref_C5 = spiDriver->readInt();
	
	spiDriver->write(READ_PROM_C6_COMMAND);
	temperatureCoefficientOfTheTemperature_TEMPSENS_C6 = spiDriver->readInt();
	
	spiDriver->endTransaction();
}

void BarometerSensor::readSensor()
{
	/**
	 * According to page 5 in MS5611-01Ba03.PDF, the best noise performance is obtained
	 * when the SPI bus is idle during ADC conversion. So we keep the ADC initiation command
	 * in it's own transaction to reduce noise. 
	 */
	
	//Initiate conversion for pressure.
	spiDriver->transactionWrite(CONVERT_D1_PRESSURE_OSR_4096);
	
	//According to the documentation, delay for 8.22 ms while it processes. 
	_delay_ms(ADC_PROCESSING_TIME_MS);
	
	spiDriver->beginTransaction();
	
	/**
	 * Read the ADC results
	 */
	byte highByte = spiDriver->readByte();
	byte midByte = spiDriver->readByte();
	byte lowByte = spiDriver->readByte();
	
	rawPressure = (highByte << 16) | (midByte << 8) | lowByte;
	
	//TODO: DISPLAY SIZEOF(UINT32) TO CHECK that it's actually 32 bytes.
	//TODO: DANGER - IF IT IS 32 BIT, HOW CAN I REPRESENT THAT AS A FLOAT?
	
	spiDriver->endTransaction();
	
	
	//Initiate conversion for temperature.
//	spiDriver->transactionWrite(CONVERT_D1_PRESSURE_OSR_4096);	
}

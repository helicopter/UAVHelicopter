#include <util/delay.h>
#include "BarometerSensor.h"


using namespace helicopter::sensors;

void BarometerSensor::init()
{
	//Per page 9 of MS5611-01Ba03.PDF Reset the barometer to ensure calibration PROM gets loaded.
	spiDriver->transactionWrite(RESET_COMMAND);

	_delay_ms(RESET_PROCESSING_TIME_MS);
	
	/**
	 * Each barometer is individually calibrated at the factory. This reads those calibration
	 * values for our calculations to compensate temperature and pressure values. 
	 * Those values are stored in the programmable read only memory (PROM) of the barometer
	 */
	spiDriver->beginTransaction();
	spiDriver->write(READ_PROM_C1_COMMAND);
	pressureSensitivity_SENSt1_C1 = spiDriver->readUInt();
	spiDriver->endTransaction();
	
	spiDriver->beginTransaction();
	spiDriver->write(READ_PROM_C2_COMMAND);
	pressureSensitivityOffset_OFFt1_C2 = spiDriver->readUInt();
	spiDriver->endTransaction();

	spiDriver->beginTransaction();
	spiDriver->write(READ_PROM_C3_COMMAND);
	temperatureCoefficientOfPressureSensitivity_TCS_C3 = spiDriver->readUInt();
	spiDriver->endTransaction();
	
	spiDriver->beginTransaction();
	spiDriver->write(READ_PROM_C4_COMMAND);
	temperatureCoefficientOfPressureOffset_TCO_C4 = spiDriver->readUInt();
	spiDriver->endTransaction();
	
	spiDriver->beginTransaction();
	spiDriver->write(READ_PROM_C5_COMMAND);
	referenceTemperature_Tref_C5 = spiDriver->readUInt();
	spiDriver->endTransaction();
	
	spiDriver->beginTransaction();
	spiDriver->write(READ_PROM_C6_COMMAND);
	temperatureCoefficientOfTheTemperature_TEMPSENS_C6 = spiDriver->readUInt();
	spiDriver->endTransaction();
}




void BarometerSensor::sendD1Command()
{
	/**
	 * Initiate conversion for pressure.
	 * According to page 5 in MS5611-01Ba03.PDF, the best noise performance is obtained
	 * when the SPI bus is idle during ADC conversion. So we keep the ADC initiation command
	 * in it's own transaction to reduce noise. 
	 */
	spiDriver->transactionWrite(CONVERT_D1_PRESSURE_OSR_4096);
}

void BarometerSensor::readD1Data()
{	
	spiDriver->beginTransaction();
	
	/**
	 * Read the ADC results
	 */
	spiDriver->write(ADC_READ_COMMAND);
	
	/**
	 * The pressure is 3 bytes. So read the hight byte, shift it by eight, read the mid byte, shift by 8, then read the low byte
	 * to form a 3 byte 'long'
	 */
	rawPressure = spiDriver->readByte();
	rawPressure = rawPressure << 8 | spiDriver->readByte();
	rawPressure = rawPressure << 8 | spiDriver->readByte();
	
	spiDriver->endTransaction();	
}

void BarometerSensor::sendD2Command()
{
	//Initiate conversion for temperature.
	spiDriver->transactionWrite(CONVERT_D2_TEMPERATURE_OSR_4096);	
}

void BarometerSensor::readD2Data()
{
	spiDriver->beginTransaction();
	
	/**
	 * Read the ADC results
	 */
	spiDriver->write(ADC_READ_COMMAND);
	rawTemperature = spiDriver->readByte();
	rawTemperature = rawTemperature << 8 | spiDriver->readByte();
	rawTemperature = rawTemperature << 8 | spiDriver->readByte();	

	spiDriver->endTransaction();	
}


void BarometerSensor::processBaroData()
{
	
	/**
	* Formulas from page 7 on MS5611-01BA03.pdf
	*/
	int64_t rawPress = rawPressure;
	int64_t rawTemp = rawTemperature;

	//I use the shift operators instead of division. (I.e. << 8 = dividing by 2^8).
	int64_t tempDifference = rawTemp - (referenceTemperature_Tref_C5 << 8);
	int64_t temp = 2000 + ((tempDifference * temperatureCoefficientOfTheTemperature_TEMPSENS_C6) >> 23);
	int64_t tempOffset = (pressureSensitivityOffset_OFFt1_C2 << 16) + ((temperatureCoefficientOfPressureOffset_TCO_C4 * tempDifference) >> 7);
	int64_t sensitivityAtTemp = (pressureSensitivity_SENSt1_C1 << 15) + ((temperatureCoefficientOfPressureSensitivity_TCS_C3 * tempDifference) >> 8);
		
	/**
	 * The sensor's accuracy drops off when the temp is below 20*C. So the following is
	 * code to compensate for temperature below 20*C. 
	 */
	if (temp < 2000)
	{
		int64_t temp2 = (tempDifference * tempDifference) >> 31;
		
		int64_t temporaryValue = (temp - 2000);
		temporaryValue *= temporaryValue;
		
		int64_t tempOffset2 = 5 * (temporaryValue >> 1);
		int64_t sensitivity2 = 5 * (temporaryValue >> 2);	
							
		//Temperature correction code for below 15*c
		if (temp < -1500)
		{
			int64_t temporaryValue2 = temp + 1500;
			temporaryValue2 *= temporaryValue2;
			
			tempOffset2 = tempOffset2 + 7 * temporaryValue2;
			sensitivity2 = sensitivity2 + 11 * (temporaryValue2 >> 1);
		}
		
				
		temp = temp - temp2;
		tempOffset = tempOffset - tempOffset2;
		sensitivityAtTemp = sensitivityAtTemp - sensitivity2;
	}
	
	pressureMillibars = (((((rawPress * sensitivityAtTemp) >> 21) - tempOffset)) >> 15);
	temperatureCelcius = temp;
}


void BarometerSensor::readSensor()
{
	/**
	 * Initiate conversion for pressure.
	 * According to page 5 in MS5611-01Ba03.PDF, the best noise performance is obtained
	 * when the SPI bus is idle during ADC conversion. So we keep the ADC initiation command
	 * in it's own transaction to reduce noise. 
	 */
	spiDriver->transactionWrite(CONVERT_D1_PRESSURE_OSR_4096);
	
	//According to documentation, delay while the ADC processes.
	_delay_ms(ADC_PROCESSING_TIME_MS);
	
	spiDriver->beginTransaction();
	
	/**
	 * Read the ADC results
	 */
	spiDriver->write(ADC_READ_COMMAND);
	
	/**
	 * The pressure is 3 bytes. So read the hight byte, shift it by eight, read the mid byte, shift by 8, then read the low byte
	 * to form a 3 byte 'long'
	 */
	rawPressure = spiDriver->readByte();
	rawPressure = rawPressure << 8 | spiDriver->readByte();
	rawPressure = rawPressure << 8 | spiDriver->readByte();
	
	spiDriver->endTransaction();
	
	
	//Initiate conversion for temperature.
	spiDriver->transactionWrite(CONVERT_D2_TEMPERATURE_OSR_4096);	
	
	_delay_ms(ADC_PROCESSING_TIME_MS);
	
	spiDriver->beginTransaction();
	
	/**
	 * Read the ADC results
	 */
	spiDriver->write(ADC_READ_COMMAND);
	rawTemperature = spiDriver->readByte();
	rawTemperature = rawTemperature << 8 | spiDriver->readByte();
	rawTemperature = rawTemperature << 8 | spiDriver->readByte();	

	spiDriver->endTransaction();
	
	/**
	* Formulas from page 7 on MS5611-01BA03.pdf
	*/
	int64_t rawPress = rawPressure;
	int64_t rawTemp = rawTemperature;

	//I use the shift operators instead of division. (I.e. << 8 = dividing by 2^8).
	int64_t tempDifference = rawTemp - (referenceTemperature_Tref_C5 << 8);
	int64_t temp = 2000 + ((tempDifference * temperatureCoefficientOfTheTemperature_TEMPSENS_C6) >> 23);
	int64_t tempOffset = (pressureSensitivityOffset_OFFt1_C2 << 16) + ((temperatureCoefficientOfPressureOffset_TCO_C4 * tempDifference) >> 7);
	int64_t sensitivityAtTemp = (pressureSensitivity_SENSt1_C1 << 15) + ((temperatureCoefficientOfPressureSensitivity_TCS_C3 * tempDifference) >> 8);
		
	/**
	 * The sensor's accuracy drops off when the temp is below 20*C. So the following is
	 * code to compensate for temperature below 20*C. 
	 */
	if (temp < 2000)
	{
		int64_t temp2 = (tempDifference * tempDifference) >> 31;
		
		int64_t temporaryValue = (temp - 2000);
		temporaryValue *= temporaryValue;
		
		int64_t tempOffset2 = 5 * (temporaryValue >> 1);
		int64_t sensitivity2 = 5 * (temporaryValue >> 2);	
							
		//Temperature correction code for below 15*c
		if (temp < -1500)
		{
			int64_t temporaryValue2 = temp + 1500;
			temporaryValue2 *= temporaryValue2;
			
			tempOffset2 = tempOffset2 + 7 * temporaryValue2;
			sensitivity2 = sensitivity2 + 11 * (temporaryValue2 >> 1);
		}
		
				
		temp = temp - temp2;
		tempOffset = tempOffset - tempOffset2;
		sensitivityAtTemp = sensitivityAtTemp - sensitivity2;
	}
	
	pressureMillibars = (((((rawPress * sensitivityAtTemp) >> 21) - tempOffset)) >> 15);
	temperatureCelcius = temp;
}

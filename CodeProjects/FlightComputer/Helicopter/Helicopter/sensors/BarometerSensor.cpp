#include <util/delay.h>
#include <avr/io.h>
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



















//FLVS
void beginTransmission()
{
	//	PORTB &= ~(1<<DDB0);
	PORTG &= ~(1<<DDG1);
}
void endTransmission()
{
	//	PORTB |= (1<<DDB0);
	PORTG |= (1<<DDG1);
}


int SPI_transfer(int mem_rw) {
	////send address and write command
	

	
	SPDR = mem_rw;
	
	while(!(SPSR & (1<<SPIF)))
	;
	return 0;
}
int SPI_read(int val)
{
	SPI_transfer(val);
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

int SPI_read16(uint8_t val)
{
	uint8_t dump;
	uint8_t highbyte;
	uint8_t lowbyte;
	int return_value;
	
	beginTransmission();
	dump = SPI_transfer(val);
	highbyte = SPI_read(0);
	lowbyte = SPI_read(0);

	endTransmission();
	
	return_value = (highbyte <<8) | lowbyte;
	
	return(return_value);
}

int BARO_READADC = 0X00;

uint32_t SPI_readADC()
{
	uint8_t dump;
	uint8_t highbyte;
	uint8_t midbyte;
	uint8_t lowbyte;
	uint32_t return_value = 0;
	
	beginTransmission();
	dump = SPI_transfer(BARO_READADC);
	highbyte = SPI_read(0);
	midbyte = SPI_read(0);
	lowbyte = SPI_read(0);

	endTransmission();
	
	return_value = (0x00<<8) | (highbyte );
	return_value = return_value << 8;
	return_value |= midbyte;
	return_value = return_value << 8;
	return_value |= lowbyte;
	
	//	return_value = (0x00<<24) | (highbyte <<16) | (midbyte<<8) | lowbyte;
	
	return(return_value);
}



void _SPI_write(uint8_t cmd)
{
	uint8_t dump;
	
	beginTransmission();
	
	dump = SPI_transfer(cmd);

	endTransmission();
}




int BARO_RESET = 0X1E;
int BARO_CONVERTD1 = 0X48;
int BARO_CONVERTD2 = 0X58;
int BARO_PROMREAD = 0X00;



void BarometerSensor::readSensor()
{
				//_SPI_write(BARO_CONVERTD2);
				//_delay_ms(10);
				//uint32_t rawtemp = SPI_readADC();
				//
				//_SPI_write(BARO_CONVERTD1);
				//_delay_ms(10);
				//uint32_t rawbaro = SPI_readADC();
				//
				//
				//
				//
				////rawTemperature = rawtemp;
				////rawPressure = rawbaro;
//
	 
	
	
	
	
	
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
	spiDriver->write(ADC_READ_COMMAND);
	byte highByte = spiDriver->readByte();
	byte midByte = spiDriver->readByte();
	byte lowByte = spiDriver->readByte();
	
	rawPressure = highByte;
	rawPressure = rawPressure << 8 | midByte;
	rawPressure = rawPressure << 8 | lowByte;
	/*
	rawPressure = (unsigned long) highByte << 16;
	rawPressure |= (unsigned long) midByte << 8;
	rawPressure |= (unsigned long) lowByte;
	*/
	//rawPressure = (unsigned long)(((unsigned long)highByte) << 16) | (((unsigned long)midByte) << 8) | (unsigned long) lowByte;

	
	spiDriver->endTransaction();
	
	
	//Initiate conversion for temperature.
	spiDriver->transactionWrite(CONVERT_D2_TEMPERATURE_OSR_4096);	
	
	_delay_ms(ADC_PROCESSING_TIME_MS);
	
	spiDriver->beginTransaction();
	
	/**
	 * Read the ADC results
	 */
	spiDriver->write(ADC_READ_COMMAND);
	highByte = spiDriver->readByte();
	midByte = spiDriver->readByte();
	lowByte = spiDriver->readByte();
	
	rawTemperature = (long) highByte << 16 | (long) midByte << 8 | lowByte;

/*	
	rawTemperature = (unsigned long) highByte << 16;
	rawTemperature |= (unsigned long) midByte << 8;
	rawTemperature |= (unsigned long) lowByte;
	*/
	

	
	//rawTemperature = (unsigned long)(((unsigned long)highByte) << 16) | (((unsigned long)midByte) << 8) | (unsigned long) lowByte;
	
	spiDriver->endTransaction();
	
	
	uint64_t C1 = pressureSensitivity_SENSt1_C1;
	uint64_t C2 = pressureSensitivityOffset_OFFt1_C2;
	uint64_t C3 = temperatureCoefficientOfPressureSensitivity_TCS_C3;
	uint64_t C4 = temperatureCoefficientOfPressureOffset_TCO_C4;
	uint64_t C5 = referenceTemperature_Tref_C5;
	uint64_t C6 = temperatureCoefficientOfTheTemperature_TEMPSENS_C6;
	
	int64_t rawPress = rawPressure;
	int64_t rawTemp = rawTemperature;
	
	int64_t tempDifference = rawTemp - C5 * 256;
	
	int64_t temp = 2000 + tempDifference * C6 / 8388608;
	
	int64_t tempOffset = C2 * 65536 + (C4 * tempDifference) / 128;
	
	int64_t sensitivityAtTemp = C1 * 32768 + (C3 * tempDifference) / 256;
	
	int64_t press = (rawPress * sensitivityAtTemp / 2097152 - tempOffset) / 32768;
	
	//int64_t rawPress = rawPressure;
	//int64_t rawTemp = rawTemperature;
	//
	//int64_t tempDifference = rawPress - C5 * 256;
	//
	//int64_t temp = 2000 + tempDifference * temperatureCoefficientOfTheTemperature_TEMPSENS_C6 / 8388608;
	//
	//int64_t tempOffset = pressureSensitivityOffset_OFFt1_C2 * 65536 + (temperatureCoefficientOfPressureOffset_TCO_C4 * tempDifference) / 128;
	//
	//int64_t sensitivityAtTemp = pressureSensitivity_SENSt1_C1 * 32768 + (temperatureCoefficientOfPressureSensitivity_TCS_C3 * tempDifference) / 256;
	//
	//int64_t press = (rawPressure * sensitivityAtTemp / 2097152 - tempOffset) / 32768;

	
	
	
	pressure = press;
	temperature = temp;
	//
	///**
	 //* This code was taken from AP_Baro_MS5611.cpp from adrucopter
	 //* Formulas from page 7 on MS5611-01BA03.pdf
	 //*/
	//// Formulas from manufacturer datasheet
	//// sub -20c temperature compensation is not included
//
	//// we do the calculations using floating point
	//// as this is much faster on an AVR2560, and also allows
	//// us to take advantage of the averaging of D1 and D1 over
	//// multiple samples, giving us more precision
	//float dT;
	//float TEMP;
	//float OFF;
	//float SENS;
		//
	//dT = rawTemperature-(((uint32_t)referenceTemperature_Tref_C5)<<8);
	//TEMP = (dT * temperatureCoefficientOfTheTemperature_TEMPSENS_C6)/8388608;
	//OFF = pressureSensitivityOffset_OFFt1_C2 * 65536.0 + (temperatureCoefficientOfPressureOffset_TCO_C4 * dT) / 128;
	//SENS = pressureSensitivity_SENSt1_C1 * 32768.0 + (temperatureCoefficientOfPressureSensitivity_TCS_C3 * dT) / 256;
//
	//if (TEMP < 0) {
		//// second order temperature compensation when under 20 degrees C
		//float T2 = (dT*dT) / 0x80000000;
		//float Aux = TEMP*TEMP;
		//float OFF2 = 2.5*Aux;
		//float SENS2 = 1.25*Aux;
		//TEMP = TEMP - T2;
		//OFF = OFF - OFF2;
		//SENS = SENS - SENS2;
	//}
//
	//pressure = (rawPressure*SENS/2097152 - OFF)/32768;
	//temperature = TEMP + 2000;
	
}

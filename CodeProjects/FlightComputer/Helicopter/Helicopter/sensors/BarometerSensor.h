/*
 * BarometerSensor.h
 *
 * Created: 2/18/2014 10:20:11 PM
 *  Author: HP User
 */ 


#ifndef BAROMETERSENSOR_H_
#define BAROMETERSENSOR_H_


#include "SPIDriver.h"

using namespace helicopter::drivers;


namespace helicopter
{
	namespace sensors
	{
		class BarometerSensor
		{
			public:
				/**
				 * According to page 10 of MS5611-01Ba03.PDF, it takes 8.22 ms to do the
				 * ADC conversion for OSR of 4096.
				 */
				static const int ADC_PROCESSING_TIME_MS = 10;
				
			private:
			
				static const byte ADC_READ_COMMAND = 0x00;
				static const byte RESET_COMMAND = 0x1E;
				static const byte READ_PROM_C1_COMMAND = 0xA2;
				static const byte READ_PROM_C2_COMMAND = 0xA4;
				static const byte READ_PROM_C3_COMMAND = 0xA6;
				static const byte READ_PROM_C4_COMMAND = 0xA8;
				static const byte READ_PROM_C5_COMMAND = 0xAA;
				static const byte READ_PROM_C6_COMMAND = 0xAC;

				
				/**
				 * Have the analog to digital converter on the barometer convert the D1 pressure
				 * value with an oversampling rate (OSR) of 4096.
				 */
				static const byte CONVERT_D1_PRESSURE_OSR_4096 = 0x48;
				
				/**
				 * Have the analog to digital converter on the barometer convert the D2 temperature
				 * value with an oversampling rate (OSR) of 4096.
				 */
				static const byte CONVERT_D2_TEMPERATURE_OSR_4096 = 0x58;

				

				
				//According to the documentation (page 10), it's only a 2.8ms delay, but delay for 5ms just in case.
				static const int RESET_PROCESSING_TIME_MS = 5;								
			
				SPIDriver *spiDriver; 
				
				/**
				 * The raw sensor readings from the sensor
				 */
				unsigned long rawTemperature;
				unsigned long rawPressure;
				
				/**
				 * The temperature and pressure values after being calculated from the raw
				 * sensor readings.
				 */
				
				/**
				 * This is the temperature in Celsius * 100. so 23 degrees C is 2300.
				 */
				long temperatureCelcius;
				long pressureMillibars;
				
				/**
				 * Each barometer is individually calibrated at the factory. These values are
				 * used in calculations to compensate temperature and pressure values. 
				 * Those values are stored in the programmable read only memory (PROM) of the barometer
				 * I made these int64_t even though the documentation says these could be uint16 because
				 * when doing the math with these variables, it required 64 bit values and signed values.
				 * if I tried with uint64's or a smaller size, massive math errors ocurred.
				 */
				int64_t pressureSensitivity_SENSt1_C1;
				int64_t pressureSensitivityOffset_OFFt1_C2;
				int64_t temperatureCoefficientOfPressureSensitivity_TCS_C3;
				int64_t temperatureCoefficientOfPressureOffset_TCO_C4;
				int64_t referenceTemperature_Tref_C5;
				int64_t temperatureCoefficientOfTheTemperature_TEMPSENS_C6;
				
			public:
				/**
				 * Constructs the accel sensor.
				 * @param driver The SPI driver used to communicate with the
				 * MPU6000's accelerometer sensor via the SPI protocol.
				 */
				BarometerSensor(SPIDriver *spiDriver): 
					spiDriver (spiDriver),
					rawTemperature(0),
					rawPressure(0),
					temperatureCelcius(0),
					pressureMillibars(0),
					pressureSensitivity_SENSt1_C1(0),
					pressureSensitivityOffset_OFFt1_C2(0),
					temperatureCoefficientOfPressureSensitivity_TCS_C3(0),
					temperatureCoefficientOfPressureOffset_TCO_C4(0),
					referenceTemperature_Tref_C5(0),
					temperatureCoefficientOfTheTemperature_TEMPSENS_C6(0)
				{
				}
				
				
				/**
				 * Reads the sensor values from the sensor and converts the values into
				 * usable temperature and pressure formats.
				 */
				void readSensor();
				
				void processBaroData();
				
				void readD2Data();
				
				void sendD2Command();
				
				void readD1Data();
				
				void sendD1Command();
				
				/**
				 * Initializes the sensor
				 */
				void init();
								
				
				unsigned long getRawTemperature()
				{
					return rawTemperature;
				}
				
				unsigned long getRawPressure()
				{
					return rawPressure;
				}		
				
				long getTemperatureCelcius()
				{
					return temperatureCelcius;
				}
				
				long getPressureMillibars()
				{
					return pressureMillibars;
				}								
		};
	}
}


#endif /* BAROMETERSENSOR_H_ */
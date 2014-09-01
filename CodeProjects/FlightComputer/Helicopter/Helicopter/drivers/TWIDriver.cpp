/*
 * TWIDriver.cpp
 *
 * Created: 2/8/2014 5:52:49 PM
 *  Author: HP User
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "TWIDriver.h"

using namespace helicopter::drivers;

void TWIDriver::reset()
{
	this->stop();
	TWCR &= ~(1<<TWEN);
}

void TWIDriver::init()
{
	//From this code located here: https://github.com/diydrones/ardupilot/blob/8f4665c4c7e36714cc35eaf9dfe764a788fcf4c1/libraries/AP_HAL_AVR/I2CDriver.cpp
	
	/**
	 * Apparently the twi driver needs to be initialized with a TWI Bit Rate (TWBR - twi bit rate register)
	 * because if you run it in conjunction with the servo radio controller, it'll cause the wait for twint to wait forever
	 * if you don't set up the bit rate.
	 */	
	
	//Set the data line and clock line to output. 
	DDRD |= (1<<PD0);
	DDRD |= (1<<PD1);

	//Set the prescaler for twi to 1
	TWSR &= ~(1<<TWPS0);
	TWSR &= ~(1<<TWPS1);

	//set the bit rate (page 248 of atmega2560 data sheet)
	//this is 200khz clock rate. 
	//equation is here http://www.ermicro.com/blog/?p=744
	//SCL freq = cpu clock freq / (16 + 2(TWBR) * prescaler)
	TWBR = ((16000000UL / 400000UL) - 16UL) / 2UL;
}

bool TWIDriver::start()
{
	//Send the Start condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	//wait for twint flag to be set.
	//This indicates that the start condition was sent.
//	while (!(TWCR & (1<<TWINT)))
//	;
	for (int i = 0; i < 100  && !(TWCR & (1<<TWINT)); i++)
	{
	}

	if (!(TWCR & (1<<TWINT)))
	{
		return false;
	}
	
	
	
	//Verify that start condition was successfully sent.
	int status = TWSR & 0xF8;
	
	if (status == START ||
	 status == REPEATED_START)
	{	
		return true;
	}

	
	return false;
}


void TWIDriver::stop()
{
	//Send the stop condition.
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

bool TWIDriver::write( byte data, byte acknowledgeValue)
{
	//Place data onto the Two Wire Data Register
	TWDR = data; 

	//Start the transmission of the data.
	//Writing 1 to TWINT clears TWINT. Otherwise, TWINT could hold the SCL line low so that other transmissions
	//arn't allowed. Writing 1 clears it so transmissions are allowed. 
	//Setting TWEN free's up the pin so that the clock can take control of it for managing clock cycles between the two devices. 
	TWCR = (1<<TWINT) | (1<<TWEN); 
	
	

	
	//Wait for TWINT flag to be set. This indicates that the data has been transmitted
	//and the ack/nack has been received.  
	//while (!(TWCR & (1<<TWINT)))
	//;
	for (int i = 0; i < 100  && !(TWCR & (1<<TWINT)); i++)
	{
	}

	if (!(TWCR & (1<<TWINT)))
	{
		return false;
	}

	
	//If an acknowledgment value was given,
	//check to ensure that the ack value received from TW matched
	//the parameter
	if (acknowledgeValue != 0 && 
		((TWSR & TWI_STATUS_MASK) != acknowledgeValue))
	{
		return false;
	}
	
	return true;
}

bool TWIDriver::readByte(bool acknowledge, byte& val)
{
	//Clear the interrupt to start receiving data (why acknowledge?)*** clocks out data??
	//TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	if (acknowledge)
	{
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	}else
	{
		TWCR = (1<<TWINT) | (1<<TWEN);
	}
	
	
	//Wait for the interrupt to be reset, indicating that we received the data
	//while (!(TWCR & (1<<TWINT))) ;
	for (int i = 0; i < 100  && !(TWCR & (1<<TWINT)); i++)
	{
	}
	
	if (!(TWCR & (1<<TWINT)))
	{
		val = 0;
		return false;
	}

	
	//Read the data.
	//byte receivedValue = TWDR;
	val = TWDR;
	
	//return receivedValue;
	return true;
}

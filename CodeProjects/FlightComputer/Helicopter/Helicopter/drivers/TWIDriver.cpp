/*
 * TWIDriver.cpp
 *
 * Created: 2/8/2014 5:52:49 PM
 *  Author: HP User
 */ 

#include <avr/io.h>
#include <avr/delay.h>

#include "TWIDriver.h"

using namespace helicopter::drivers;


void TWIDriver::init()
{
	/**
	* Configure the two wire control register (TWCR) with the following values:
	* TWI Enable acknowledge bit (TWEA) - enables acknowledgment of data
	* TWI Enable bit (TWEN) - enables operation and activates the TWI Interface.
	*/
	//TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA);
	
	//***trying this without the interrupt flat. 
	TWCR = (1<<TWEN) | (1<<TWEA);
}

bool TWIDriver::start()
{
	//Send the Start condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	//wait for twint flag to be set.
	//This indicates that the start condition was sent.
	while (!(TWCR & (1<<TWINT)))
	;
	
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
	while (!(TWCR & (1<<TWINT)))
	;
	
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

byte TWIDriver::readByte(bool acknowledge)
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
	while (!(TWCR & (1<<TWINT))) ;
	
	//Read the data.
	byte receivedValue = TWDR;
	
	/**
	 * Sending an acknowledgment will tell the sending device that the data
	 * was successfully received (and in the case of the magnetometer, moves the
	 * register to the next address so that the next read will read the next value)
	 */
//	acknowledge();
	
	return receivedValue;
}

/*
 * SerialDriver.cpp
 *
 * Created: 8/25/2013 5:08:35 PM
 *  Author: HP User
 */ 
#include <stdint.h>
#include <math.h>
#include <avr/io.h>

#include "SerialDriver.h"
#include "commonheader.h"

using namespace helicopter::drivers;

void SerialDriver::initialize()
{
	unsigned int baudPrescaller = 0;
	
	if (useDoubleSpeedMode)
	{
		baudPrescaller = floor((((F_CPU / (baudRate * 8UL))) - 1UL));
	}else
	{
		//Calculation for the baud prescaller 
		baudPrescaller = floor((((F_CPU / (baudRate * 16UL))) - 1UL));
	}
	
	
	if (uartPort == Zero)
	{
		/* Set baud rate */
		UBRR0 = baudPrescaller;
		
		/* Set frame format: asynchronous usart, no parity 1stop bit, 8 bits of data, */
		UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
		
		//If double speed mode flag was set, set the U2X0 flag to enable double speed mode.
		if (useDoubleSpeedMode)
		{
			UCSR0A |= (1<<U2X0);
		}
		
			
			
		/* Enable receiver and transmitter */
		UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	}
}

int SerialDriver::transmitByte(byte byteToSend)
{
	if (uartPort == Zero)
	{
		int counter = 0;
		
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) )
		{
			//Check for timeout
			if (isTimeoutEnabled)
			{
				if (counter > SENDTIMEOUTCOUNTER)
				{
					return -1;
				}
				
				counter++;
			}
		}
				
				
		/* Put data into buffer, sends the data */
		UDR0 = byteToSend;
	}
	
	return 0;
}

int SerialDriver::receiveByte(byte &receivedByte)
{
	if (uartPort == Zero)
	{
		int counter = 0;
		
		/* Wait for data on the receive buffer */
		while ( !(UCSR0A & (1<<RXC0)) )
		{
			if (isTimeoutEnabled)
			{
				//Check for timeout
				if (counter > RECEIVETIMEOUTCOUNTER)
				{
					return -1;
				}
			
				counter++;			
			}
		}
		
		
		/* Put data into buffer, sends the data */
		receivedByte = UDR0;
	}
	
	return 0;
}
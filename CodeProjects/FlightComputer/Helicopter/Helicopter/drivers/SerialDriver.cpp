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
#include "CommonHeader.h"

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

int SerialDriver::transmitByte(byte byteToSend, Timer *timer)
{
	int status = 0;
	
	if (uartPort == Zero)
	{

		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) )
		{
			//Check for timeout
			if (timer->hasTimedout())
			{
				status = -1;
				break;
			}
		}
				
		if (status == 0)
		{
			/* Put data into buffer, sends the data */
			UDR0 = byteToSend;			
		}

	}
	
	return status;
}

int SerialDriver::receiveByte(byte &receivedByte, Timer *timer)
{
	int status = 0;
	
	if (uartPort == Zero)
	{
		
		/* Wait for data on the receive buffer */
		while ( !(UCSR0A & (1<<RXC0)))
		{
			if (timer->hasTimedout())
			{
				status = -1;
				break;
			}
		}
		
		if (status == 0)
		{
			//determine if there has been a data overrun.
			if ((UCSR0A & (1 << DOR0)) != 0)
			{
				status = -2;
			}
		
			/* Read the data from the serial port buffer, even if the buffer was overrun */
			receivedByte = UDR0;
		}
	}

	return status;
}
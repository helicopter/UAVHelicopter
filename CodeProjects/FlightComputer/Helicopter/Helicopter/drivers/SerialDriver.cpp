/*
 * SerialDriver.cpp
 *
 * Created: 8/25/2013 5:08:35 PM
 *  Author: HP User
 */ 
#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <string.h>

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

int SerialDriver::transmit(float valueToSend, Timer *timer)
{
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	
	byte bytes[4] = {0};
	
	memcpy(bytes, (void*) &valueToSend, 4);
	
	status1 = transmit(bytes[0],timer);
	status2 = transmit(bytes[1],timer);
	status3 = transmit(bytes[2],timer);
	status4 = transmit(bytes[3],timer);
	
	return status1 | status2 | status3 | status4;
}

int SerialDriver::transmit(int valueToSend, Timer *timer)
{
	int status1 = 0;
	int status2 = 0;
	
	status1 = transmit((byte)(((int)valueToSend) >> 8),timer);
	status2 = transmit((byte)valueToSend,timer);	
	
	return status1 | status2;
}

int SerialDriver::transmit(unsigned long valueToSend, Timer *timer)
{
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	

	status1 = transmit((byte)((valueToSend >> 24) & 0xFF),timer);
	status2 = transmit((byte)((valueToSend >> 16) & 0xFF),timer);
	status3 = transmit((byte)((valueToSend >> 8) & 0xFF),timer);
	status4 = transmit((byte)(valueToSend & 0xFF),timer);
	
	return status1 | status2 | status3 | status4;
}

int SerialDriver::transmit(long valueToSend, Timer *timer)
{
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	

	status1 = transmit((byte)((valueToSend >> 24) & 0xFF),timer);
	status2 = transmit((byte)((valueToSend >> 16) & 0xFF),timer);
	status3 = transmit((byte)((valueToSend >> 8) & 0xFF),timer);
	status4 = transmit((byte)(valueToSend & 0xFF),timer);
	
	return status1 | status2 | status3 | status4;
}

int SerialDriver::transmit(byte valueToSend, Timer *timer)
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
			UDR0 = valueToSend;			
		}

	}
	
	return status;
}

int SerialDriver::receive(byte &receivedByte, Timer *timer)
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
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

void SerialDriver::startTimer()
{
	if (timer != NULL)
	{
		timer->startTimer();
	}
	
}

void SerialDriver::stopTimer()
{
	if (timer != NULL)
	{
		timer->stopTimer();
	}
}

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
	} else if (uartPort == One)
	{
		/* Set baud rate */
		UBRR1 = baudPrescaller;
		
		/* Set frame format: asynchronous usart, no parity 1stop bit, 8 bits of data, */
		UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
		
		//If double speed mode flag was set, set the U2X0 flag to enable double speed mode.
		if (useDoubleSpeedMode)
		{
			UCSR1A |= (1<<U2X1);
		}

		/* Enable receiver and transmitter */
		UCSR1B |= (1<<RXEN1) | (1<<TXEN1);		
	}
}

int SerialDriver::transmit(float valueToSend)
{
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	
	byte bytes[4] = {0};
	
	//Memcpy the float into an array of bytes because the compiler doesn't like
	//bit shifting floats. 
	memcpy(bytes, (void*) &valueToSend, 4);
	
	status1 = transmit(bytes[0]);
	status2 = transmit(bytes[1]);
	status3 = transmit(bytes[2]);
	status4 = transmit(bytes[3]);
	
	return status1 | status2 | status3 | status4;
}

int SerialDriver::transmit(int valueToSend)
{
	int status1 = 0;
	int status2 = 0;
	
	status1 = transmit((byte)(valueToSend >> 8));
	status2 = transmit((byte)valueToSend);	
	
	return status1 | status2;
}

int SerialDriver::transmit(unsigned long valueToSend)
{
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	

	status1 = transmit((byte)((valueToSend >> 24) & 0xFF));
	status2 = transmit((byte)((valueToSend >> 16) & 0xFF));
	status3 = transmit((byte)((valueToSend >> 8) & 0xFF));
	status4 = transmit((byte)(valueToSend & 0xFF));
	
	return status1 | status2 | status3 | status4;
}

int SerialDriver::transmit(long valueToSend)
{
	int status1 = 0;
	int status2 = 0;
	int status3 = 0;
	int status4 = 0;
	

	status1 = transmit((byte)((valueToSend >> 24) & 0xFF));
	status2 = transmit((byte)((valueToSend >> 16) & 0xFF));
	status3 = transmit((byte)((valueToSend >> 8) & 0xFF));
	status4 = transmit((byte)(valueToSend & 0xFF));
	
	return status1 | status2 | status3 | status4;
}

int SerialDriver::transmit(const char *buffer)
{
	int length = strlen(buffer);
	
	return transmit (buffer, length);
}

int SerialDriver::transmit(const char *buffer, int numOfBytes)
{
	int status = 0;

	for (int i = 0; i < numOfBytes && status == 0; i++)
	{
		status = transmit((byte) buffer[i]);
	}
	
	return status;
}

int SerialDriver::timedTransmit(const char *buffer, int numOfBytes)
{
	int status = 0;
	
	if (timer != NULL)
	{
		timer->startTimer();
	}
	
	
	for (int i = 0; i < numOfBytes && status == 0; i++)
	{
		status = transmit(buffer[i]);
	}
	
	
	if (timer != NULL)
	{
		timer->stopTimer();
	}
	
	return status;
}

int SerialDriver::transmit(byte valueToSend)
{
	int status = 0;
	
	if (uartPort == Zero)
	{

		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) )
		{
			//Check for timeout
			if (timer != NULL && timer->hasTimedout())
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

	}else if (uartPort == One)
	{
		/* Wait for empty transmit buffer */
		while ( !( UCSR1A & (1<<UDRE1)) )
		{
			//Check for timeout
			if (timer != NULL && timer->hasTimedout())
			{
				status = -1;
				break;
			}
		}
		
		if (status == 0)
		{
			/* Put data into buffer, sends the data */
			UDR1 = valueToSend;
		}		
	}
	
	return status;
}

int SerialDriver::receive(byte &receivedByte)
{
	int status = 0;
	
	if (uartPort == Zero)
	{
		
		/* 
		* Wait for data on the receive buffer. This is done by
		* checking the Receiver Complete register (RXC)
		*/
		while ( !(UCSR0A & (1<<RXC0)))
		{
			if (timer != NULL && timer->hasTimedout())
			{
				status = -1;
				break;
			}
		}
		
		if (status == 0)
		{
			/*
			* Determine if there has been:
			* a Data Overrun (Data Overrun Register (DOR))
			* a parity error (USART parity error (UPE))
			* a Frame error (Frame Error (FE))
			*/
			if ((UCSR0A & ((1 << DOR0) | (1<<FE0) | (1<<UPE0))) != 0)
			{
				status = -2;
			}
		
			/* Read the data from the serial port buffer, even if the buffer was overrun */
			receivedByte = UDR0;
		}
	} else if (uartPort == One)
	{
			
		/* 
		* Wait for data on the receive buffer. This is done by
		* checking the Receiver Complete register (RXC)
		*/
		while ( !(UCSR1A & (1<<RXC1)))
		{
			if (timer != NULL && timer->hasTimedout())
			{
				status = -1;
				break;
			}
		}
			
		if (status == 0)
		{
			/*
			* Determine if there has been:
			* a Data Overrun (Data Overrun Register (DOR))
			* a parity error (USART parity error (UPE))
			* a Frame error (Frame Error (FE))
			*/

			if ((UCSR1A & ((1 << DOR1) | (1<<FE1) | (1<<UPE1))) != 0)
			{
				status = -2;
			}
				
			/* Read the data from the serial port buffer, even if the buffer was overrun */
			receivedByte = UDR1;
		}
	}

	return status;
}

void SerialDriver::clearBuffer()
{
	byte dummy = 0;
	
	if (uartPort == Zero)
	{
		//This clears the buffers, and since the byte is shifted off of the
		//Usart Data Register (UDR),  any buffer overrun or other error flags are cleared as well
		while (UCSR0A & (1<<RXC0) )
		{
			dummy = UDR0;
		}
	} else if (uartPort == One)
	{
		while (UCSR1A & (1<<RXC1) )
		{
			dummy = UDR1;
		}		
	}
	
	dummy++; // here to remove 'set but not used' warning
}
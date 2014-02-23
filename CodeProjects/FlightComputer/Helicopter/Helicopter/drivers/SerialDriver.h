/*
 * SerialDriver.h
 *
 * Created: 8/25/2013 5:08:21 PM
 *  Author: HP User
 */ 


#ifndef SERIALDRIVER_H_
#define SERIALDRIVER_H_

#include "Timer.h"
#include "CommonHeader.h"

using namespace helicopter::util;


namespace helicopter 
{
	namespace drivers
	{
		/**
		 * This class is used for interfacing with the Uart Serial hardware
		 * for sending messages using serial communication.
		 */
		class SerialDriver
		{
			public: 
				/**
				 * Data type for identifying which UART port 
				 */
				enum UartPort {Zero};
					
			private:
				
				unsigned long baudRate;
				
				UartPort uartPort;
				
				bool useDoubleSpeedMode;
				
			public:
			
			//				timer(F_CPU, 1024, 75), //This is decent for the radio. it drops a packet every now and then.
			//timer(F_CPU, 1024, 10),   //this is decent for the usb. it drops a packet every now and then.
				
				/**
				 * Creates a driver for communicating with the UART serial port
				 * @param baudRate the baud rate for communicating with the serial modem
				 * @param uartPort Which UART port on the board to utilize.
				 * @param useDoubleSpeedMode A flag indicating if double speed mode should
				 * be used when calculating the baud rate configurations. This is useful for
				 * using the radio interface because it seems to have issues if double speed
				 * mode isn't set to true.
				 */		
				SerialDriver(
					unsigned long baudRate, 
					UartPort uartPort,
					bool useDoubleSpeedMode = true) :
				baudRate(baudRate),
				uartPort(uartPort),
				useDoubleSpeedMode(useDoubleSpeedMode)
				{
					
				}
						
				virtual ~SerialDriver()
				{
					
				}
			
				/** 
				* An initializer to initialize the driver 
				* This is necessary before communication with
				* the hardware can begin
				*/
				virtual void initialize();
				
				/**
				 * Sends a byte over Serial
				 * @byteToSend Byte to send over Serial
				 */
				virtual int transmit(byte valueToSend, Timer *timer);
				
				virtual int transmit(int valueToSend, Timer *timer);
				
				virtual int transmit(unsigned long valueToSend, Timer *timer);
				
				virtual int transmit(long valueToSend, Timer *timer);
				
				virtual int transmit(float valueToSend, Timer *timer);
				
				/**
				 * Receive Bytes over Serial
				 * @receivedByte byte received from serial
				 * @return -1 if there was an error (timeout), 
				 * 0 otherwise (success)
				 */
				virtual int receive(byte &receivedByte, Timer *timer);
		};
	}
}


#endif /* SERIALDRIVER_H_ */
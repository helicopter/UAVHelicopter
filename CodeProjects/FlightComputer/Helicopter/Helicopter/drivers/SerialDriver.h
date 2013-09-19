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
				
				Timer *timer;
				
				bool isTimeoutEnabled;
				
				bool useDoubleSpeedMode;
				
			public:
			
			//				timer(F_CPU, 1024, 75), //This is decent for the radio. it drops a packet every now and then.
			//timer(F_CPU, 1024, 10),   //this is decent for the usb. it drops a packet every now and then.
				
				/**
				 * Creates a driver for communicating with the UART serial port
				 * @param baudRate the baud rate for communicating with the serial modem
				 * @param uartPort Which UART port on the board to utilize.
				 * @param timer Timer object which controls when to timeout when receiving and transmitting data
				 * only used when enableTimeout = true.
				 * @param enableTimeout Bool indicating if the system should timeout
				 * while waiting to send or receive data. True by default so that the
				 * driver will timeout if it can't send or receive data after a short timeout period.
				 */		
				SerialDriver(
					unsigned long baudRate, 
					UartPort uartPort,
					Timer *timer,
					bool enableTimeout = true,
					bool useDoubleSpeedMode = false) :
				baudRate(baudRate),
				uartPort(uartPort),
				timer(timer),
				isTimeoutEnabled(enableTimeout),
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
				virtual int transmitByte(byte byteToSend);
				
				/**
				 * Receive Bytes over Serial
				 * @receivedByte byte received from serial
				 * @return -1 if there was an error (timeout), 
				 * 0 otherwise (success)
				 */
				virtual int receiveByte(byte &receivedByte);
		};
	}
}


#endif /* SERIALDRIVER_H_ */
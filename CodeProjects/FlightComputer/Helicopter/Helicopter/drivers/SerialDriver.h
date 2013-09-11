/*
 * SerialDriver.h
 *
 * Created: 8/25/2013 5:08:21 PM
 *  Author: HP User
 */ 


#ifndef SERIALDRIVER_H_
#define SERIALDRIVER_H_

#include "commonheader.h"

using namespace helicopter::util::common;

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
				enum UartPort {Zero};
					
			private:
				static const int SENDTIMEOUTCOUNTER = 40;
				static const int RECEIVETIMEOUTCOUNTER = 40;
			
				unsigned long baudRate;
				
				UartPort uartPort;
				
				bool isTimeoutEnabled;
				
				bool useDoubleSpeedMode;
				
			public:
				
				/**
				 * Creates a driver for communicating with the UART serial port
				 * @param baudRate the baud rate for communicating with the serial modem
				 * @param uartPort Which UART port on the board to utilize.
				 * @param enableTimeout Bool indicating if the system should timeout
				 * while waiting to send or receive data. True by default so that the
				 * driver will timeout if it can't send or receive data after a short timeout period.
				 */		
				SerialDriver(
					unsigned long baudRate, 
					UartPort uartPort,
					bool enableTimeout = true,
					bool useDoubleSpeedMode = false) :
				baudRate(baudRate),
				uartPort(uartPort),
				isTimeoutEnabled(enableTimeout),
				useDoubleSpeedMode(useDoubleSpeedMode)
				{}
						
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
				 * @return -1 if there was an error (timeout), 0 otherwise
				 */
				virtual int receiveByte(byte &receivedByte);
		};
	}
}


#endif /* SERIALDRIVER_H_ */
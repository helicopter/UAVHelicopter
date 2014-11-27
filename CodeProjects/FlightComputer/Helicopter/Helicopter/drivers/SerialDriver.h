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
#include "CircularBuffer.h"

using namespace helicopter::util;
using namespace helicopter::buffer;


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
				enum UartPort {Zero, One};
					
				static CircularBuffer receiveBuffer;
				
				static CircularBuffer transmitBuffer;
					
			//private:
				
				unsigned long baudRate;
				
				UartPort uartPort;
				
				bool useDoubleSpeedMode;
				
				bool asyncReceiveTransmitData;
				
				Timer *timer;
				
				
				
				
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
				 * @param asyncReceiveData Asynchronously receive data using interrupts rather than waiting for data. 
				 * @param timer A timer which is used to determine if the system has timed out while
				 * transmitting or receiving data. If this field is Null then the timeout mechanism won't be utilized.
				 */		
				SerialDriver(
					unsigned long baudRate, 
					UartPort uartPort,
					bool useDoubleSpeedMode = true,
					bool asyncReceiveTransmitData = false,
					Timer *timer = NULL) :
				baudRate(baudRate),
				uartPort(uartPort),
				useDoubleSpeedMode(useDoubleSpeedMode),
				asyncReceiveTransmitData(asyncReceiveTransmitData),
				timer(timer)
				{

				}
						
				virtual ~SerialDriver()
				{
					
				}
				
				/**
				 * If the timer isn't null (disabled), start the timer so that
				 * if the timer expires, the driver will stop trying to send or receive
				 * data. Calls timer->startTimer. This was implemented so classes
				 * which depended on serialDriver didn't have to depend on Timer in order
				 * to start and stop the timers
				 */
				void startTimer();
				
				/**
				 * If the timer isn't null (disabled), stop the timer. Calls timer->stopTimer
				 * This was implemented so classes
				 * which depended on serialDriver didn't have to depend on Timer in order
				 * to start and stop the timers
				 */
				void stopTimer();
			
				/** 
				* An initializer to initialize the driver 
				* This is necessary before communication with
				* the hardware can begin
				*/
				virtual void init();
				
				/**
				 * Sends a byte over Serial
				 * @byteToSend Byte to send over Serial
				 */
				virtual int transmit(byte valueToSend);
				
				virtual int transmit(int valueToSend);
				
				virtual int transmit(unsigned long valueToSend);
				
				virtual int transmit(long valueToSend);
				
				virtual int transmit(int64_t valueToSend);
				
				virtual int transmit(float valueToSend);
				
				/**
				 * Assuming a null terminated char buffer.
				 */
				int transmit(const char *buffer);
				
				int transmit(const char *buffer, int numOfBytes);
				
				/**
				 * Transmits the data within a timed context. The timer is started
				 * before transmits starts
				 * @return -1 if the timer timed out while transmitting. 
				 * -2 if a byte was received before we could process the current byte and there was an overrun
				 * 0 otherwise (success)
				 */
				int timedTransmit(const char *buffer, int numOfBytes);
				
				
				/**
				 * Receive Bytes over Serial
				 * @receivedByte byte received from serial
				 * @return -1 if there was an error (timeout), 
				 * 0 otherwise (success)
				 */
				virtual int receive(byte &receivedByte);
				
				
				/**
				 * Clears the buffer of any data, and clears any buffer overflow
				 * indicators. Continues to read data until there is no more data on the buffer.
				 * Don't use this in time sensitive code. 
				 * 
				 * The serial port has a 2 byte buffer where the last byte could be in a
				 * state of being overwritten multiple times. This clears any overwrite flags and
				 * any data still left in the buffer.
				 * Note: i think there are actually 3 total bytes. 2 buffer bytes + 1 current UDR value byte.
				 * The documentation indicates a 2 byte buffer on page 217 of the atmelavr2560_datasheet.pdf
				 */
				void clearBuffer();
		};
	}
}


#endif /* SERIALDRIVER_H_ */
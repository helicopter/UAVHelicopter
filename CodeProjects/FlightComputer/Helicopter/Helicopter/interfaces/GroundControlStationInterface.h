/*
 * RadioInterface.h
 *
 * Created: 8/25/2013 6:11:00 PM
 *  Author: HP User
 */ 


#ifndef GROUNDCONTROLSTATIONINTERFACE_H_
#define GROUNDCONTROLSTATIONINTERFACE_H_

#include "SerialDriver.h"
#include "Message.h"

using namespace helicopter::drivers;
using namespace helicopter::messages;

namespace helicopter
{
	namespace interfaces
	{
		/**
		 * Interface for communicating with the ground control station
		 */
		class GroundControlStationInterface
		{
			private:
				SerialDriver *serialDriver;
				
				Timer *timer;
				
				bool enableTimeout;
				
				/**
				 * Calculates the checksum of a message payload using a simple 8-bit fletcher algorithm
				 * @param msgPayload the payload of the message to calculate the checksum against
				 * @param payloadSize the number of bytes in the payload
				 * @param checksumA the first byte of the checksum
				 * @param checksumB the second byte of the checksum
				 */
				void calculateChecksum (byte *msgPayload, int payloadSize, byte &checksumA, byte &checksumB);
			
			public:
				/**
				 * Synchronization bytes indicating the start of a message. There are three
				 * sync bytes because the usart buffer is 3 bytes long. This is to resolve
				 * a bug where when I had 2 sync bytes, the system thought it was receiving a valid
				 * message, but it was an old message where the third byte was getting overwritten
				 * by more current data, causing the system to drop messages due to invalid ID's.
				 */
				static const byte SyncByte1 = 0xB5;
				static const byte SyncByte2 = 0x62;
				static const byte SyncByte3 = SyncByte1 ^ SyncByte2;
				
				static const int MsgHeaderFooterSize = 5;
			
				/**
				 * The interfaces with the 3DR radio for reading data and sending
				 * data utilizing the radio to and from the Ground Control station.
				 * The receiver and transmitter use the same serialport, and therefore it will timeout
				 * at the same time interval (i.e. 20 milliseconds) for both transmitting a byte as well
				 * as receiving a byte.
				 * @param serialDriver the driver used to communicate with the serial port
				 * which is used to communicate with the Ground Control Station.
				 * @param timer A timer used to determine if too much time has been spent transmitting
				 * data or receiving data.
				 * @param enableTimeout A flag indicating if the timer should be used to determine if the system should
				 * timeout when sending and receiving data. If false, the system will wait indefiniately for data.
				 * if true, the system will only wait a short period of time (specified by the timer) before continuing on.
				 */
				GroundControlStationInterface(SerialDriver *serialDriver, Timer *timer, bool enableTimeout = true):
					serialDriver(serialDriver),
					timer(timer),
					enableTimeout(enableTimeout)
					{}
						
				~GroundControlStationInterface()
				{
				}
					
				/**
				 * Utilizes the serial driver to transmit data over the radio.
				 * This is a blocking transmit so it will block as every byte in the message
				 * is sent.
				 * @param msgToSend The message to transmit over the radio.
				 * @return less than 0 in the event of an error (timeout), 0 otherwise
				 */
				int transmit(Message *msgToSend);
				
				/**
				 * Receives an incoming message.
				 * @param receivedMessage the message received. The caller is responsible
				 * for 'freeing' the message since this message is on the heap.
				 * @return -1 for a timeout error (we didn't receive all the data in time), 
				 * -2 in the event of a buffer overrun which can occur if we didn't receive
				 * a byte in time and it was overwriten by another incoming byte
				 * -3 if the message type received was not a system telemetry message (unknown message type)
				 * -4 if the checksum failed
				 * 0 otherwise (success).
				 */
				int receive(Message * &receivedMessage);
		};
	}
}


#endif /* GROUNDCONTROLSTATIONINTERFACE_H_ */
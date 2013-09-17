/*
 * RadioInterface.h
 *
 * Created: 8/25/2013 6:11:00 PM
 *  Author: HP User
 */ 


#ifndef RADIOINTERFACE_H_
#define RADIOINTERFACE_H_

#include "SerialDriver.h"
#include "Message.h"

using namespace helicopter::drivers;
using namespace helicopter::messages;

namespace helicopter
{
	namespace interfaces
	{
		/**
		 * Interface for communicating with the 3DR Radio Telemetry kit
		 */
		class RadioInterface
		{
			private:
				SerialDriver *serialDriver;
				
				/**
				 * Calculates the checksum of a message payload using a simple 8-bit fletcher algorithm
				 */
				void calculateChecksum (byte *msgPayload, int payloadSize, byte &checksumA, byte &checksumB);
			
			public:
				static const byte SyncByte1 = 0xB5;
				static const byte SyncByte2 = 0x62;
				static const byte SyncByte3 = SyncByte1 ^ SyncByte2;
				
				static const int MsgHeaderFooterSize = 5;
			
				/**
				 * The radio interface interfaces with the radio for reading data and sending
				 * data utilizing the radio.
				 * The message builder is used to build a message being read from the radio
				 * therefore, this radio interface and this message builder should not be shared
				 * with other radio interfaces, and other messages should not be read from 
				 * the radio until this message has been completely read.
				 * I.e. don't create a radio interface, and pass in the same message builder
				 * to another radio interface and have an interrupt call receive on both radio interface.
				 * This is because internally, the message builder 'builds' the message as the bytes
				 * are received. If another message starts to be received and starts inserting
				 * bytes into the same internal message structure, it will corrupt the message
				 * being built. 
				 */
				RadioInterface(SerialDriver *serialDriver):
					serialDriver(serialDriver)
					{}
						
				//TODO:
				//implement a delete method to delete the serialDriver and builder.
					
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
				 * @return -1 in the event of an error (timeout), -2 if the message type
				 * received was not a system telemetry message, -3 if the checksum failed
				 * 0 otherwise (success).
				 */
				int receive(Message * &receivedMessage);
		};
	}
}


#endif /* RADIOINTERFACE_H_ */
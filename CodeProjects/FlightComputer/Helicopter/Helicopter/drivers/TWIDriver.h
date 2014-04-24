/*
 * TWIDriver.h
 *
 * Created: 2/8/2014 5:49:59 PM
 *  Author: HP User
 */ 


#ifndef TWIDRIVER_H_
#define TWIDRIVER_H_

#include "CommonHeader.h"

namespace helicopter 
{
	namespace drivers
	{

		/**
		 * This class encapsulates the logic for communicating using the
		 * Two Wire Serial Interface (TWI) protocol (also called I2C).

			TWEN - TWI ENABLE BIT - The TWEN bit enables TWI operation and activates the TWI interface. When TWEN is written to
			one, the TWI takes control over the I/O pins connected to the SCL and SDA pins

			TWEA - TWI ENABLE ACKNOWLEDGE BIT - The TWEA bit controls the generation of the acknowledge pulse. If the TWEA bit is written to
			one, the ACK pulse is generated on the TWI bus

			TWISTA - TWI START CONDITION BIT - The application writes the TWSTA bit to one when it desires to become a Master on the 2-wire
			Serial Bus.

			TWSTO - TWI STOP CONDITION BIT - Writing the TWSTO bit to one in Master mode will generate a STOP condition on the 2-wire
			Serial Bus.

			TWINT - TWI Interrupt flag - This bit is set by hardware when the TWI has finished its current job and expects application
			software response. clear by writing a one to it.

			Enable TWINT and TWEN to start a transmission	 
		 */
		
				
		class TWIDriver
		{
			private:
				static const byte START = 0x08;
				static const byte REPEATED_START = 0x10;
				
				/**
				 * This is used to mask off the twi status bits which are the first five
				 * bits in the Two wire status register (TWSR).
				 * (The rest of the bits in the TWSR are prescaller bits and a reserved bit)
				 */
				static const byte TWI_STATUS_MASK = 0xF8;
				
			public: 
				/**
				 * Indicates that the sensor acknowledges that the cpu is in master transmit
				 * mode and will be writing data to the sensor in the subsequent transmissions
				 */
				static const byte MASTERTRANSMIT_SLAVE_WRITE_ACK = 0x18;
				
				/**
				 * Indicates that the sensor acknowledges that data was received from the CPU
				 * which was the master transmitter.
				 */
				static const byte MASTERTRANSMIT_DATA_ACK = 0x28;
				
				/**
				* Indicates that the sensor acknowledges that the master has been put
				* into receiver mode, and that data should be transmitted from the
				* sensor to the receive in subsequent calls.
				*/
				static const byte MASTERRECEIVER_SLAVE_READ_ACK = 0x40;
				
				/**
				 * Starts communication with a TW device.
				 * If called again before 'stop' is called, then this is
				 * treated as a 'repeated start'. This is just a simplification
				 * so stop doesn't have to be called before start again.
				 */
				bool start();

				void stop();	
				
				bool write( byte data, byte acknowledgeValue = 0);	
				
				/**
				 * Reads a byte from the two wire line.
				 * @param acknowledge Flag indicating that if after the byte is read, should 
				 * the system send an acknowledgment (ACK) or not (NACK).
				 * The acknowledgment has to be set BEFORE the byte is read because sending the ack
				 * is done at the same time after the last bit is read from the line. I.e. 8 bits are read
				 * from the line, then during the same transfer process, the ack bit is checked, and if
				 * it's set to acknowledge, then the acknowledgment is sent to the device. Otherwise
				 * the NACK is sent. 
				 */
				byte readByte(bool acknowledge);
				
				void init();
		};
	}
}






#endif /* TWIDRIVER_H_ */
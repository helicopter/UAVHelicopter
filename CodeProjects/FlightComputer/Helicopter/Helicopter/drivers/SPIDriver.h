/*
 * SPIDriver.h
 *
 * Created: 1/28/2014 7:11:18 PM
 *  Author: HP User
 */ 


#ifndef SPIDRIVER_H_
#define SPIDRIVER_H_

#include "CommonHeader.h"

namespace helicopter 
{
	namespace drivers
	{

		/**
		 * This class encapsulates the hardware for communicating with
		 * an Serial peripheral Interface (SPI) device.
		 */
		class SPIDriver
		{
			public: 
			
				/**
				 * Defines which slave select line should be used for
				 * communicating with a device.
				 * SS_G - This is used by the Barometer
				 * SS_B - This is used by the Inertial Measurement Unit (IMU)
				 */
				enum SLAVE_SELECT_LINE {SS_B = 1, SS_G = 2};			
							
			private:
				SLAVE_SELECT_LINE ssLine;
			
			public: 
			
				SPIDriver(SLAVE_SELECT_LINE ssLine = SS_B):
					ssLine(ssLine)
				{
					
				}
				
			
				/**
				 * Initializes the various SPI ports for writing and
				 * reading data from an SPI device.
				 */
				void init();

				/**
				 * The SPI protocol works by first pulling the Slave Select line low
				 * indicating that data is ready to be transmitted on the master.
				 */
				void beginTransaction();
				
				/**
				 * Free the SPI line by driving the slave select line high
				 * indicating that this master device is done communicating.
				 */
				void endTransaction();				

				/**
				 * Write a byte of data to the SPI line.
				 */
				void write( byte data );
				
				/**
				 * Write a byte of data to the SPI line within a transaction.
				 */
				void transactionWrite( byte data );
				
				void write( byte commandAddress, byte commandValue );	
							
				/**
				* Writes the commandValue to the given commandAddress
				* enclosed within Begin and endTransaction calls.
				*/
				void transactionWrite( byte commandAddress, byte commandValue );
				
				/**
				 * Reads a 16 bit integer from the SPI line. This assumes
				 * the high byte is communicated first.
				 */
				int readInt();
				
				unsigned int readUInt();
				
				/**
				 * Reads a single byte from the SPI line.
				 */
				byte readByte();			
		};
	}
}





#endif /* SPIDRIVER_H_ */
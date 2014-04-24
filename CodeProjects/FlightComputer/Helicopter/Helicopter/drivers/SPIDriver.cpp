#include <avr/io.h>
#include <util/delay.h>

#include "SPIDriver.h"

using namespace helicopter::drivers;


/**
 * The following table shows the PIN mapping between the various pins
 * necessary for SPI communication, and what the pins map to 
 * as defined in the APM_v252_REALSE.sch document:
 * SPI Function | data direction pin | port pin | Descrip
 * MISO | PB3 | DDB3 | Master In Slave Out - If this device is the master, used for receiving data from the device that is being communicated with
 * MOSI | PB2 | DDB2 | Master Out Slave In - If this device is the master, used for this device to send data to other devices
 * SCK  | PB1 | DDB1 | SPI Clock  
 * SS| PB0/PG1 | DDB0/DDG1 | Slave Select(SS)/Chip Select(CS) - When this pin is set to 0, this indicates that the MOSI/MISO line is 'active' for transmission. When set high, it's inactive.
 */
void SPIDriver::init()
{
	/**
	* Configure the MOSI (DDB2) pin, SCK (DDB1), and SS (DDB0) pin (the SS pin for the accelerometer/IMU)
	* as output
	*/
	DDRB = (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
	
	/**
	 * Configure the SS pin for the barometer as output
	 */
	DDRG |= (1<<DDG1);
	
	//Set the slave select lines 'high', therefore freeing up the SPI line. (This would otherwise default to 0, making this device
	//hold onto the SPI line). This turns the line 'off' to 'end' any current transactions. These lines
	//default to low (active) so they have to be pulled high in order to free up the SS lines so a device
	//can communicate on the line.
	//Note: Slave Select for the IMU (accelerometer, etc) is PB0, while slave select for the barometer is PG1.
	PORTB |= (1<<PB0);
	PORTG |= (1<<PG1);

	/**
	 * SPI Control Register (SPCR)
	 * Enable SPI (1<<SPE)
	 * Set this device as the master device on the SPI line (1<<MSTR)
	 * Set clock frequency to Fosc / 16 (1<<SPR0) (clock speed divided by 16)
	 */
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

void SPIDriver::beginTransaction()
{
	/**
	* Set the Slave Select to 0 indicating that the master will be sending
	* data across the SPI line.
	*/
	if (ssLine == SPIDriver::SS_B)
	{	
		PORTB &= ~(1<<PB0);
	}else if (ssLine == SPIDriver::SS_G)
	{
		PORTG &= ~(1<<PG1);
	}
}


void SPIDriver::endTransaction()
{
	/**
	 * Set the Slave Select line to 1 indicating that the master
	 * is finished communicating (release the SPI line)
	 */
	if (ssLine == SPIDriver::SS_B)
	{	
		PORTB |= (1<<PB0);	
	}else if (ssLine == SPIDriver::SS_G)
	{
		PORTG |= (1<<PG1);	
	}
}

int SPIDriver::readInt()
{
	byte highByte = readByte();
	byte lowByte = readByte();
	
	return  ((int) highByte<<8) | lowByte;
}

unsigned int SPIDriver::readUInt()
{
	byte highByte = readByte();
	byte lowByte = readByte();
	//
	//unsigned int hb = 0;
	//unsigned int lb = 0;
	//
	//hb = (unsigned int) highByte;
	//lb = (unsigned int) lowByte;
	//
	//hb = hb << 8;
	//
	//return hb | lb;
	
	unsigned int returnValue = 0;
	
	returnValue = (highByte << 8) | lowByte;
	
	return returnValue;
	
	
//	return  (((unsigned int) highByte)<<8) | lowByte;
}

byte SPIDriver::readByte()
{
	/**
	 * A 0 is put onto the SPI Data register. This is because the device
	 * we are communicating with, uses the same clock cycles that are used
	 * to push each bit in the message we put into the register, to send
	 * this device data. 1 clock cycle used to push data to the device
	 * is also used to push data to THIS device.
	 */
	SPDR = 0;
	
	/**
	 * Wait until the SPI Status Register's
	 * SPI Interrupt flag to be set which is set
	 * when data transfer is complete, and is cleared
	 * when the SPSR register is read.
	 */
	while(!(SPSR & (1<<SPIF)));
	
	/**
	 * Return the value of SPDR because now the data register
	 * contains the data that was sent to us by the device.
	 */
	return SPDR;
}


void SPIDriver::write( byte data )
{
	/**
	 * Write the byte to SPI Data Register
	 */
	SPDR = data;

	/**
	 * Wait until the SPI Status Register's
	 * SPI Interrupt flag to be set which is set
	 * when data transfer is complete, and is cleared
	 * when the SPSR register is read.
	 */
	while(!(SPSR & (1<<SPIF)));
}

void SPIDriver::transactionWrite( byte data )
{
	beginTransaction();
	write(data);
	endTransaction();
}

void SPIDriver::write( byte commandAddress, byte commandValue )
{
	write(commandAddress);
	write(commandValue);
}

void SPIDriver::transactionWrite( byte commandAddress, byte commandValue )
{
	beginTransaction();
	write(commandAddress);
	write(commandValue);
	endTransaction();
}

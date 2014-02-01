#include <avr/io.h>

#include "SPIDriver.h"

using namespace helicopter::drivers;


/**
 * The following table shows the PIN mapping between the various pins
 * necessary for SPI communication, and what the pins map to 
 * as defined in the APM_v252_REALSE.sch document:
 * SPI Function | data direction pin | port pin | Descrip
 * MISO | PB3 | DDB3 | Master In Slave Out - If this device is the master, used for receiving data from the device that is being communicated with
 * MOSI | PB2 | DDB2 | Master Out Slave In - If this device is the master, used for this device to send data to other devices
 * SCK  | PB1 | DDB1 | SPI Clock - 
 * SS   | PB0 | DDB0 | Slave Select
 */
void SPIDriver::init()
{
	/**
	 * Configure the MOSI (DDB2) pin, SCK (DDB1), and SS (DDB0) pins
	 * as output
	 */
	DDRB = (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
	
	
	//wHAT IS THE BELOW CODE FOR? WHY SET SS HIGH, THEN WRITE 0, AND WHAT IS PB1? i think it should be pb2, pb1
	//Set the Slave Select line high, 'freeing' up the SPI line. (This would otherwise default to 0, making this device
	//hold onto the SPI line).
	PORTB |= (1<<DDB0); //Set SS as high /// THIS WAS HERE BUT I DON'T THINK I NEED IT
	//Put the port in a definitive state of 'low' so they can't be 'floating'. MOSI, SPI Clock
	PORTB &= ~((1<<DDB2) | (1<<PB1)); ///THIS WAS THERE BUT I DON'T THINK WE NEED IT.
	
	//The barometer can hold the SPI bus, so we stop it so that we
	//can communicate with the target device.
	//(Only really needs to be executed once - not per init()).
	DDRG |= (1<<DDG1);
	PORTG |= (1<<DDG1);

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
	PORTB &= ~(1<<PB0);
}


void SPIDriver::endTransaction()
{
	/**
	 * Set the Slave Select line to 1 indicating that the master
	 * is finished communicating (release the SPI line)
	 */
	PORTB |= (1<<PB0);	
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

int SPIDriver::readInt16()
{
	byte highByte = readByte();
	byte lowByte = readByte();

	return ((int) highByte<<8) | lowByte;
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

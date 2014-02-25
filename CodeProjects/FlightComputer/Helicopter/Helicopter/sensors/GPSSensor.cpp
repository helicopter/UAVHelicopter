/*
 * GPSSensor.cpp
 *
 * Created: 2/24/2014 8:27:57 PM
 *  Author: HP User
 */ 
#include "GPSSensor.h"

using namespace helicopter::sensors;

bool GPSSensor::readSensor()
{
	return false;
}
				
void GPSSensor::init()
{
	/**
	 * Setup the port settings so that the GPS sends and receives data on UART 2
	 * with a baud rate of 9600
	 */
	//serialDriver->transmit("$PUBX,41,1,0002,0002,9600,1*15\r\n")
	char portSetupPort[] = "$PUBX,41,1,0002,0002,9600,1*15\r\n";
	
	/**
	 * Configure the GPS so we receive the Recommended Minimum Data (RMC) 
	 * message which contains the minimum set of data that we want to use.
	 * (lat, long, etc). Output data on UART 2
	 */
	char portSetupMsgs[] = "$PUBX,40,RMC,0,1,0,0,0,0*47\r\n";
	
	/**
	 * Disable the other messages.
	 */
}
				
bool GPSSensor::isGpsReady()
{
	return false;
}
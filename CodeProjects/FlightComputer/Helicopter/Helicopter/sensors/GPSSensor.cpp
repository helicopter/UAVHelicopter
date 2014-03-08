/*
 * GPSSensor.cpp
 *
 * Created: 2/24/2014 8:27:57 PM
 *  Author: HP User
 */ 
#include <string.h>
#include <util/delay.h>

#include <avr/io.h>

#include "GPSSensor.h"

using namespace helicopter::sensors;

int GPSSensor::readSensor()
{
	int status = 0;

	byte last = 0;
	byte secondToLast = 0;
	
	//Clear out the current msg memory.
	memset(rawGpsMsg, 0, sizeof(rawGpsMsg));
	
	//reset counter back to 0.
	msgBytesRead = 0;
	
	serialDriver->startTimer();
	
	status = serialDriver->transmit("$EIGPQ,RMC*3A\r\n");
	
	
	
	//I'll need to count how many bytes I read to be able to decrypt. 
	
	/**
	 * Keep receiving bytes until the status is an error, or the end of the message
	 * is received (carriage return, line feed)
	 */
	while (status == 0 && !(secondToLast == '\r' && last == '\n'))
	//while (last != '\n')
	{
		secondToLast = last;
		
		status = serialDriver->receive(last);
		
		if (status == 0)
		{
			rawGpsMsg[msgBytesRead++] = last;
			

			if (msgBytesRead >= 150)
			{
				status = -1;
			}			
		}
	}


	serialDriver->stopTimer();
	
	/**
	 * Parse out the lat, long, and status fields. 
	 */
	if (status == 0)
	{
		parseFields(msgBytesRead, rawGpsMsg);
	}


	return status;
}

void GPSSensor::init()
{
	//Link to CRC calculator http://www.hhhh.org/wiml/proj/nmeaxor.html
	
	/**
	 * Setup the port settings so that the GPS sends and receives data on UART 2
	 * with a baud rate of 9600
	 */
	serialDriver->transmit("$PUBX,41,1,0002,0002,9600,1*15\r\n");
	
	/**
	 * Disable all automatic transmission of messages since
	 * we will be polling for data.
	 */
	serialDriver->transmit("$PUBX,40,RMC,0,0,0,0,0,0*47\r\n");
	serialDriver->transmit("$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n");
	serialDriver->transmit("$PUBX,40,VTG,0,0,0,0,0,0*5E\r\n");
	serialDriver->transmit("$PUBX,40,DTM,0,0,0,0,0,0*46\r\n");
	serialDriver->transmit("$PUBX,40,GBS,0,0,0,0,0,0*4D\r\n");
	serialDriver->transmit("$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n");
	serialDriver->transmit("$PUBX,40,GPQ,0,0,0,0,0,0*5D\r\n");
	serialDriver->transmit("$PUBX,40,GRS,0,0,0,0,0,0*5D\r\n");
	serialDriver->transmit("$PUBX,40,GSA,0,0,0,0,0,0*4E\r\n");
	serialDriver->transmit("$PUBX,40,GST,0,0,0,0,0,0*5B\r\n");
	serialDriver->transmit("$PUBX,40,GSV,0,0,0,0,0,0*59\r\n");
	serialDriver->transmit("$PUBX,40,THS,0,0,0,0,0,0*54\r\n");
	
	/**
	 * Data will be received from the GPS before we canceled all the 
	 * message from being auto sent. Therefore, the buffer will
	 * have garbage data on it that needs to be cleared.
	 */
	serialDriver->clearBuffer();
}
				
bool GPSSensor::isGpsReady()
{
	return positionFixStatus == VALID;;
}

void GPSSensor::parseFields( int msgLength, byte *gpsMsg ) 
{
		byte currentByte = 0;
		
		int commaCounter = 0;
		int statusLocation = 0;
		int longitudeLocation = 0;
		int latitudeLocation = 0;
		int longHemisphereLocation = 0;
		int latHemisphereLocation = 0;
		int latLength = 0;
		int longLength = 0;
		
		
		/**
		 * Determine position and length of relevant fields lat/long fields in the message.
		 * The length is important to know because the long and lat fields
		 * might not be a fixed number of bytes.
		 * Status is after the second comma
		 * Latitude is after the 3rd comma (Degrees + minutes)
		 * Latitude N/S indicator is after the 4th comma
		 * Longitude is after the 5th comma
		 * Longitude E/W indicator is after the 6th comma
		 */
		for (int byteLocation = 0; byteLocation < msgLength; byteLocation++)
		{
			//read the current byte
			currentByte = gpsMsg[byteLocation];
			
			//if the comma count is one we care about, then store the current byteLocation  as that fields location.
			//if the comma count is one greater than a comma we cared about then store the message length in the appropriate lat/long message length field
			//increase comma count, reset temp msg length then loop
			if (currentByte == ',')
			{
				commaCounter++;
				
				switch (commaCounter)
				{
					/**
					 * Start of a message we care about
					 */
					case STATUS_COMMA_LOCATION:
						statusLocation = byteLocation + 1;
					break;
					case LAT_COMMA_LOCATION:
						latitudeLocation = byteLocation + 1;
					break;
					case LONG_COMMA_LOCATION:
						longitudeLocation = byteLocation + 1;
					break;
					case LAT_HEMI_COMMA_LOCATION:
						latHemisphereLocation = byteLocation + 1;
						
						//Calculate how many characters are assigned to lat. subtract one to account for the comma.
						latLength = byteLocation- latitudeLocation;
					break;
					case LONG_HEMI_COMMA_LOCATION:
						longHemisphereLocation = byteLocation + 1;
						
						//Calculate how many characters are assigned to long
						longLength = byteLocation- longitudeLocation;						
					break;					
				}
			}
		}
		
		

		
		//Parse the status field
		byte statusByte = gpsMsg[statusLocation];
		
		if (statusByte == 'A')
		{
			positionFixStatus = VALID;
		}else // 'V', navigation receiver warning
		{
			positionFixStatus = INVALID;
		}

		//Only parse the data if the gps data is valid
		if (positionFixStatus == VALID)
		{

			long temp = 0;
			
			/**
			 * Transform relevant fields to usable data types based on their location and length.
			 * But only update the lat/long values if a lat/long value is available. If the message mostly
			 * contains ',' without lat long values, then the parser will return 0. 
			 */			
			if ((temp = parseDegrees(true, gpsMsg, latHemisphereLocation, latitudeLocation, latLength)) != 0)
			{
				latitude = temp;
			}
			
			temp = 0;
			
			if ((temp = parseDegrees(false, gpsMsg, longHemisphereLocation, longitudeLocation, longLength)) != 0)
			{
				longitude = temp;
			}
		}
}

float GPSSensor::convertFromNMEAToDegrees( float NMEACoordinate, char hemisphere ) 
{
	//Convert lat long format from NMEA format, to Degrees
	int degrees = (int) (NMEACoordinate / 100);
	
	return (degrees + ((NMEACoordinate - degrees * 100)/60)) * (hemisphere == 'N' ? 0 : -1);
}

long GPSSensor::parseDegrees( bool isLatitude, byte * gpsMsg, int hemisphereLocation, int variableLocation, int variableLength) 
{
	int numOfMaxDegreeCharacters = isLatitude ? 2 : 3;
	
	char currentChar = 0;
	long degrees = 0;
	int index = variableLocation;
		
	//Pull out the hemisphere location
	char hemisphere = gpsMsg[hemisphereLocation];
		
	//pull out the degrees
	for (int degreesIndex = 0; degreesIndex < numOfMaxDegreeCharacters; degreesIndex++)
	{
		currentChar = gpsMsg[index++];
		
		//If the character is a digit, append it to degrees.
		if (currentChar >= '0' && currentChar <= '9')
		{
			degrees = degrees * 10 + (currentChar - '0');
		}
	}
		
		
	long minutes = 0;
		
	//max num of minutes characters is the length of the message, minutes the 
	//num of degrees, minus the 1 for the '-' sign if there is one
	int numOfMaxMinutesCharacters = variableLength - numOfMaxDegreeCharacters;
	int numberOfShifts = 7; //Number of multiplications by 10 that need to be made
	int shiftIdx = 0;
		
	while (shiftIdx < numberOfShifts)
	{
		//If the shift index is less then the number of characters in the minutes field,
		//then pull out the current character and process it. Otherwise, just shift the field by
		//multiplying by 10.
		if (shiftIdx < numOfMaxMinutesCharacters)
		{
			currentChar = gpsMsg[index++];
			
			//If the field isn't a period, the process the character and shift the fields
			//Don't increase the shift index if it is a '.' because we skip the character.
			if (currentChar >= '0' && currentChar <= '9')
			{
				degrees = degrees * 10;
				minutes = minutes * 10 + (currentChar - '0');
				
				shiftIdx++;
			}	
		}else
		{
			degrees = degrees * 10;
			minutes = minutes * 10;	
				
			shiftIdx++;		
		}
	}
		
	//Convert the minutes into degrees, and add them to the degrees value.
	degrees = ((degrees + ((long)(minutes / .60f))));
		
	/**
	* If the hemisphere is in the western or southern hemisphere, then the degree value will be negative.
	*/
	if (hemisphere == 'W' || hemisphere == 'S')
	{
		degrees *= -1;
	}
		
	return degrees;
}

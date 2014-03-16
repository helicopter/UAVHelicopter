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


/**
 * Ublox's U-Center application was used to generate these messages
 */
const byte GPSSensor::NAV_POSLLH_POLLMSG[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};

const byte GPSSensor::NAV_POSECEF_POLLMSG[] = {0xB5, 0x62, 0x01, 0x01, 0x00, 0x00, 0x02, 0x07};
	
const byte GPSSensor::NAV_STATUS_POLLMSG[] = {0xB5, 0x62, 0x01, 0x03, 0x00, 0x00, 0x04, 0x0D};	
	
const byte GPSSensor::CFG_PRT[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};
	

int GPSSensor::readSensor(byte *pollMsg, int pollMsgSize, byte *msgData, int msgDataSize )
{
	int status = 0;
	
	//Clear the serial driver's buffer of any existing data.
	serialDriver->clearBuffer();
	
	//Start timer
	serialDriver->startTimer();
	
	
	//Send poll command
	status = serialDriver->transmit((const char*)pollMsg, pollMsgSize);
	
	byte b = 0;
	
	/**
	 * The message that is being sent will have the same header and ID fields of the message
	 * that should be received. So convert the header and ID field into a long to be compared
	 * against the incoming header and ID field.
	 */
	unsigned long desiredHeaderID = (unsigned long) pollMsg[0] << 24 | 
									(unsigned long) pollMsg[1] << 16 | 
									(unsigned long) pollMsg[2] << 8 | 
									(unsigned long) pollMsg[3];
	unsigned long currentFourBytes = 0;
	
	/*
	* loop until the header/id is found, or a timeout occurs. 
	* This prevents synchronization issues where if a message transmission to the
	* GPS was canceled half way, the next message send would result in
	* an nack half way through the next message transmission, so you have
	* to ensure that the message that is being read, is the desired message.
	*/
	while (status == 0 && currentFourBytes != desiredHeaderID)
	{
		status = serialDriver->receive(b);
		currentFourBytes = currentFourBytes << 8 | b;
	}
	
	/**
	 * Populate the message array with the first four bytes.
	 */
	msgData[0] = 0xFF & (currentFourBytes >> 24);
	msgData[1] = 0xFF & (currentFourBytes >> 16);
	msgData[2] = 0xFF & (currentFourBytes >> 8);
	msgData[3] = 0xFF & currentFourBytes;
	
	//while loop for reading data if status == 0
	for (int i = 4; i < msgDataSize && status == 0; i++)
	{
		status = serialDriver->receive(b);
		
		msgData[i] = b;
	}
	

	//Stop timer
	serialDriver->stopTimer();
	
	return status;
}



int GPSSensor::readSensorNavStatus()
{
	int status = 0;
	
	byte statusMsg[22] = {0};
	
	status = readSensor((byte *)NAV_STATUS_POLLMSG, sizeof(NAV_STATUS_POLLMSG), statusMsg, sizeof(statusMsg));
	
	if (status == 0)
	{
		//parse results
		positionFixStatus =  statusMsg[10] >= 3 ? VALID : INVALID;
	}

	return status;
}

int GPSSensor::readSensorECEF()
{
	int status = 0;
	
	byte ecefMsg[26] = {0};
	
	status = readSensor((byte *)NAV_POSECEF_POLLMSG, sizeof(NAV_POSECEF_POLLMSG), ecefMsg, sizeof(ecefMsg));
	
	if (status == 0)
	{
		//parse results
		xEcefCm =  ((long)ecefMsg[13] << 24) | ((long)ecefMsg[12] << 16) | ((long)ecefMsg[11] << 8) | (long)ecefMsg[10];
		yEcefCm =  ((long)ecefMsg[17] << 24) | ((long)ecefMsg[16] << 16) | ((long)ecefMsg[15] << 8) | (long) ecefMsg[14];
		zEcefCm =  ((long)ecefMsg[21] << 24) | ((long)ecefMsg[20] << 16) | ((long)ecefMsg[19] << 8) | (long) ecefMsg[18];
		positionAccuracyEstimateEcefCm =  ((long)ecefMsg[25] << 24) | ((long)ecefMsg[24] << 16) | ((long)ecefMsg[23] << 8) | (long) ecefMsg[22];
	}

	return status;
}

int GPSSensor::readSensorLLH()
{
	int status = 0;
	
	byte llhMsg[34] = {0};
	
	status = readSensor((byte *)NAV_POSLLH_POLLMSG, sizeof(NAV_POSLLH_POLLMSG), llhMsg, sizeof(llhMsg));

	if (status == 0)
	{
		//parse results
		longitudeDegE7 =  ((long)llhMsg[13] << 24) | ((long)llhMsg[12] << 16) | ((long)llhMsg[11] << 8) | (long)llhMsg[10];
		latitudeDegE7 =  ((long)llhMsg[17] << 24) | ((long)llhMsg[16] << 16) | ((long)llhMsg[15] << 8) | (long) llhMsg[14];		
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
	//serialDriver->transmit("$PUBX,41,1,0002,0002,38400,1*25\r\n");
	//serialDriver->transmit("$PUBX,41,1,0002,0002,9600,1*15\r\n");
	
	/**
	 * Disable all automatic transmission of messages since
	 * we will be polling for data.
	 */
	
	/*
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
	*/
	
	/**
	* Configure the GPS's port to use, protocol, and speed.
	*/
	serialDriver->transmit((const char*)CFG_PRT, sizeof(CFG_PRT));
	
	
	
	/**
	 * Data will be received from the GPS before we canceled all the 
	 * message from being auto sent. Therefore, the buffer will
	 * have garbage data on it that needs to be cleared.
	 */
	_delay_ms(50);
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
				latitudeDegE7 = temp;
			}
			
			temp = 0;
			
			if ((temp = parseDegrees(false, gpsMsg, longHemisphereLocation, longitudeLocation, longLength)) != 0)
			{
				longitudeDegE7 = temp;
			}
		}
}

float GPSSensor::convertFromNMEAToDegrees( float NMEACoordinate, char hemisphere ) 
{
	//Convert lat long format from NMEA format, to Degrees
	int degrees = (int) (NMEACoordinate / 100);
	
	return (degrees + ((NMEACoordinate - degrees * 100)/60)) * (hemisphere == 'N' ? 0 : -1);
}

int64_t GPSSensor::parseDegrees( bool isLatitude, byte * gpsMsg, int hemisphereLocation, int variableLocation, int variableLength) 
{
	int numOfMaxDegreeCharacters = isLatitude ? 2 : 3;
	
	char currentChar = 0;
	int64_t degrees = 0;
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
		
		
	int64_t minutes = 0;
		
	//max num of minutes characters is the length of the message, minutes the 
	//num of degrees, minus the 1 for the '-' sign if there is one
	int numOfMaxMinutesCharacters = variableLength - numOfMaxDegreeCharacters;
	int shiftIdx = 0;
		
	while (shiftIdx < LATLONG_SIGNIFICANT_DIGITS_SHIFTED)
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
	degrees = ((degrees + ((int64_t)(minutes / .60f))));
		
	/**
	* If the hemisphere is in the western or southern hemisphere, then the degree value will be negative.
	*/
	if (hemisphere == 'W' || hemisphere == 'S')
	{
		degrees *= -1;
	}
		
	return degrees;
}

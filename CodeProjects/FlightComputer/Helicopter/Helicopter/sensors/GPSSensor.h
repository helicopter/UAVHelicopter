/*
 * GPSSensor.h
 *
 * Created: 2/24/2014 8:20:34 PM
 *  Author: HP User
 */ 


#ifndef GPSSENSOR_H_
#define GPSSENSOR_H_

#include "SerialDriver.h"

using namespace helicopter::drivers;


namespace helicopter
{
	namespace sensors
	{
		/**
		 * Class for reading the Ublox Lea-6h GPS.
		 * U-Center was used to generate messages to send to GPS
		 */
		class GPSSensor
		{
			public: 
				enum FixStatus {INVALID = 1, VALID = 2};
					
				/**
				 * Number of multiplications by 10 (10^x) that are applied to the
				 * lat/long fields to shift the significant digits to transform the floating
				 * point value into a long.
				 */
				static const int LATLONG_SIGNIFICANT_DIGITS_SHIFTED = 8;
						
			private:
				static const int STATUS_COMMA_LOCATION = 2;
				static const int LAT_COMMA_LOCATION = 3;
				static const int LONG_COMMA_LOCATION = 5;
				static const int LAT_HEMI_COMMA_LOCATION = LAT_COMMA_LOCATION + 1;
				static const int LONG_HEMI_COMMA_LOCATION = LONG_COMMA_LOCATION + 1;
				
				static const int GPS_MSG_BUFFER_SIZE = 150;
				
				/**
				 * UBX message for polling lat, long, height
				 */
				static const byte NAV_POSLLH_POLLMSG[];
				
				static const byte NAV_POSECEF_POLLMSG[];
				
				static const byte NAV_STATUS_POLLMSG[];
				
				SerialDriver *serialDriver;

				byte rawGpsMsg[GPS_MSG_BUFFER_SIZE];
				
				int msgBytesRead;
				
				/*
				int64_t latitude;
				int64_t longitude;
				*/
				
				//latitude in degrees
				long latitudeDegE7;
				long longitudeDegE7;
				
				long xEcefCm;
				long yEcefCm;
				long zEcefCm;
				unsigned long positionAccuracyEstimateEcefCm;
				
				FixStatus positionFixStatus;
				
				int64_t parseDegrees( bool isLatitude, byte * gpsMsg, int hemisphereLocation, int variableLocation, int variableLength);
				
				int readSensor(byte *pollMsg, int pollMsgSize, byte *msgData, int msgDataSize );
				
			public:
				GPSSensor(SerialDriver *serialDriver): 
					serialDriver (serialDriver),
					msgBytesRead(0),
					latitudeDegE7 (0),
					longitudeDegE7 (0),
					xEcefCm (0),
					yEcefCm (0),
					zEcefCm (0),
					positionAccuracyEstimateEcefCm (0),
					positionFixStatus(INVALID)
				{
					memset(rawGpsMsg, 0, sizeof(rawGpsMsg));
				}
				
				
				/**
				 * Reads the sensor values from the sensor and converts the values into
				 * usable lat, long formats
				 */
				//int readSensor();
				
				
				
				
				/**
				 * Reads the sensors latitude, longitude and height (NAV-POSLLH)
				 */
				int readSensorLLH();
				
				
				/**
				 * Reads the sensors x, y, z position in ECEF (NAV-POSECEF)
				 */
				int readSensorECEF();
				
				/**
				 * Reads the navigation status on the sensor (NAV-STATUS)
				 * If GPS fields >= 0x03 (3d fix) then fix status = valid.
				 */
				int readSensorNavStatus();
				
				/**
				 * Configures and initializes the GPS
				 */
				void init();
				
				bool isGpsReady();
				
				
				int getMsgBytesRead()
				{
					return msgBytesRead;
				}
				
				//This is obviously very dangerous. Don't delete this data. 
				byte * getRawMsg()
				{
					return rawGpsMsg;
				}
				
				/*
				int64_t getLatitude()
				{
					return latitude;
				}
				
				int64_t getLongitude()
				{
					return longitude;
				}
				*/
				/**
				 * Latitude in degrees X 10^7
				 */

				long getLatitudeDegE7()
				{
					return latitudeDegE7;
				}
				
				/**
				 * Longitude in degrees X 10^7
				 */				
				long getLongitudeDegE7()
				{
					return longitudeDegE7;
				}			
					
				long getXEcefCm()
				{
					return xEcefCm;
				}

				long getYEcefCm()
				{
					return yEcefCm;
				}
				
				long getZEcefCm()
				{
					return zEcefCm;
				}							

				unsigned long getPositionAccuracyEstimateEcefCm()
				{
					return positionAccuracyEstimateEcefCm;
				}
								
				FixStatus getPositionFixStatus()
				{
					return positionFixStatus;
				}
				
				void parseFields( int msgLength, byte *gpsMsg );
				
				//TODO: Delete this method since it's not used anymore. But delete it after checkin so i've got a record of it
				//incase i want to use it again.
				float convertFromNMEAToDegrees( float NMEACoordinate, char hemisphere );			
		};
	}
}




#endif /* GPSSENSOR_H_ */
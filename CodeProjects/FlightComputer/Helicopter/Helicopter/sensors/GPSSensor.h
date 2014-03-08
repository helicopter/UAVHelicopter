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
		class GPSSensor
		{
			public: 
				enum FixStatus {INVALID = 1, VALID = 2};
						
			private:
				static const int STATUS_COMMA_LOCATION = 2;
				static const int LAT_COMMA_LOCATION = 3;
				static const int LONG_COMMA_LOCATION = 5;
				static const int LAT_HEMI_COMMA_LOCATION = LAT_COMMA_LOCATION + 1;
				static const int LONG_HEMI_COMMA_LOCATION = LONG_COMMA_LOCATION + 1;
				
				static const int GPS_MSG_BUFFER_SIZE = 150;
				
				SerialDriver *serialDriver;

				byte rawGpsMsg[GPS_MSG_BUFFER_SIZE];
				
				int msgBytesRead;
				
				long latitude;
				long longitude;
				
				FixStatus positionFixStatus;
				
				long parseDegrees( bool isLatitude, byte * gpsMsg, int hemisphereLocation, int variableLocation, int variableLength);
				
			public:
				GPSSensor(SerialDriver *serialDriver): 
					serialDriver (serialDriver),
					msgBytesRead(0),
					latitude (0),
					longitude (0),
					positionFixStatus(INVALID)
				{
					memset(rawGpsMsg, 0, sizeof(rawGpsMsg));
				}
				
				
				/**
				 * Reads the sensor values from the sensor and converts the values into
				 * usable lat, long formats
				 */
				int readSensor();
				
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
				
				long getLatitude()
				{
					return latitude;
				}
				
				long getLongitude()
				{
					return longitude;
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
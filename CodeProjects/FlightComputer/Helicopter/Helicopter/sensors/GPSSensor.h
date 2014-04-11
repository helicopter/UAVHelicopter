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
					
			private:
				
				/**
				 * UBX message for polling lat, long, height
				 */
				static const byte NAV_POSLLH_POLLMSG[];
				
				static const byte NAV_SOL_POLLMSG[];
				
				static const byte NAV_POSECEF_POLLMSG[];
				
				static const byte NAV_STATUS_POLLMSG[];

				static const byte CFG_PRT[];

				static const byte CFG_RATE[];				
				
				SerialDriver *serialDriver;
				
				//latitude in degrees * 10^7
				long latitudeDegE7;
				long longitudeDegE7;
				
				long xEcefCm;
				long yEcefCm;
				long zEcefCm;
				unsigned long positionAccuracyEstimateEcefCm;
				
				long xVEcefCms;
				long yVEcefCms;
				long zVEcefCms;
				unsigned long velocityAccuracyEstimateEcefCms;
				
				FixStatus positionFixStatus;
				
				int64_t parseDegrees( bool isLatitude, byte * gpsMsg, int hemisphereLocation, int variableLocation, int variableLength);
				
				int readSensor(byte *pollMsg, int pollMsgSize, byte *msgData, int msgDataSize );
				
				/**
				 * Reads data from the GPS.
				 */
				int receiveGpsData(unsigned long desiredHeaderID, byte *msgData, int msgDataSize); 
				
			public:
				GPSSensor(SerialDriver *serialDriver): 
					serialDriver (serialDriver),
					latitudeDegE7 (0),
					longitudeDegE7 (0),
					xEcefCm (0),
					yEcefCm (0),
					zEcefCm (0),
					positionAccuracyEstimateEcefCm (0),
					xVEcefCms(0),
					yVEcefCms(0),
					zVEcefCms(0),
					velocityAccuracyEstimateEcefCms (0),
					positionFixStatus(INVALID)
				{
				}
				
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
				 * Reads the NAV-SOL (navigation solution information) message from the GPS.
				 * This includes position, and velocity information in ECEF as well as status info.
				 */
				int readSensorSolution();
				
				int readSensorSolutionSendCommand();
				int readSensorSolutionReadData();
				
				/**
				 * Configures and initializes the GPS
				 */
				int init();
				
				bool isGpsReady();
				
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
				
				
				
				long getXVEcefCms()
				{
					return xVEcefCms;
				}

				long getYVEcefCms()
				{
					return yVEcefCms;
				}
				
				long getZVEcefCms()
				{
					return zVEcefCms;
				}

				unsigned long getVelocityAccuracyEstimateEcefCm()
				{
					return velocityAccuracyEstimateEcefCms;
				}				
								
				FixStatus getPositionFixStatus()
				{
					return positionFixStatus;
				}
		};
	}
}




#endif /* GPSSENSOR_H_ */
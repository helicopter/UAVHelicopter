/*
 * GPSSensor.cpp
 *
 * Created: 2/24/2014 8:27:57 PM
 *  Author: HP User
 */ 
#include <string.h>
#include <util/delay.h>

#include "GPSSensor.h"

using namespace helicopter::sensors;


/**
 * Ublox's U-Center application was used to generate these messages
 */
const byte GPSSensor::NAV_POSLLH_POLLMSG[] = {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};

const byte GPSSensor::NAV_SOL_POLLMSG[] = {0xB5, 0x62, 0x01, 0x06, 0x00, 0x00, 0x07, 0x016};

const byte GPSSensor::NAV_POSECEF_POLLMSG[] = {0xB5, 0x62, 0x01, 0x01, 0x00, 0x00, 0x02, 0x07};
	
const byte GPSSensor::NAV_STATUS_POLLMSG[] = {0xB5, 0x62, 0x01, 0x03, 0x00, 0x00, 0x04, 0x0D};	
			
/**
* Target=uart1, protocolin = ubx, protocolout = ubx, baud = 9600
*/
const byte GPSSensor::CFG_PRT[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};

/**
 * Configure gps for 5hz rate
 */
const byte GPSSensor::CFG_RATE[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};
	

int GPSSensor::receiveGpsData(unsigned long desiredHeaderID, byte * msgData, int msgDataSize ) 
{
	int status = 0;
	byte b = 0;
	
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
	
	if (status == 0)
	{
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
	}	
		
	return status;
}

int GPSSensor::readSensor(byte *pollMsg, int pollMsgSize, byte *msgData, int msgDataSize )
{
	int status = 0;
	
	//Clear the serial driver's buffer of any existing data.
	//serialDriver->clearBuffer();
	
	//Start timer
	serialDriver->startTimer();
	
	
	//Send poll command
	status = serialDriver->transmit((const char*)pollMsg, pollMsgSize);
	
	if (status == 0)
	{
		/**
		 * The message that is being sent will have the same header and ID fields of the message
		 * that should be received. So convert the header and ID field into a long to be compared
		 * against the incoming header and ID field.
		 */
		unsigned long desiredHeaderID = (unsigned long) pollMsg[0] << 24 | 
										(unsigned long) pollMsg[1] << 16 | 
										(unsigned long) pollMsg[2] << 8 | 
										(unsigned long) pollMsg[3];
										
		status = receiveGpsData(desiredHeaderID, msgData, msgDataSize);

	}
	

	//Stop timer
	serialDriver->stopTimer();
	
	return status;
}


int GPSSensor::readSensorSolution()
{
	int status = 0;
	
	byte navSolMsg[60] = {0};
	
	status = readSensor((byte *)NAV_SOL_POLLMSG, sizeof(NAV_SOL_POLLMSG), navSolMsg, sizeof(navSolMsg));
	
	if (status == 0)
	{
		//Parse status info 
		positionFixStatus =  navSolMsg[16] >= 3 ? VALID : INVALID;
		
		//Parse position info
		xEcefCm =  ((long)navSolMsg[21] << 24) | ((long)navSolMsg[20] << 16) | ((long)navSolMsg[19] << 8) | (long) navSolMsg[18];
		yEcefCm =  ((long)navSolMsg[25] << 24) | ((long)navSolMsg[24] << 16) | ((long)navSolMsg[23] << 8) | (long) navSolMsg[22];
		zEcefCm =  ((long)navSolMsg[29] << 24) | ((long)navSolMsg[28] << 16) | ((long)navSolMsg[27] << 8) | (long) navSolMsg[26];
		positionAccuracyEstimateEcefCm =  ((long)navSolMsg[33] << 24) | ((long)navSolMsg[32] << 16) | ((long)navSolMsg[31] << 8) | (long) navSolMsg[30];
		
		//Parse velocity info
		xVEcefCms =  ((long)navSolMsg[37] << 24) | ((long)navSolMsg[36] << 16) | ((long)navSolMsg[35] << 8) | (long) navSolMsg[34];
		yVEcefCms =  ((long)navSolMsg[41] << 24) | ((long)navSolMsg[40] << 16) | ((long)navSolMsg[39] << 8) | (long) navSolMsg[38];
		zVEcefCms =  ((long)navSolMsg[45] << 24) | ((long)navSolMsg[44] << 16) | ((long)navSolMsg[43] << 8) | (long) navSolMsg[42];
		velocityAccuracyEstimateEcefCms =  ((long)navSolMsg[49] << 24) | ((long)navSolMsg[48] << 16) | ((long)navSolMsg[47] << 8) | (long) navSolMsg[46];
		
	}

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

int GPSSensor::init()
{
	int status = 0;
	byte ack[10] = {0xB5, 0x62, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	/**
	* Configure the GPS's port to use, protocol, and speed.
	*/
	serialDriver->transmit((const char*)CFG_PRT, sizeof(CFG_PRT));
	
	/**
	 * Read and discard the ack received from the cfg message. We need to read each
	 * byte and discard each byte of the ack message because if we don't read ALL the bytes
	 * the GPS will be completely unresponsive. It will not receive other messages
	 * unless all bytes of the ack message are received. 
	 */
	unsigned long desiredHeaderID = (unsigned long) ack[0] << 24 |
			(unsigned long) ack[1] << 16 |
			(unsigned long) ack[2] << 8 |
			(unsigned long) ack[3];
			
	status = receiveGpsData(desiredHeaderID, ack, sizeof(ack)); 
	
	serialDriver->transmit((const char*)CFG_RATE, sizeof(CFG_RATE));
	
	status |= receiveGpsData(desiredHeaderID, ack, sizeof(ack)); 
	
	return status;
}
				
bool GPSSensor::isGpsReady()
{
	return positionFixStatus == VALID;;
}

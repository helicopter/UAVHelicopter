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

const byte GPSSensor::NAV_SOL_POLLMSG[] = {0xB5, 0x62, 0x01, 0x06, 0x00, 0x00, 0x07, 0x016};

const byte GPSSensor::NAV_POSECEF_POLLMSG[] = {0xB5, 0x62, 0x01, 0x01, 0x00, 0x00, 0x02, 0x07};
	
const byte GPSSensor::NAV_STATUS_POLLMSG[] = {0xB5, 0x62, 0x01, 0x03, 0x00, 0x00, 0x04, 0x0D};	
	
//msg to periodically receive nav_sol messages.
const byte GPSSensor::CFG_MSG_CONFIG_PERIODIC_SOL[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x17, 0xDA};

byte GPSSensor::navSolMsgBuffer[60] = {};
	
int GPSSensor::navSolBufferCounter = 0;

bool GPSSensor::navSolMsgReceived = false;
			
/**
* Target=uart1, protocolin = ubx, protocolout = ubx, baud = 9600
*/
const byte GPSSensor::CFG_PRT[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};

/**
 * Configure gps for 5hz rate
 */
const byte GPSSensor::CFG_RATE[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};
	
/**
 * Resets the gps with hotstart, controlled (watchdog) hardware reset.
 */
const byte GPSSensor::CFG_RST[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x12, 0x6C};
	

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


int GPSSensor::readSensorSolutionSendCommand()
{
	int status = 0;
	

	//Start timer
	serialDriver->startTimer();
	
	//Send poll command
	status = serialDriver->transmit((const char*)NAV_SOL_POLLMSG, sizeof(NAV_SOL_POLLMSG));
	
	//Stop timer
	serialDriver->stopTimer();
	
	return status;
}


int GPSSensor::readSensorSolutionReadData()
{
	int status = 0;
	
	byte navSolMsg[60] = {0};
	

	/**
	* The message that is being sent will have the same header and ID fields of the message
	* that should be received. So convert the header and ID field into a long to be compared
	* against the incoming header and ID field.
	*/
	unsigned long desiredHeaderID = (unsigned long) NAV_SOL_POLLMSG[0] << 24 | 
									(unsigned long) NAV_SOL_POLLMSG[1] << 16 | 
									(unsigned long) NAV_SOL_POLLMSG[2] << 8 | 
									(unsigned long) NAV_SOL_POLLMSG[3];
										
										
	//Start timer
	serialDriver->startTimer();
											
	status = receiveGpsData(desiredHeaderID, navSolMsg, sizeof(navSolMsg));

	//Stop timer
	serialDriver->stopTimer();
		
	
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


void GPSSensor::processSensorSolution()
{
	
	if (navSolMsgReceived)
	{
		//TODO: Do a checksum check on the data.
				
		byte navSolMsg[navSolBufferSize] = {0};
			
		//Copy the nav solution data into a local buffer in case an
		//interrupt occurs during processing.
		memcpy(navSolMsg,navSolMsgBuffer, navSolBufferSize);
		
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
		
		navSolMsgReceived = false;
	}
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
	/**
	 * The status field gets all out of wack during gps init. The hard
	 */
	int status = 0;
	byte ack[10] = {0xB5, 0x62, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		
	
	
	/**
	 * Transmit a hotstart reset and then clear the buffers so that if the gps is still periodically
	 * sending data, it doesn't cause an overflow of the serial drivers buffer. 
	 */
	serialDriver->clearBuffer();
	serialDriver->transmit((const char*)CFG_RST, sizeof(CFG_RST));
	
	unsigned long desiredHeaderID = (unsigned long) ack[0] << 24 |
	(unsigned long) ack[1] << 16 |
	(unsigned long) ack[2] << 8 |
	(unsigned long) ack[3];
		
	/**
	 * There might have been old overflowed data on the buffer so keep reading
	 * until we receive the ack.
	 */
	while(receiveGpsData(desiredHeaderID, ack, sizeof(ack)) != 0)
	{
		
	}
	
	_delay_ms(750); //wait for reset
	
	
	/**
	* Configure the GPS's port to use, protocol, and speed.
	*/
	serialDriver->transmit((const char*)CFG_PRT, sizeof(CFG_PRT));
	
	/**
	 * Read and discard the ack received from the cfg message. We need to read each
	 * byte and discard each byte of the ack message because if we don't read ALL the bytes
	 * the GPS will be completely unresponsive. It will not receive other messages
	 * unless all bytes of the ack message are received. 
	 * might fail to receive ack if there was a previous buffer overrun.
	 */
	while(receiveGpsData(desiredHeaderID, ack, sizeof(ack)) != 0)
	{
		
	}
	
	serialDriver->transmit((const char*)CFG_RATE, sizeof(CFG_RATE));
	status = receiveGpsData(desiredHeaderID, ack, sizeof(ack)); 

	/**
	* Hack to setup the serial driver to interrupt when data is received.
	*/
	UCSR1B |= (1<<RXCIE1);
	
//	sei();
	
	/**
	* Setup gps for polling navigation solution.
	*/
	serialDriver->transmit((const char*)CFG_MSG_CONFIG_PERIODIC_SOL, sizeof(CFG_MSG_CONFIG_PERIODIC_SOL));
		
	status |= receiveGpsData(desiredHeaderID, ack, sizeof(ack));

	return status;
}
				
bool GPSSensor::isGpsReady()
{
	return positionFixStatus == VALID;;
}




//ISR for receiving serial data from the gps when an interrupt occurs
ISR(USART1_RX_vect)
{

	byte msgByte = UDR1;
	

	if (GPSSensor::navSolBufferCounter < GPSSensor::navSolBufferSize)
	{			
			
		
		bool isCorrupted = false;
		
		/**
		* if the first data values don't match header values and id values, then the msg has been corrupted
		* so ignore the message. 0xB5 is the first header byte, 0x62 is the second header byte,
		* 0x01 is the first ID byte for the nav_sol msg and 0x06 is the second header byte.
		*/
		if ((GPSSensor::navSolBufferCounter == 0 && msgByte != 0xB5) ||
			(GPSSensor::navSolBufferCounter == 1 && msgByte != 0x62) ||
			(GPSSensor::navSolBufferCounter == 2 && msgByte != 0x01) || 
			(GPSSensor::navSolBufferCounter == 3 && msgByte != 0x06))
			
		{
			isCorrupted = true;
		}
		
		if (isCorrupted)
		{
			GPSSensor::navSolMsgReceived = false;
			GPSSensor::navSolBufferCounter = 0;
		}else
		{
			GPSSensor::navSolMsgBuffer[GPSSensor::navSolBufferCounter++] = msgByte;
			
			if (GPSSensor::navSolBufferCounter >= GPSSensor::navSolBufferSize)
			{
				GPSSensor::navSolMsgReceived = true;
				GPSSensor::navSolBufferCounter = 0;
			}			
		}
	}else
	{
		//Theres been some kind of error and the buffer overran, so reset.
		GPSSensor::navSolMsgReceived = false;
		GPSSensor::navSolBufferCounter = 0;
	}


	
	//I'll want to make sure I do a memcpy on the buffer when i start working on it
	//to ensure it's not adjusted while I'm working on it.
	//and I'll want to set variable to false first.
}
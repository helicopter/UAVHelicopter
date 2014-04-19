/*
 * NavigationPrototypeTests.cpp
 *
 * Created: 3/18/2014 6:52:51 PM
 *  Author: HP User
 */ 

#include <util/delay.h>

#include "NavigationPrototypeTests.h"
#include "SPIDriver.h"
#include "SerialDriver.h"
#include "UnitTestUtils.h"
#include "Timer.h"
#include "IMUSensor.h"
#include "GPSSensor.h"
#include "TWIDriver.h"
#include "MagnetometerSensor.h"
#include "AHRS.h"
#include "CoordinateUtil.h"


using namespace helicopter::drivers;
using namespace helicopter::util;
using namespace helicopter::sensors;
using namespace helicopter::util;
using namespace helicopter::navigation;



//TODO:
//Check the timing of this function, can it keep up with adequate speed?
//(assuming it didn't use serial communication every iteration)
//Perhaps even remove the delay since serial driver will be slow enough.

int ahrs_test(TestCase *test)
{
	//Create AHRS. 98f is the sampling frequency of the gyroscope
	float gyroscopeSamplingFrequency = 98.0f;
	//float accelerometerSamplingFrequency = 94.0f;
	
	//float accelFreqSquared = accelerometerSamplingFrequency * accelerometerSamplingFrequency;

	//float complementaryFilterAlpha = 1.0f;
	//float complimentaryFilterBeta = 1.0f;
	//float velocityX = 0.0f;
	
	
	float velocityBodyFrame[3] = {0};
	
	
	
	
	AHRS *ahrs = new AHRS(1/gyroscopeSamplingFrequency);
	
	
	
	
	
	Timer *gpsTimer = new Timer(F_CPU, PRESCALE_BY_TENTWENTYFOUR, 500);
	
	
	SerialDriver *gpsSerialDriver = new SerialDriver(9600, SerialDriver::One, true, gpsTimer);
	gpsSerialDriver->init();
		
	//Create objects for reading sensors
	SPIDriver *spiDriver = new SPIDriver();
	spiDriver->init();
	
	TWIDriver *twDriver = new TWIDriver();
	
	SerialDriver *serialDriver = new SerialDriver(115200, SerialDriver::Zero, true, NULL);
	serialDriver->init();
	
	GPSSensor *gpsSensor = new GPSSensor(gpsSerialDriver);
	gpsSensor->init();
		
	MagnetometerSensor *magnetometerSensor = new MagnetometerSensor(twDriver);
	magnetometerSensor->init();
	
	IMUSensor *imuSensor = new IMUSensor(spiDriver);
	imuSensor->init();
	
	
	while (!gpsSensor->isGpsReady())
	{
		gpsSensor->readSensorSolution();
	}
	
	while (gpsSensor->getXEcefCm() == 0 || gpsSensor->getYEcefCm() == 0 || gpsSensor->getZEcefCm() == 0)
	{
		gpsSensor->readSensorSolution();
	}
		
	gpsSensor->readSensorLLH();
		
	/**
	 * calculate eceftoned rotation matrix and set initial ECEF values
	 */
	float EcefToLocalNEDRotationMatrix[3][3];
	memset(EcefToLocalNEDRotationMatrix, 0, 3 * 3 * sizeof(float));
	
	float latitude = (float) gpsSensor->getLatitudeDegE7() / 10000000.0f;
	float longitude = (float) gpsSensor->getLongitudeDegE7() / 10000000.0f;

	CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(latitude, longitude, EcefToLocalNEDRotationMatrix);

	/*
	long initialXPositionEcef = gpsSensor->getXEcefCm();
	long initialYPositionEcef = gpsSensor->getYEcefCm();
	long initialZPositionEcef = gpsSensor->getZEcefCm();	
	*/
	
	
	
	
	int counter = 0;
	int counter2 = 0;
	
	//Supposed to be a 50 hz loop. Not even close due to lag of transmitting data.
	while (true)
	{
		//Read IMU
		imuSensor->readSensor();
		magnetometerSensor->readSensor();
		
		
		/*ahrs->update(.2, .3, .4,
			.5, .6, .7,
			.8, .9, .10);*/
		
		//Update AHRS
		ahrs->update(imuSensor->getFRDAccXMss(), imuSensor->getFRDAccYMss(), imuSensor->getFRDAccZMss(),
					 imuSensor->getFRDGyroXRs(), imuSensor->getFRDGyroYRs(), imuSensor->getFRDGyroZRs(),
					 magnetometerSensor->getFRDX(), magnetometerSensor->getFRDY(), magnetometerSensor->getFRDZ());
		
		
		
		
		/**
		 * Calculate nav settings. From this paper: A comparison of complementary and kalman filtering.pdf by Walter T. Higgins, JR
		 */
		
		
		/**
		 * Calculate velocity
		 */
/*		float tempVelocityX = ahrs->getLinearAccelerationXMss() * (1/accelerometerSamplingFrequency) + velocityX;
		
		
		
		//apply complimentary filter
		tempVelocityX = tempVelocityX * (1 - (accelFreqSquared / (accelFreqSquared + complementaryFilterAlpha * accelerometerSamplingFrequency + complimentaryFilterBeta)));
*/		
		
		if (counter2++ >= 30)
		{
			gpsSensor->readSensorSolution();
			
			/*
			
			//Get the current helicopters position relative to it's starting point in ecef.
			long differenceXECEF = gpsSensor->getXEcefCm() - initialXPositionEcef;
			long differenceYECEF = gpsSensor->getYEcefCm() - initialYPositionEcef;
			long differenceZECEF = gpsSensor->getZEcefCm() - initialZPositionEcef;
			
			
			
			float rotatedMatrix[3] = {};
			float positionMatrix[3] = {(float)differenceXECEF, (float)differenceYECEF, (float)differenceZECEF};
		
			//Rotate the current ecef position from earth centered earth fixed (ECEF) into North-East-Down(NED).
			//Iterate through the rows of the rotation matrix
			MatrixUtil::RotateMatrix(EcefToLocalNEDRotationMatrix,positionMatrix,rotatedMatrix);
		
			float localNEDXCm = rotatedMatrix[0];
			float localNEDYCm = rotatedMatrix[1];
			float localNEDZCm = rotatedMatrix[2];			
			*/
			
			//Rotate ECEF velocity to local NED
			float rotatedVelocityMatrix[3] = {};
			float velocityMatrix[3] = {(float)gpsSensor->getXVEcefCms(), (float)gpsSensor->getYVEcefCms(), (float)gpsSensor->getZVEcefCms()};
			MatrixUtil::RotateMatrix(EcefToLocalNEDRotationMatrix,velocityMatrix,rotatedVelocityMatrix);

			//rotate velocity into body frame
			float nedToBodyFrameMatrix[3][3] = {};
			MatrixUtil::CreateRotationMatrixTransposed(ahrs->getRollRads(), ahrs->getPitchRads(), ahrs->getYawRads(), nedToBodyFrameMatrix);
			MatrixUtil::RotateMatrix(nedToBodyFrameMatrix,rotatedVelocityMatrix,velocityBodyFrame);
			
			//MatrixUtil::RotateMatrix(ahrs->getRollRads(), ahrs->getPitchRads(), ahrs->getYawRads(), rotatedVelocityMatrix, velocityBodyFrame);
			
			
			
			//float gpsXVel =(float) (gpsSensor->getXVEcefCms() / 1000);
			
			//velocityX = tempVelocityX + (gpsXVel * (complimentaryFilterBeta / (accelFreqSquared + complementaryFilterAlpha * accelerometerSamplingFrequency + complimentaryFilterBeta)));
			counter2 = 0;
		}else
		{
//			velocityX = tempVelocityX;
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		if (counter++ > 10)
		{
			//Send linear acc, and euler angles to GCS.
			serialDriver->transmit((byte)'S');
			
			
			/*serialDriver->transmit(ahrs->getYawRads());
			serialDriver->transmit(ahrs->getPitchRads());
			serialDriver->transmit(ahrs->getRollRads());*/
			/*serialDriver->transmit(imuSensor->getFRDGyroZRs());
			serialDriver->transmit(imuSensor->getFRDGyroYRs());
			serialDriver->transmit(imuSensor->getFRDGyroXRs());		
			serialDriver->transmit(ahrs->data1);
			serialDriver->transmit(ahrs->data2);
			serialDriver->transmit(ahrs->data3);*/

			
			/*
			serialDriver->transmit(imuSensor->getFRDAccXMss());
			serialDriver->transmit(imuSensor->getFRDAccYMss());
			serialDriver->transmit(imuSensor->getFRDAccZMss());
			serialDriver->transmit(imuSensor->getFRDGyroXRs());
			serialDriver->transmit(imuSensor->getFRDGyroYRs());
			serialDriver->transmit(imuSensor->getFRDGyroZRs());
			serialDriver->transmit(magnetometerSensor->getFRDX());
			serialDriver->transmit(magnetometerSensor->getFRDY());
			serialDriver->transmit(magnetometerSensor->getFRDZ());
			*/
			
		
			
			serialDriver->transmit(ahrs->getYawRads());
			serialDriver->transmit(ahrs->getPitchRads());
			serialDriver->transmit(ahrs->getRollRads());
			serialDriver->transmit(ahrs->getLinearAccelerationXMss());
			serialDriver->transmit(ahrs->getLinearAccelerationYMss());
			serialDriver->transmit(ahrs->getLinearAccelerationZMss());
			//serialDriver->transmit(velocityX);
			serialDriver->transmit(velocityBodyFrame[0]);
			
			
			
			
			
			
			

			
			counter = 0;
		}
		
		
		//_delay_ms(20);	
	}

	
	return 0;
}

int prototypefusion_test(TestCase *test)
{
	//implement ahrs system
	//create tests for that
	//then develop complementary filter
	
	
	//Create and initialize IMU sensor
	//Create and initialize GPS
	
	
	
	//count
	//for every 10 counts, read the gps
	//every time read the imu
	//run the algorithm
	//send the data to the GCS.
	
	
	return 0;
}


int fakedata_test(TestCase *test)
{

float EcefToLocalNEDRotationMatrix[3][3] = {};
	
//CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(47.495945, -122.216972, EcefToLocalNEDRotationMatrix);	
CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(47.495945, -122.216972, EcefToLocalNEDRotationMatrix);	
	
	float rotatedVelocityMatrix[3] = {};
	float velocityBodyFrame[3] = {};
	
	float velocityMatrix[3] = {(float)-1047, (float)	-1472, (float)	0};
	MatrixUtil::RotateMatrix(EcefToLocalNEDRotationMatrix, velocityMatrix, rotatedVelocityMatrix);

	//rotate velocity into body frame
	float nedToBodyFrameMatrix[3][3] = {};
	MatrixUtil::CreateRotationMatrixTransposed(-0.00101321645, -0.109547511, 2.80199742, nedToBodyFrameMatrix);
	MatrixUtil::RotateMatrix(nedToBodyFrameMatrix,rotatedVelocityMatrix, velocityBodyFrame);
	
	/*
	float vx = velocityBodyFrame[0];
	float vy = velocityBodyFrame[1];
	float vz = velocityBodyFrame[2];
	*/
	
	
	
	
	
	
	
	
	
	float gyroscopeSamplingFrequency = 98.0f;
	
	
	AHRS *ahrs = new AHRS(1/gyroscopeSamplingFrequency);
	
	while(true)
	{
		//the mag value should be positive since it's supposed to be in FRD format.
		//ahrs->update(-.386354269,.0948173553,9.792792,0.0107812453,-0.000344082626,0,-0.0526136123,-0.3411594,0.9385319);		
		ahrs->update(-.386354269,.0948173553,9.792792,0,0,0,-0.0526136123,-0.3411594,0.9385319);		
	}

	
	
	return 0;
}
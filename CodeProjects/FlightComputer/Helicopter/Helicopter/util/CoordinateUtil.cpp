/*
 * CoordinateUtil.cpp
 *
 * Created: 12/17/2013 11:22:37 PM
 *  Author: HP User
 */ 
#include <math.h>

#include "CoordinateUtil.h"

#include "MatrixUtil.h"


using namespace helicopter::util;


const float CoordinateUtil::E = 0.08181919;
const float CoordinateUtil::REA = 6378137.0;


const float CoordinateUtil::PIOVER180 = M_PI / 180.0f;

void CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(float latitudeDegrees, float longitudeDegrees, float  (&ecefToLocalNEDRotationMatrix)[3][3])
{
	float radLat = DegreesToRad(latitudeDegrees);
	float radLong = DegreesToRad(longitudeDegrees);
	
	float sinLat = sin(radLat);
	float cosLat = cos(radLat);
	
	float sinLong = sin(radLong);
	float cosLong = cos(radLong);

	//Create a rotation matrix for rotating earth-centered-earth-fixed coordinates
	//into Local North-East-Down coordinates.
	ecefToLocalNEDRotationMatrix[0][0] = -1 * sinLat * cosLong;
	ecefToLocalNEDRotationMatrix[0][1] = -1 * sinLat * sinLong;
	ecefToLocalNEDRotationMatrix[0][2] = cosLat;
	ecefToLocalNEDRotationMatrix[1][0] = -1 * sinLong;
	ecefToLocalNEDRotationMatrix[1][1] = cosLong;
	ecefToLocalNEDRotationMatrix[1][2] = 0;
	ecefToLocalNEDRotationMatrix[2][0] = -1 * cosLat * cosLong;
	ecefToLocalNEDRotationMatrix[2][1] = -1 * cosLat * sinLong;
	ecefToLocalNEDRotationMatrix[2][2] = -1 * sinLat;
}

float CoordinateUtil::calculateNe(float E, float latitudeRads)
{
	//return REA / sqrt(1 - pow(E, 2) * pow(sin(latitudeRads), 2));
	return REA / sqrt(1 - (E*E) * pow(sin(latitudeRads), 2));
}

/**
 * A description of the variables used to convert lat/long to ecef can be found on page 25 of 
 * Unmanned Rotor Craft Systems
 */
void CoordinateUtil::ConvertFromGeodeticToECEF(float latitudeDegrees, float longitudeDegrees, float altitudeFeetAgl, float &ecefX, float &ecefY, float &ecefZ)
{
	float radLat = DegreesToRad(latitudeDegrees);
	float radLong = DegreesToRad(longitudeDegrees);
	

	float Ne = calculateNe(E, radLat);
	
	ecefX = (Ne + altitudeFeetAgl) * cos(radLat) * cos(radLong);
	ecefY = (Ne + altitudeFeetAgl) * cos(radLat) * sin(radLong);
	ecefZ = ((Ne * (1 - (E*E))) + altitudeFeetAgl) * sin(radLat);
}

void CoordinateUtil::ConvertFromECEFToLocalNED(float ecefReferenceX, float ecefReferenceY, float ecefReferenceZ,
	float ecefX, float ecefY, float ecefZ, float ecefToLocalNEDRotationMatrix[][3],
	float &localNEDX, float &localNEDY, float &localNEDZ)
{
	//Get the current helicopters position relative to it's starting point in ecef.
	float differenceXECEF = ecefX - ecefReferenceX;
	float differenceYECEF = ecefY - ecefReferenceY;
	float differenceZECEF = ecefZ - ecefReferenceZ;
	
	float rotatedMatrix[3] = {};
	float positionMatrix[3] = {differenceXECEF, differenceYECEF, differenceZECEF};
		
	//Rotate the current ecef position from earth centered earth fixed (ECEF) into North-East-Down(NED).
	//Iterate through the rows of the rotation matrix
	MatrixUtil::RotateMatrix(ecefToLocalNEDRotationMatrix,positionMatrix,rotatedMatrix);
	
	localNEDX = rotatedMatrix[0];
	localNEDY = rotatedMatrix[1];
	localNEDZ = rotatedMatrix[2];
}



void CoordinateUtil::ConvertFromECEFToLocalNED(long ecefReferenceX, long ecefReferenceY, long ecefReferenceZ,
	long ecefX, long ecefY, long ecefZ, float ecefToLocalNEDRotationMatrix[][3],
	float &localNEDX, float &localNEDY, float &localNEDZ)
{
	//Get the current helicopters position relative to it's starting point in ecef.
	long differenceXECEF = ecefX - ecefReferenceX;
	long differenceYECEF = ecefY - ecefReferenceY;
	long differenceZECEF = ecefZ - ecefReferenceZ;
	
	float rotatedMatrix[3] = {};
	float positionMatrix[3] = {(float)differenceXECEF, (float)differenceYECEF, (float)differenceZECEF};
	
	//Rotate the current ecef position from earth centered earth fixed (ECEF) into North-East-Down(NED).
	//Iterate through the rows of the rotation matrix
	MatrixUtil::RotateMatrix(ecefToLocalNEDRotationMatrix,positionMatrix,rotatedMatrix);
	
	localNEDX = rotatedMatrix[0];
	localNEDY = rotatedMatrix[1];
	localNEDZ = rotatedMatrix[2];
}


void CoordinateUtil::ConvertFromGeodedicToLocalNED(
	float geodedicLatitude, float geodedicLongitude, float altitudeFeetAgl,
	float ecefReferenceX, float ecefReferenceY, float ecefReferenceZ, float ecefToLocalNEDRotationMatrix[][3],
	float &localNEDX,float &localNEDY,float &localNEDZ)
{
	float ecefX = 0;
	float ecefY = 0;
	float ecefZ = 0;
	
	ConvertFromGeodeticToECEF(geodedicLatitude, geodedicLongitude, altitudeFeetAgl, ecefX, ecefY, ecefZ);
	
	ConvertFromECEFToLocalNED(ecefReferenceX, ecefReferenceY, ecefReferenceZ,
		ecefX, ecefY, ecefZ, ecefToLocalNEDRotationMatrix,
		localNEDX, localNEDY, localNEDZ);
}
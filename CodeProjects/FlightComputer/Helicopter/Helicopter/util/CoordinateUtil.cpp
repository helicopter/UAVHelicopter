/*
 * CoordinateUtil.cpp
 *
 * Created: 12/17/2013 11:22:37 PM
 *  Author: HP User
 */ 
#include "CoordinateUtil.h"
#include <math.h>


using namespace helicopter::util;

double CoordinateUtil::DegreesToRad(double valueDegrees)
{
	return valueDegrees * (M_PI / 180.0);
}

void CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(double latitudeDegrees, double longitudeDegrees, double  ecefToLocalNEDRotationMatrix[][3])
{
	double radLat = DegreesToRad(latitudeDegrees);
	double radLong = DegreesToRad(longitudeDegrees);

	
	ecefToLocalNEDRotationMatrix[0][0] = -1 * sin(radLat) * cos(radLong);
	ecefToLocalNEDRotationMatrix[0][1] = -1 * sin(radLat) * sin(radLong);
	ecefToLocalNEDRotationMatrix[0][2] = cos(radLat);
	ecefToLocalNEDRotationMatrix[1][0] = -1 * sin (radLong);
	ecefToLocalNEDRotationMatrix[1][1] = cos(radLong);
	ecefToLocalNEDRotationMatrix[1][2] = 0;
	ecefToLocalNEDRotationMatrix[2][0] = -1 * cos(radLat) * cos(radLong);
	ecefToLocalNEDRotationMatrix[2][1] = -1 * cos(radLat)*sin(radLong);
	ecefToLocalNEDRotationMatrix[2][2] = -1 * sin(radLat);
}

double CoordinateUtil::calculateNe(double E, double latitudeRads)
{
	return Rea / sqrt(1 - pow(E, 2) * pow(sin(latitudeRads), 2));
}

/**
 * A description of the variables used to convert lat/long to ecef can be found on page 25 of 
 * Unmanned Rotor Craft Systems
 */
void CoordinateUtil::ConvertFromGeodeticToECEF(double latitudeDegrees, double longitudeDegrees, double altitudeFeetAgl, double &ecefX, double &ecefY, double &ecefZ)
{
	double radLat = DegreesToRad(latitudeDegrees);
	double radLong = DegreesToRad(longitudeDegrees);
	

	double Ne = calculateNe(E, radLat);
	
	ecefX = (Ne + altitudeFeetAgl) * cos(radLat) * cos(radLong);
	ecefY = (Ne + altitudeFeetAgl) * cos(radLat) * sin(radLong);
	ecefZ = ((Ne * (1 - pow(E, 2))) + altitudeFeetAgl) * sin(radLat);
}

void CoordinateUtil::ConvertFromECEFToLocalNED(double ecefReferenceX, double ecefReferenceY, double ecefReferenceZ,
	double ecefX, double ecefY, double ecefZ, double ecefToLocalNEDRotationMatrix[][3],
	double &localNEDX, double &localNEDY, double &localNEDZ)
{
	//Get the current helicopters position relative to it's starting point in ecef.
	double differenceXECEF = ecefX - ecefReferenceX;
	double differenceYECEF = ecefY - ecefReferenceY;
	double differenceZECEF = ecefZ - ecefReferenceZ;
	
	double rotatedMatrix[3] = {};
	double positionMatrix[3] = {differenceXECEF, differenceYECEF, differenceZECEF};
	
	//Rotate the current ecef position from earth centered earth fixed (ECEF) into North-East-Down(NED).
	//Iterate through the rows of the rotation matrix
	for (int i = 0; i < 3; i++)
	{
		//iterate through the columns of the rotation matrix
		for (int j = 0; j < 3; j++)
		{
			rotatedMatrix[i] += ecefToLocalNEDRotationMatrix[i] [j] * positionMatrix[j];
		}
	}
	
	localNEDX = rotatedMatrix[0];
	localNEDY = rotatedMatrix[1];
	localNEDZ = rotatedMatrix[2];
}

void CoordinateUtil::ConvertFromGeodedicToLocalNED(
	double geodedicLatitude, double geodedicLongitude, double altitudeFeetAgl,
	double ecefReferenceX, double ecefReferenceY, double ecefReferenceZ, double ecefToLocalNEDRotationMatrix[][3],
	double &localNEDX,double &localNEDY,double &localNEDZ)
{
	double ecefX = 0;
	double ecefY = 0;
	double ecefZ = 0;
	
	ConvertFromGeodeticToECEF(geodedicLatitude, geodedicLongitude, altitudeFeetAgl, ecefX, ecefY, ecefZ);
	
	ConvertFromECEFToLocalNED(ecefReferenceX, ecefReferenceY, ecefReferenceZ,
		ecefX, ecefY, ecefZ, ecefToLocalNEDRotationMatrix,
		localNEDX, localNEDY, localNEDZ);
}
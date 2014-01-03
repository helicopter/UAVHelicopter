/*
 * MathTests.cpp
 *
 * Created: 12/15/2013 4:47:25 PM
 *  Author: HP User
 */ 

#include "CoordinateUtilTests.h"
#include "CoordinateUtil.h"

using namespace helicopter::util;

int coordinateutil_test(TestCase *test)
{
	//3 4  x  2 3 = 38 37
	//5 6  x  8 7 = 58 57
/*	float testArr1[2][2] = {{3,4}, {5, 6}};
		
	float testArr2[2][2] = {{2,3}, {8, 7}};
	
	float result = MathUtil::DotProduct(testArr1, testArr2);
	
	AssertTrue(result[0][0] == 38);
	AssertTrue(result[0][1] == 37);
	AssertTrue(result[1][0] == 58);
	AssertTrue(result[1][1] == 57);
	*/
	float initialPositionGeodeticLatitude = 47.500312805175781;
	float initialPositionGeodeticLongitude = -122.21683502197266;
	float initialAltitudeFeetAgl = 0;
	
	
	float ecefToLocalNEDRotationMatrix[3][3]= {};
	CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(initialPositionGeodeticLatitude, initialPositionGeodeticLongitude, ecefToLocalNEDRotationMatrix);
	
	
	float ecefReferenceX = 0;
	float ecefReferenceY = 0;
	float ecefReferenceZ = 0;
	CoordinateUtil::ConvertFromGeodeticToECEF(initialPositionGeodeticLatitude, initialPositionGeodeticLongitude, initialAltitudeFeetAgl, ecefReferenceX, ecefReferenceY, ecefReferenceZ);
	
	
	float geodedicLatitude = 47.5;
	float geodedicLongitude = -122.2;
	float altitudeFeetAgl = 100.1;
	float ecefX = 0;
	float ecefY = 0;
	float ecefZ = 0;
	CoordinateUtil::ConvertFromGeodeticToECEF(geodedicLatitude, geodedicLongitude, altitudeFeetAgl, ecefX, ecefY, ecefZ);
	

	
	
	float localNEDX = 0;
	float localNEDY = 0;
	float localNEDZ = 0;
	CoordinateUtil::ConvertFromECEFToLocalNED(ecefReferenceX, ecefReferenceY, ecefReferenceZ,ecefX,ecefY,ecefZ,ecefToLocalNEDRotationMatrix,localNEDX,localNEDY,localNEDZ);
	
	
	
	//Do initial reference conversion
	//gather new lat long.
	//convert lat long to ned.
	CoordinateUtil::ConvertFromGeodedicToLocalNED(geodedicLatitude, geodedicLongitude, altitudeFeetAgl, ecefReferenceX, ecefReferenceY, ecefReferenceZ,ecefToLocalNEDRotationMatrix,localNEDX,localNEDY,localNEDZ);
	
	
	
	//what about having a coordinate/frame of reference class?
	//so abstractly we have coordinates. and we go from one frame of reference to another.
	//but then you need to use xyz to get speed, and stuff.
	
	
	return 0;
}
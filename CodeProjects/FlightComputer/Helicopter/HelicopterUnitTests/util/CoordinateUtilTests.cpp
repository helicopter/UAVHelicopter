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
/*	double testArr1[2][2] = {{3,4}, {5, 6}};
		
	double testArr2[2][2] = {{2,3}, {8, 7}};
	
	double result = MathUtil::DotProduct(testArr1, testArr2);
	
	AssertTrue(result[0][0] == 38);
	AssertTrue(result[0][1] == 37);
	AssertTrue(result[1][0] == 58);
	AssertTrue(result[1][1] == 57);
	*/
	double initialPositionGeodeticLatitude = 47.500312805175781;
	double initialPositionGeodeticLongitude = -122.21683502197266;
	double initialAltitudeFeetAgl = 0;
	
	
	double ecefToLocalNEDRotationMatrix[3][3]= {};
	CoordinateUtil::CalculateECEFToLocalNEDRotationMatrix(initialPositionGeodeticLatitude, initialPositionGeodeticLongitude, ecefToLocalNEDRotationMatrix);
	
	
	double ecefReferenceX = 0;
	double ecefReferenceY = 0;
	double ecefReferenceZ = 0;
	CoordinateUtil::ConvertFromGeodeticToECEF(initialPositionGeodeticLatitude, initialPositionGeodeticLongitude, initialAltitudeFeetAgl, ecefReferenceX, ecefReferenceY, ecefReferenceZ);
	
	
	double geodedicLatitude = 47.5;
	double geodedicLongitude = -122.2;
	double altitudeFeetAgl = 100.1;
	double ecefX = 0;
	double ecefY = 0;
	double ecefZ = 0;
	CoordinateUtil::ConvertFromGeodeticToECEF(geodedicLatitude, geodedicLongitude, altitudeFeetAgl, ecefX, ecefY, ecefZ);
	

	
	
	double localNEDX = 0;
	double localNEDY = 0;
	double localNEDZ = 0;
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
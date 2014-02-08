/*
 * MatrixUtilTests.cpp
 *
 * Created: 2/1/2014 2:25:48 PM
 *  Author: HP User
 */ 

#include <math.h>

#include "UnitTestUtils.h"
#include "MatrixUtilTests.h"
#include "MatrixUtil.h"

using namespace helicopter::util;

int matrixrotation_test(TestCase *test)
{

	/**
	 * Rotate a matrix from xyz back, left, down, to xyz forward, right, down
	 * (so rotate about the z axis by 90 degrees).
	 */
	float phiRotationAboutXRads = 0.0;
	float thetaRotationAboutYRads = 0.0;
	float saiRotationAboutZRads = 90 * (M_PI / 180.0); // counter clockwise rotation

	float rotatedValues[3] = {};
		
	float valuesToRotate[3] = {.75f, .25f, 1.0f}; //{x,y,z} in frame back, right, down
		
	MatrixUtil::RotateMatrix(phiRotationAboutXRads, thetaRotationAboutYRads, saiRotationAboutZRads, valuesToRotate, rotatedValues);
	
	AssertTrue(AboutEqual(rotatedValues[0],-.25f));
	AssertTrue(rotatedValues[1] == .75f);
	AssertTrue(rotatedValues[2] == 1.0f);	
	

	MatrixUtil::RotateMatrix(0.0f, 90 * (M_PI / 180.0), 0.0, valuesToRotate, rotatedValues);
		
	AssertTrue(AboutEqual(rotatedValues[0], 1.0f));
	AssertTrue(AboutEqual(rotatedValues[1], .25f));
	AssertTrue(AboutEqual(rotatedValues[2], -.75f));	
	
	
	float rotationMatrix[3][3] = {};
		
	MatrixUtil::CreateRotationMatrix(phiRotationAboutXRads, thetaRotationAboutYRads, saiRotationAboutZRads, rotationMatrix);
	
	int valuesToRotate2[3] = {1, 1, 1}; //{x,y,z} in frame back, right, down

	MatrixUtil::RotateMatrix(rotationMatrix, valuesToRotate2, rotatedValues);


	AssertTrue(AboutEqual(rotatedValues[0], -1.0f));
	AssertTrue(AboutEqual(rotatedValues[1], 1.0f));
	AssertTrue(AboutEqual(rotatedValues[2], 1.0f));	
	
	return 0;
}
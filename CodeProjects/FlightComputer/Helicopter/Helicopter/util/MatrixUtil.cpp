/*
 * MatrixUtil.cpp
 *
 * Created: 2/1/2014 6:49:02 PM
 *  Author: HP User
 */ 
#include <math.h>
#include <string.h>

#include "MatrixUtil.h"

using namespace helicopter::util;

void MatrixUtil::CreateRotationMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float rotationMatrix[][3] )
{
	rotationMatrix[0][0] = cos(thetaRotationAboutYRads)*cos(saiRotationAboutZRads);
	rotationMatrix[0][1] = sin(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*cos(saiRotationAboutZRads) - cos(phiRotationAboutXRads)*sin(saiRotationAboutZRads);
	rotationMatrix[0][2] = cos(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*cos(saiRotationAboutZRads) + sin(phiRotationAboutXRads)*sin(saiRotationAboutZRads);
	rotationMatrix[1][0] = cos(thetaRotationAboutYRads)*sin(saiRotationAboutZRads);
	rotationMatrix[1][1] = sin(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*sin(saiRotationAboutZRads) + cos(phiRotationAboutXRads)*cos(saiRotationAboutZRads);
	rotationMatrix[1][2] = cos(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*sin(saiRotationAboutZRads) - sin(phiRotationAboutXRads)*cos(saiRotationAboutZRads);
	rotationMatrix[2][0] = -sin(thetaRotationAboutYRads);
	rotationMatrix[2][1] = sin(phiRotationAboutXRads)*cos(thetaRotationAboutYRads);
	rotationMatrix[2][2] = cos(phiRotationAboutXRads)*cos(thetaRotationAboutYRads);
}

void MatrixUtil::RotateMatrix( float rotationMatrix[][3], float valuesToRotate[3], float rotatedValues[3] )
{
	//clear out the rotated values list;
	memset(rotatedValues,0,sizeof(float) * 3);
	
	//iterate through the rows of the rotation matrix.
	for (int row = 0; row < 3; row++)
	{
		//iterate through the columns of the rotation matrix
		for (int column = 0; column < 3; column++)
		{
			//technically the rotatedMatrix[row] is writing to a 'column' in the matrix, and valuesToRotate is actually the 'row'.
			rotatedValues[row] += rotationMatrix[row] [column] * valuesToRotate[column];
		}
	}
}

void MatrixUtil::RotateMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float valuesToRotate[3], float rotatedValues[3])
{
	float rotationMatrix[3][3] = {};
		
	CreateRotationMatrix(phiRotationAboutXRads, thetaRotationAboutYRads, saiRotationAboutZRads, rotationMatrix);
		
	RotateMatrix(rotationMatrix, valuesToRotate, rotatedValues);
}

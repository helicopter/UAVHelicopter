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

void MatrixUtil::CrossProduct(float vector1[3], float vector2[3], float (&crossProduct)[3])
{
	crossProduct[0] = vector1[1] * vector2[2] - vector1[2]*vector2[1];
	crossProduct[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2];//-1 * (vector1[0] * vector2[2] - vector1[2]*vector2[0]);
	crossProduct[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];
}

void MatrixUtil::Normalize(float (&vector)[3])
{
	float magnitude = 0;
	
	//Square each vector value and add it to the magnitude
	for (int i = 0; i < 3; i++)
	{
		magnitude += vector[i] * vector[i];
	}
	
	magnitude = sqrt(magnitude);
	
	for (int i = 0; i < 3; i++)
	{
		vector[i] = vector[i] / magnitude;
	}
}

void MatrixUtil::Transpose(float originalMatrix[3][3], float (&transposedMatrix)[3][3])
{
	for(int i = 0 ; i < 3 ; i++ )
	{
		for(int j = 0 ; j < 3 ; j++ )
		{
			transposedMatrix[j][i] = originalMatrix[i][j];
		}
	}
}

void MatrixUtil::CreateRotationMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float (&rotationMatrix)[3][3] )
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

void MatrixUtil::CreateRotationMatrixTransposed( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float (&rotationMatrix)[3][3] )
{
	rotationMatrix[0][0] = cos(thetaRotationAboutYRads)*cos(saiRotationAboutZRads);
	rotationMatrix[0][1] = cos(thetaRotationAboutYRads)*sin(saiRotationAboutZRads);
	rotationMatrix[0][2] = -sin(thetaRotationAboutYRads);
	rotationMatrix[1][0] = sin(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*cos(saiRotationAboutZRads) - cos(phiRotationAboutXRads)*sin(saiRotationAboutZRads);
	rotationMatrix[1][1] = sin(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*sin(saiRotationAboutZRads) + cos(phiRotationAboutXRads)*cos(saiRotationAboutZRads);
	rotationMatrix[1][2] = sin(phiRotationAboutXRads)*cos(thetaRotationAboutYRads);
	rotationMatrix[2][0] = cos(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*cos(saiRotationAboutZRads) + sin(phiRotationAboutXRads)*sin(saiRotationAboutZRads);
	rotationMatrix[2][1] = cos(phiRotationAboutXRads)*sin(thetaRotationAboutYRads)*sin(saiRotationAboutZRads) - sin(phiRotationAboutXRads)*cos(saiRotationAboutZRads);
	rotationMatrix[2][2] = cos(phiRotationAboutXRads)*cos(thetaRotationAboutYRads);
}

void MatrixUtil::RotateMatrix( float rotationMatrix[][3], int valuesToRotate[3], float (&rotatedValues)[3] )
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

void MatrixUtil::RotateMatrix( float rotationMatrix[][3], float valuesToRotate[3], float (&rotatedValues)[3] )
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

void MatrixUtil::RotateMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float valuesToRotate[3], float (&rotatedValues)[3])
{
	float rotationMatrix[3][3] = {};
		
	CreateRotationMatrix(phiRotationAboutXRads, thetaRotationAboutYRads, saiRotationAboutZRads, rotationMatrix);
		
	RotateMatrix(rotationMatrix, valuesToRotate, rotatedValues);
}


float MatrixUtil::DotProduct( float vector1[3], float vector2[3] )
{
	return vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2];
}

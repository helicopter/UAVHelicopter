/*
 * MatrixUtil.h
 *
 * Created: 2/1/2014 6:48:43 PM
 *  Author: HP User
 */ 


#ifndef MATRIXUTIL_H_
#define MATRIXUTIL_H_


#define fsin(input,output) \
{ \
    if (input < -3.14159265) \
		input += 6.28318531f; \
    else if (input > 3.14159265) \
		input -= 6.28318531f; \
    if (input < 0) \
		output = 1.27323954f * input + .405284735f * input * input; \
    else \
		output = 1.27323954f * input - 0.405284735f * input * input; \
} \

namespace helicopter
{
	namespace util
	{
		class MatrixUtil
		{	
			private:
				
				/*static const int RESOLUTION = 1000;
				static float* sinTable;*/

				static const int ARRAYSIZE = sizeof(float) * 3;
			
			public: 
			
				//static void createLookupTables();
				
				//Uses lookup table to determine sin value. 
				//Value should be in rads.
				//static float fsin(float valueRads);
				
				/**
				 * Creates a 3x3 rotation matrix which rotates about X, then Y, then Z where positive angles are rotations
				 * about the given axis in a clockwise direction.
				 *
				 * The rotation matrix that is created can be used to go from Body Frame coordinate System, to Local NED coordinate System
				 */
				static void CreateRotationMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float (&rotationMatrix)[3][3]);
				
				/**
				 * This creates a 3x3 rotation matrix that is the transpose of the matrix created in CreateRotationMatrix.
				 *
				 * This rotation matrix can be used to go from Local NED to Body Frame coordinate Systems.
				 */
				static void CreateRotationMatrixTransposed( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float (&rotationMatrix)[3][3]);
				
				
				
				/**
				 * Creates a transposed rotation matrix using the sin and cos tables rather than the actual methods. 
				 */
				//static void CreateEstimatedRotationMatrixTransposed( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float (&rotationMatrix)[3][3]);
				
				/**
				 * Rotates a 3x1 matrix using a 3x3 rotation matrix
				 */
				static void RotateMatrix( float rotationMatrix[][3], float valuesToRotate[3], float (&rotatedValues)[3]);
				
				/**
				 * Rotates a 3x1 matrix using a 3x3 rotation matrix
				 */
				static void RotateMatrix( float rotationMatrix[][3], int valuesToRotate[3], float (&rotatedValues)[3]);	
				
				/**
				 * Rotates a 3x1 matrix using the generated matrix created by the CreateRotationMatrix method.
				 * e.g. if saiRotationAboutZRads is pi/2 (90 degrees), then that would result in the x/y plane being rotated 90 degrees clockwise.
				 */
				static void RotateMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float valuesToRotate[3], float (&rotatedValues)[3]);
				
				/**
				 * Normalizes the contents of a vector so the magnitude of the vector = 1.
				 * This is a mutator function, so the passed in vector will have it's values normalized.
				 */
				static void Normalize(float (&vector)[3]);				
					
				/**
				 * Calculates the cross product of two vectors.
				 * @return crossProduct - The cross product of the two vectors
				 */
				static void CrossProduct(float vector1[3], float vector2[3], float (&crossProduct)[3]);
				
				
				/**
				 * Calculates the dot product of two vectors
				 */
				static float DotProduct( float vector1[3], float vector2[3] );
				
				/**
				 * Creates a transpose of the given matrix.
				 */
				static void Transpose(float originalMatrix[3][3], float (&transposedMatrix)[3][3]);
		};
	}
	
}




#endif /* MATRIXUTIL_H_ */
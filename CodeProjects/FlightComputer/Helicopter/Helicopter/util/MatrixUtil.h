/*
 * MatrixUtil.h
 *
 * Created: 2/1/2014 6:48:43 PM
 *  Author: HP User
 */ 


#ifndef MATRIXUTIL_H_
#define MATRIXUTIL_H_


namespace helicopter
{
	namespace util
	{
		class MatrixUtil
		{	
			private:

			
			public: 
				/**
				 * Creates a 3x3 rotation matrix which rotates about X, then Y, then Z where positive angles are rotations
				 * about the given axis in a clockwise direction
				 */
				static void CreateRotationMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float (&rotationMatrix)[3][3]);
				
				/**
				 * Rotates a 3x1 matrix using a 3x3 rotation matrix
				 */
				static void RotateMatrix( float rotationMatrix[][3], float valuesToRotate[3], float rotatedValues[3]);
				
				/**
				 * Rotates a 3x1 matrix using a 3x3 rotation matrix
				 */
				static void RotateMatrix( float rotationMatrix[][3], int valuesToRotate[3], float rotatedValues[3]);	
				
				/**
				 * Rotates a 3x1 matrix using the generated matrix created by the CreateRotationMatrix method.
				 * e.g. if saiRotationAboutZRads is pi/2 (90 degrees), then that would result in the x/y plane being rotated 90 degrees clockwise.
				 */
				static void RotateMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float valuesToRotate[3], float rotatedValues[3]);
				
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
		};
	}
	
}




#endif /* MATRIXUTIL_H_ */
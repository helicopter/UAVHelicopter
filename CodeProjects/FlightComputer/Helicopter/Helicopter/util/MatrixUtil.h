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
				 * Creates a 3x3 rotation matrix which rotates about X, then Y, then Z.
				 */
				static void CreateRotationMatrix( float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads, float rotationMatrix[][3]);
				
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
					
		};
	}
	
}




#endif /* MATRIXUTIL_H_ */
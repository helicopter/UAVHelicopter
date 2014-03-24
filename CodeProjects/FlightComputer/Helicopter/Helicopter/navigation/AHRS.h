/*
 * AHRS.h
 *
 * Created: 3/19/2014 7:16:24 PM
 *  Author: HP User
 */ 


#ifndef AHRS_H_
#define AHRS_H_

#include <string.h>

namespace helicopter 
{
	namespace navigation
	{
		/**
		 * This class represents the Attitude and Heading Reference System. This class takes
		 * sensor data to calculate the orientation, heading, and linear acceleration of the helicopter.
		 * This class uses a Direction Cosine Matrix complementary filter for determining orientation.
		 * This class builds a DCM matrix which can be used to rotate from body frame to ground frame (inertial frame)
		 * determine the orientation of the helicopter, and determine the linear acceleration of the helicopter.
		 * The code is based on the code found in https://code.google.com/p/picquadcontroller/source/browse/trunk/imu.h
		 * This is a good location discussing the DCM Complementary filter for INS - http://www.starlino.com/dcm_tutorial.html
		 * This is also a good paper - DCMDraft2.pdf http://gentlenav.googlecode.com/files/DCMDraft2.pdf
		 */
		class AHRS
		{
			private:
				static const float ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT;
				static const float MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT;
				
				float gyroTimeInterval;
				
				float frdAccX;
				float frdAccY;
				float frdAccZ;
				
				float frdGyroXRadsPerSec;
				float frdGyroYRadsPerSec;
				float frdGyroZRadsPerSec;
				
				float frdMagX;
				float frdMagY;
				float frdMagZ;
				
				float yawRads;
				float pitchRads;
				float rollRads;
				
				float linearAccelerationXMss;
				float linearAccelerationYMss;
				float linearAccelerationZMss;
				
				
				/**
				 * Direction cosine matrix which stores the cosines of the angles between the body frame vectors
				 * and the ground frame vectors. This can be thought of as the x,y,z components which is mapped from
				 * one frame to the other.
				 * 
				 * This matrix consists of the vectors I(X),J(Y) and K(Z), and i, j, k
				 * where I,J,K are unity vectors of the magnetic (north) vector, the cross product of the gravitational and magnetic (north) vector), and the gravitational vector respectively
				 * I,J,K represents the ground frame.
				 * i,j, and k are the vectors in body frame (magnetic, ixk, gravitation)
				 * Cos(I,i) represents the magnetic vector's projection onto the ground frames X axis (I). Cos(I,i) = I.i (the dot product between I and i = i sub x in ground frame (ixG) - the x component of the i vector projected onto the I (magnetic north) ground frame vector
				 * Cos(K,k) represents the gravitational vector's body frame projection onto the ground frames Z axis.
				 * (etc)
				 *
				 * The directed cosine matrix is defined as follows:
				 * Row 1 = Cos(I,i), Cos(I,j), Cos(I,k) - Magnetic
				 * Row 2 = Cos(J,i), Cos(J,j), Cos(J,k) - Mag X Gravity - cross product of magnetic and gravity vectors - i.e. a vector that is orthogonal to the plane formed by the mag and grav vectors
				 * Row 3 = Cos(K,i), Cos(K,k), Cos(K,k) - Gravity
				 * dcm{v][0] = x component of the vector - North East Down frame
				 * dcm[v][1] = y component of the vector - North East Down frame
				 * dcm[v][2] = z component of the vector - North East Down frame
				 */
				float dcm[3][3];
				
				/**
				 * Takes the vectorToScale and multiplies that by the scalerValue.
				 * Then takes the vectorToAdjust and subtracts the scaled vector.
				 * @return outputVector - The vector which represents the scaled and adjusted vector.
				 */
				void scaleAndAdjust(float vectorToAdjust[3], float vectorToScale[3], float scalerValue, float (&outputVector)[3]);
				
				/**
				 * Takes the dcm matrix and adjusts it to be more orthonormal (every vector is 90 degrees from each other).
				 */
				void orthonormalizeDcm();
			
			public:

				/**
				 * @param the time interval between gyroscope readings. I.e. .02 seconds.
				 */
				AHRS(float gyroTimeInterval):
					gyroTimeInterval(gyroTimeInterval),
					frdAccX(0),
					frdAccY(0),
					frdAccZ(0),
					frdGyroXRadsPerSec(0),
					frdGyroYRadsPerSec(0),
					frdGyroZRadsPerSec(0),
					frdMagX(0),
					frdMagY(0),
					frdMagZ(0),
					yawRads(0),
					pitchRads(0),
					rollRads(0),
					linearAccelerationXMss(0),
					linearAccelerationYMss(0),
					linearAccelerationZMss(0)	
				{
					/**
					 * Setup initial state of the DCM matrix with initial unit vectors
					 * {{1,0,0},{0,1,0},{0,0,1}}
					 */
					dcm[0][0] = 1;
					dcm[0][1] = 0;
					dcm[0][2] = 0;
					dcm[1][0] = 0;
					dcm[1][1] = 1;
					dcm[1][2] = 0;
					dcm[2][0] = 0;
					dcm[2][1] = 0;
					dcm[2][2] = 1;
				}
			
				/**
				 * Updates the internal representation of the orientation of the helicopter with the given
				 * sensor data and calculates the linear acceleration and euler angles given the updated orientation
				 * While the internal representation doesn't care about units for acceleration, in order to calculate linear acceleration
				 * the accelerometer data must be in meters per second per second.
				 *
				 * @param frdAccXMss - The accelerometer's X value in meters per second per second in Front-Right-Down coordinate system
				 * @param frdAccYMss - The accelerometer's Y value in meters per second per second in Front-Right-Down coordinate system
				 * @param frdAccZMss - The accelerometer's Z value in meters per second per second in Front-Right-Down coordinate system
				 * @param frdGyroXRs - The Gyroscopes angular acceleration value in Rads per second about the X axis in the Front-Right-Down coordinate system
				 * @param frdGyroYRs - The Gyroscopes angular acceleration value in Rads per second about the Y axis in the Front-Right-Down coordinate system
				 * @param frdGyroZRs - The Gyroscopes angular acceleration value in Rads per second about the Z axis in the Front-Right-Down coordinate system
				 * @param frdMaxX - The magnetometer sensor reading (no units needed) in the X axis in the Front Right Down coordinate system
				 * @param frdMaxY - The magnetometer sensor reading (no units needed) in the Y axis in the Front Right Down coordinate system
				 * @param frdMaxZ - The magnetometer sensor reading (no units needed) in the Z axis in the Front Right Down coordinate system
				 */ 
				void update(float frdAccXMss, float frdAccYMss, float frdAccZMss,
						float frdGyroXRs, float frdGyroYRs, float frdGyroZRs,
						float frdMagX, float frdMagY, float frdMagZ);
					
				/**
				 * Returns the Yaw of the system in radians (0-2pi). 0 = North, Pi/2 = east, 0-2pi.
				 */
				float getYawRads();
			
				/**
				 * Returns the pitch of the system in rads (-pi/2, pi/2). 0 = level with ground, pi/2 nose up in air
				 * -pi/2 nose pointing to ground. 
				 */
				float getPitchRads();
			
				/**
				 * Returns the roll of the system in rads (-pi, pi). 
				 * When looking at the back of the device (in the direction of the x axis in front-right-down coordinate system),
				 * 0 = level with ground, +pi/2 = on it's right side via clockwise rotation, almost +pi = upside down from clockwise rotation
				 * -pi/2 = on left side from counter clockwise rotation, almost -pi - upside down from counter clockwise rotation
				 */
				float getRollRads();
			
				/**
				 * Returns the linear acceleration of body frame FRD in the X direction in Meters per second^2
				 * In Front-Right-down coordinate system
				 * + when the device is pushed 'forward'
				 * - when device is pushed backwards
				 */
				float getLinearAccelerationXMss();
			
				/**
				 * Returns the linear acceleration of body frame FRD in the Y direction in Meters per second^2
				 * In Front-Right-down coordinate system
				 * + when the device is pushed 'right'
				 * - when device is pushed 'left'
				 */
				float getLinearAccelerationYMss();
			
				/**
				 * Returns the linear acceleration of body frame FRD in the Z direction in Meters per second^2
				 * In Front-Right-down coordinate system
				 * + when the device is pushed 'down'
				 * - when device is pushed up				 
				 */
				float getLinearAccelerationZMss();
		};
	}
}


#endif /* AHRS_H_ */
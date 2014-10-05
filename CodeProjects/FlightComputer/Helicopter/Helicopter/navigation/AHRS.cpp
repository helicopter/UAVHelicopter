/*
 * AHRS.cpp
 *
 * Created: 3/19/2014 7:16:35 PM
 *  Author: HP User
 */ 
#include <math.h>

#include "AHRS.h"
#include "MatrixUtil.h"
#include "CommonHeader.h"

using namespace helicopter::navigation;
using namespace helicopter::util;

/*
const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .07f;
const float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .01f;
*/

/*
const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .1f;
const float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .1f;
*/

//const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .17f;
//const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .10f;//latest
//const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .0175;//.00975;//.0001f;//.0175f;
//const float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .23f;


//Latest 6/13/2014
//const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .00975;
//const float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .75f;


/*
const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .02f;//.1f;//.0975;
const float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .05f;//.75f;
*/

//experimental 6/14/2014
//latest 8/3/2014
//float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .015f;
//float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .05f;

//more experimental 8/3/2014
//const float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .076f;


//latest 9/7/2014 - good values. 
//float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .016f;
//float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .05f;


//tuned gains
//float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .007;
//float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .09;


/*float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .001f; //.0036f;//.001;//.015f;//.008f;//.003;
float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .01f;//.08f;//.05f;//.15;//.14f;//.08;//.08;*/
float AHRS::ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT = .001f;
float AHRS::MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT = .01f;

void AHRS::scaleAndAdjust(float vectorToAdjust[3], float vectorToScale[3], float scalerValue, float (&outputVector)[3])
{
	outputVector[0] = vectorToAdjust[0] - scalerValue * vectorToScale[0];
	outputVector[1] = vectorToAdjust[1] - scalerValue * vectorToScale[1];
	outputVector[2] = vectorToAdjust[2] - scalerValue * vectorToScale[2];	
}


void AHRS::orthonormalizeDcm() 
{
	/**
	 * The dot product of two orthonormal vectors should be 0 (because cos(90) = 0). So this
	 * gives us an error of how far from orthonormal the vectors are. It's a measure
	 * of how much X and Y are rotated towards each other. 
	 */
	float error = MatrixUtil::DotProduct(dcm[0], dcm[1]);
	
	/**
	 * Scale and adjust the two vectors involved in calculating the error to bring them 
	 * 'closer' to each other (more orthonormal)
	 */
	scaleAndAdjust(dcm[0], dcm[1], (error/2), dcm[0]);
	scaleAndAdjust(dcm[1], dcm[0], (error/2), dcm[1]);
	
	//The remaining vector will become the cross product of the other two vectors
	MatrixUtil::CrossProduct(dcm[0], dcm[1], dcm[2]);
	
	//Normalize the matrix to ensure that the vector's magnitudes = 1
	MatrixUtil::Normalize(dcm[0]);
	MatrixUtil::Normalize(dcm[1]);
	MatrixUtil::Normalize(dcm[2]);
}




float constrain_float2(float amt, float low, float high)
{
	// the check for NaN as a float prevents propogation of
	// floating point errors through any function that uses
	// constrain_float(). The normal float semantics already handle -Inf
	// and +Inf
	if (isnan(amt)) {
		return (low+high)*0.5f;
	}
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}


float wrap(float angle)
{
	if (angle > M_PI)
	{
		angle -= (2*M_PI);
	}
	 if (angle < -M_PI)
	{
		angle += (2*M_PI);
	}
	 if (angle < 0)
	{
		angle += 2*M_PI;
	}
	return angle;
}



void AHRS::update(float frdAccXMss, float frdAccYMss, float frdAccZMss,
	float frdGyroXRs, float frdGyroYRs, float frdGyroZRs,
	float frdMagX, float frdMagY, float frdMagZ)
{
	float accelerometerVector[3] = {frdAccXMss, frdAccYMss, frdAccZMss};
	float magnetometerVector[3] = {frdMagX, frdMagY, frdMagZ};
		
	float accelerometerAngularDisplacement[3] = {0};
	float magnetometerAngularDisplacement[3] = {0};
		
	//Scale the gyro value by the time interval to get the angle in rads.
	float gyroscopeAngularDisplacement[3] = { frdGyroXRs * gyroTimeInterval, 
												frdGyroYRs * gyroTimeInterval, 
												frdGyroZRs * gyroTimeInterval};	
												
	float angularDisplacementWeightedAverage [3] = {0};	
		
	float changeInVector [3] = {0};
		 
	//Normalize the accelerometer and magnetometer data so that their magnitudes = 1. The DCM matrix
	//is based on unit vectors (vectors with a magnitude of one)
	MatrixUtil::Normalize(accelerometerVector);
	MatrixUtil::Normalize(magnetometerVector);
	
	/**
	 * The magnetometer vector is not orthogonal to the gravitational vector. It actually points
	 * downward 'into' the earth. The angle is called the 'magnetic angle of inclination' and
	 * is roughly 70 degrees into the earths surface in North America.
	 *
	 * Because of this, if it's not corrected for, the pitch and roll axes will greatly move
	 * in response to what should only be a yaw rotation. I.e. if the device is sitting on a table
	 * and you rotated it to point south, that rotation would be recognized as a pitch and roll movement
	 * as well as a yaw movement. 
	 *
	 * This is corrected by taking the cross product of the accelerometer vector, and the magnetometer vector, 
	 * to get a 'west facing' (assuming NED coordinate system) vector. Then taking the cross product of the
	 * west vector, and the accelerometer vector to get a 'north facing' vector which is the corrected
	 * magnetometer vector.
	 *
	 * Taken from Starlino's January 20, 2012 post at http://www.starlino.com/dcm_tutorial.html 
	 */
	float westFacingVector[3] = {0};
	float correctedMagnetometerVector[3] = {0};
		
	
	MatrixUtil::CrossProduct(dcm[2], magnetometerVector, westFacingVector);	
	MatrixUtil::CrossProduct(westFacingVector, dcm[2], correctedMagnetometerVector);
	
	
	//Doing it this way prevents a 'gyro' effect where if you pitch up, it causes the gyro to affect the yaw, and then it 'stabilizes' back to normal value
	//This way prevents that. if you took the DCM value and cross proded that with the mag value, you would get the gyro affect. 
	/*MatrixUtil::CrossProduct(accelerometerVector, magnetometerVector, westFacingVector);
	MatrixUtil::CrossProduct(westFacingVector, accelerometerVector, correctedMagnetometerVector);	*/
	
	MatrixUtil::Normalize(correctedMagnetometerVector);
	
	
	
	
/*
float cos_pitch_sq = 1.0f-(dcm[2][0]*dcm[2][0]);

// Tilt compensated magnetic field Y component:
float headY = frdMagY * dcm[2][2] - frdMagZ * dcm[2][1];

// Tilt compensated magnetic field X component:
float headX = frdMagX * cos_pitch_sq - dcm[2][0] * (frdMagY * dcm[2][1] + frdMagZ * dcm[2][2]);

// magnetic heading
// 6/4/11 - added constrain to keep bad values from ruining DCM Yaw - Jason S.
float heading = constrain_float2(atan2f(-headY,headX), -3.15f, 3.15f); //+M_PI;	
*/

//http://diydrones.com/profiles/blogs/tiltcompensated-heading
    //phi = pitch
	//roll = theta
//float Xh = frdMagX * cos(pitchRads) + frdMagY * sin(rollRads) * sin(pitchRads) + frdMagZ * cos(rollRads) * sin(pitchRads);
//float Yh = frdMagY * cos(rollRads) - frdMagZ * sin(rollRads);
//float heading = wrap(atan2(-Yh, Xh));
//
////theta = pitch, phi = roll, sai = yaw
//float cp = cosf(pitchRads);
//float sp = sinf(pitchRads);
//float sr = sinf(rollRads);
//float cr = cosf(rollRads);
//float sy = sinf(heading);
//float cy = cosf(heading);
//
//float cx1 = -sp;
//float cy1 = sr * cp;
//float cz1 = cr * cp;
	//
	///*
//correctedMagnetometerVector[0] = Xh;
//correctedMagnetometerVector[1] = -Yh;
//correctedMagnetometerVector[2] = 0;
//*/
	//
//correctedMagnetometerVector[0] = cp*cy;
//correctedMagnetometerVector[1] = sr*sp*cy-cr*sy;
//correctedMagnetometerVector[2] = cr*sp*cy+sr*sy;
	//
	//
//MatrixUtil::Normalize(correctedMagnetometerVector);
//







	//Calculate the cross product between the accelerometer data, and the
	//DCM matrix's gravitational vector.
	//This gives the angular displacement that the helicopter has moved since the last
	//iteration.
	//d?a ­= dt wa = KB0 x (KB1A­ - KB0) - see http://www.starlino.com/dcm_tutorial.html for proof		
	MatrixUtil::CrossProduct(accelerometerVector, dcm[2], accelerometerAngularDisplacement);
	MatrixUtil::CrossProduct(correctedMagnetometerVector, dcm[0], magnetometerAngularDisplacement);
	
	/*
	debug1 = accelerometerVector[0];
	debug2 = accelerometerVector[1];
	debug3 = accelerometerVector[2];
	*/
	//Calculate the weighted average of the angular displacements to obtain the correction vector
	//to adjust the DCM.
	for (int i = 0; i < 3; i++)
	{
		/**
		 * Note: The gyroscope angular measurement is * -1 because a clockwise rotation about X is a positive change for the gyroscope
		 * but for the accelerometer, a clockwise rotation about X results in a negative change, so we gyroData*-1 to bring the motions
		 * into alignment
		 */
		angularDisplacementWeightedAverage[i] = (-gyroscopeAngularDisplacement[i] + 
												accelerometerAngularDisplacement[i] * ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT + 
												magnetometerAngularDisplacement[i] * MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT) /
												(1.0f + ACCELEROMETER_ANGULARDISPLACEMENT_WEIGHT + MAGNETOMETER_ANGULARDISPLACEMENT_WEIGHT);

	}

	/**
	 * Adjust the dcm matrix by the calculated angular displacement. We adjust the DCM matrix by the averaged
	 * angle displacement of the three sensors because this reduces noise caused by any one sensor, and 
	 * reduces the effect of non-gravitational acceleration (linear acceleration) from the accelerometer.
	 */
	for (int i = 0; i < 3; i++)
	{
		MatrixUtil::CrossProduct(angularDisplacementWeightedAverage, dcm[i], changeInVector);
		
		dcm[i][0] += changeInVector[0];
		dcm[i][1] += changeInVector[1];
		dcm[i][2] += changeInVector[2];
	}
	
	
	
	//Adjust the matrix to make it closer to orthonormal.
	orthonormalizeDcm();

	/**
	 * The linear acceleration is calculated by taking the gravitational vector
	 * in the DCM (dcm[2]) and multiplying the individual components by the gravity constant.
	 * This will give you what proportion of acceleration is due to gravity. Then you subtract
	 * the accelerometer's value from the proportion due to gravity. 
	 * (You actually add the two values together because gravity is a negative value)
	 */
	linearAccelerationXMss = frdAccXMss + dcm[2][0] * GRAVITY_MSS;
	linearAccelerationYMss = frdAccYMss + dcm[2][1] * GRAVITY_MSS;
	linearAccelerationZMss = frdAccZMss + dcm[2][2] * GRAVITY_MSS;
	
	/**
	 * Calculate the euler angles. Eqn 3. In Euler Angles.pdf By William Premerlani
	 */
	//Add the yaw angle to pi to convert from -pi or pi being north and -3pi/2 being east to 0 or 2pi being north, and pi/2 being east
	yawRads = M_PI + atan2(dcm[1][0], dcm[0][0]);
	//yawRads = heading;
	
	
/*	
float cos_pitch_sq = 1.0f-(dcm[2][0]*dcm[2][0]);

// Tilt compensated magnetic field Y component:
float headY = frdMagY * dcm[2][2] - frdMagZ * dcm[2][1];

// Tilt compensated magnetic field X component:
float headX = frdMagX * cos_pitch_sq - dcm[2][0] * (frdMagY * dcm[2][1] + frdMagZ * dcm[2][2]);

// magnetic heading
// 6/4/11 - added constrain to keep bad values from ruining DCM Yaw - Jason S.
float heading = constrain_float2(atan2f(-headY,headX), -3.15f, 3.15f) +M_PI;


	
	
	
	
	
yawRads = heading;	*/
	
	
	
	
	pitchRads = -asin(dcm[2][0]);
	rollRads = atan2(dcm[2][1], dcm[2][2]);
}

float AHRS::getYawRads()
{
	return yawRads;
}

float AHRS::getPitchRads()
{
	return pitchRads;
}

float AHRS::getRollRads()
{
	return rollRads;
}

float AHRS::getLinearAccelerationXMss()
{
	return linearAccelerationXMss;
}

float AHRS::getLinearAccelerationYMss()
{
	return linearAccelerationYMss;
}

float AHRS::getLinearAccelerationZMss()
{
	return linearAccelerationZMss;
}


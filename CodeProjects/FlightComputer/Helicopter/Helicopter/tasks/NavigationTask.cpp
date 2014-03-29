/*
 * NavigationTask.cpp
 *
 * Created: 3/26/2014 7:32:33 AM
 *  Author: HP User
 */ 
#include <math.h>

#include "NavigationTask.h"
#include "CommonHeader.h"
#include "MatrixUtil.h"
#include "CoordinateUtil.h"

using namespace helicopter::tasks;
using namespace helicopter::util;

const float NavigationTask::WEIGHT = .15;


NavigationTask::NavigationTask (AHRS *ahrs, SystemModel *model, int delay, int period) :
Task(delay, period),
ahrs(ahrs),
model(model)
{
	
}

void NavigationTask::runTaskImpl()
{
	/**
	 * Update the AHRS
	 */
	ahrs->update(model->XAccelFrdMss(), model->YAccelFrdMss(), model->ZAccelFrdMss(),
				model->RollAngularVelocityRadsPerSecond(), model->PitchAngularVelocityRadsPerSecond(), model->YawAngularVelocityRadsPerSecond(),
				model->XMagFrd(), model->YMagFrd(), model->ZMagFrd());
				
	model->YawRads(ahrs->getYawRads());
	model->PitchRads(ahrs->getPitchRads());
	model->RollRads(ahrs->getRollRads());
	
	/**
	 * Process GPS data into NED position and velocity. 
	 */
	float rotatedVelocityMatrix[3] = {};
	float velocityBodyFrame[3] = {};
		
	float velocityMatrix[3] = {(float)model->XVEcefCms(), (float)model->YVEcefCms(), (float)model->ZVEcefCms()};
	MatrixUtil::RotateMatrix(model->EcefToLocalNEDRotationMatrix,velocityMatrix,rotatedVelocityMatrix);

	//rotate velocity into body frame
	float nedToBodyFrameMatrix[3][3] = {};
	MatrixUtil::CreateRotationMatrixTransposed(ahrs->getRollRads(), ahrs->getPitchRads(), ahrs->getYawRads(), nedToBodyFrameMatrix);
	MatrixUtil::RotateMatrix(nedToBodyFrameMatrix,rotatedVelocityMatrix, velocityBodyFrame);	
	
	model->XVelocityMetersPerSecond(velocityBodyFrame[0] / 100);
	model->YVelocityMetersPerSecond(velocityBodyFrame[1] / 100);
	

	
	
	
	
	/**
	 * Process barometer data into altitude
	 * altitude equation from: http://www.barnardmicrosystems.com/L4E_FMU_sensors.htm#Pressure
	 */
	float altitudeMeters = (288.15/(6.5/1000.0))*(1-(pow((model->PressureMillibars()/101325.0),(6.5/1000.0)*(287.052/9.78))));
					
	//exponential smoothing. http://en.wikipedia.org/wiki/Exponential_smoothing
	//St = aX + (1-a)St-1
	float altitudeMetersAglTemp = (float) (altitudeMeters - model->InitialAltitudeMeters());
	float previousAltitudeMetersAgl = model->AltitudeMetersAgl();
	float currentAltitudeMetersAgl = WEIGHT * (altitudeMetersAglTemp) + (1 - WEIGHT)*previousAltitudeMetersAgl;
	model->AltitudeMetersAgl(currentAltitudeMetersAgl);
	
	//Calculate altitude speed.
	model->ZVelocityMetersPerSecond((currentAltitudeMetersAgl - previousAltitudeMetersAgl) * BAROMETER_SENSOR_READ_PERIOD);
	
	
	
				
	/**
	* Convert position to local NED
	*/
//	float positionLocalNED[3] = {};
	
//	float positionMatrix[3] = {(float)model->XEcefCm(), (float)model->YEcefCm(), (float)model->AltitudeMetersAgl()};
//	MatrixUtil::RotateMatrix(model->EcefToLocalNEDRotationMatrix,positionMatrix,positionLocalNED);		
	
	
	float localNEDX = 0.0;
	float localNEDY = 0.0;
	float localNEDZ = 0.0;
	
	CoordinateUtil::ConvertFromECEFToLocalNED(model->InitialXPositionEcef(), model->InitialYPositionEcef(), model->InitialZPositionEcef(),
		model->XEcefCm(), model->YEcefCm(), model->ZEcefCm(), model->EcefToLocalNEDRotationMatrix,
		localNEDX, localNEDY, localNEDZ);
	
	model->XNEDLocalFrame(localNEDX);
	model->YNEDLocalFrame(localNEDY);
	//model->ZNEDLocalFrame(localNEDZ);//don't use localnedz as the z value because the altitude from barometer is more accurate
	model->ZNEDLocalFrame(currentAltitudeMetersAgl);
}

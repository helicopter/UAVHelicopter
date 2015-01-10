/*
 * NavigationTask.cpp
 *
 * Created: 3/26/2014 7:32:33 AM
 *  Author: HP User
 */ 
#include <math.h>
#include <avr/io.h>

#include "PVNavigationTask.h"
#include "CommonHeader.h"
#include "MatrixUtil.h"
#include "CoordinateUtil.h"

using namespace helicopter::tasks;
using namespace helicopter::util;

const float PVNavigationTask::WEIGHT = .15;


PVNavigationTask::PVNavigationTask (float barometerSensorReadPeriod, AHRS *ahrs, SystemModel *model, int delay, int period) :
Task(delay, period),
barometerSensorReadPeriod(barometerSensorReadPeriod),
ahrs(ahrs),
model(model)
{
	
}

void PVNavigationTask::runTaskImpl()
{	
	/**
	 * Process GPS data into NED position and velocity. 
	 */
	float rotatedVelocityMatrix[3] = {};
	float velocityBodyFrame[3] = {};
		
	float velocityMatrix[3] = {(float)model->XVEcefCms(), (float)model->YVEcefCms(), (float)model->ZVEcefCms()};
	MatrixUtil::RotateMatrix(model->EcefToLocalNEDRotationMatrix,velocityMatrix,rotatedVelocityMatrix);

	//rotate velocity into body frame
	float nedToBodyFrameMatrix[3][3] = {};
	/*
	MatrixUtil::CreateRotationMatrixTransposed(ahrs->getRollRads(), ahrs->getPitchRads(), ahrs->getYawRads(), nedToBodyFrameMatrix);
	MatrixUtil::RotateMatrix(nedToBodyFrameMatrix,rotatedVelocityMatrix, velocityBodyFrame);
	*/
	
	MatrixUtil::RotateMatrixT(model->Ahrs()->dcm,rotatedVelocityMatrix, velocityBodyFrame);	
	
	model->XVelocityFRDCms(velocityBodyFrame[0]);
	model->YVelocityFRDCms(velocityBodyFrame[1]);
	

	
	
	
	
	/**
	 * Process barometer data into altitude
	 * altitude equation from:https://www.brisbanehotairballooning.com.au/faqs/education/113-pressure-altitude-conversion.html
	 * Multiply by -100 because in NED, 'down' is positive. And to convert meters to cm.
	 * -0.00000687535 = -6.8755856 * 10^-6
	 */
	//float altitudeMslCm = (((pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1)/ (-0.00000687535)) / 3.28084) * -100.0f;
	//4433228.712 is a number I came up with from simplifying the expression (((pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1)/ (-0.00000687535)) / 3.28084) * -100.0f;
	float altitudeMslCm = (pow(10,log10(model->PressureMillibars()/1013.25) / 5.2558797) - 1) * 4433228.712; 
	
	//bool hasGoodAltitude = true;
	
	//model->ZNEDLocalFrameCm((float) (altitudeMslCm - model->InitialAltitudeCm()));
	
	
	if (!isnanf(altitudeMslCm) && 
		((altitudeMslCm - model->InitialAltitudeCm()) - model->ZNEDLocalFrameCm()) < 800 && 
		((altitudeMslCm - model->InitialAltitudeCm()) - model->ZNEDLocalFrameCm()) > -800)
	{
		/**
		 * If we get a NAN number, or the difference between the new altitude and previous altitude is over 800, then ignore the value.
		 * this could happen because the baro didn't get enough time to properly process it's data, so it gives junk values. 
		 */
	
		//exponential smoothing. http://en.wikipedia.org/wiki/Exponential_smoothing
		//St = aX + (1-a)St-1
		float altitudeCmAglTemp = (float) (altitudeMslCm - model->InitialAltitudeCm());
		float previousAltitudeCmAgl = model->ZNEDLocalFrameCm();
		float currentAltitudeCmAgl = WEIGHT * (altitudeCmAglTemp) + (1 - WEIGHT)*previousAltitudeCmAgl;
		//model->AltitudeMetersAgl(currentAltitudeMetersAgl);
	
		//Calculate altitude speed.
		//model->ZVelocityFRDCms(((currentAltitudeCmAgl - previousAltitudeCmAgl) * BAROMETER_SENSOR_READ_PERIOD));
		model->ZVelocityFRDCms(((currentAltitudeCmAgl - previousAltitudeCmAgl) * barometerSensorReadPeriod));
	
		
		model->ZNEDLocalFrameCm(currentAltitudeCmAgl); //don't use localnedz as the z value because the altitude from barometer is more accurate
	}
					

	
	
				
	/**
	* Convert position to local NED
	*/
	float localNEDX = 0.0;
	float localNEDY = 0.0;
	float localNEDZ = 0.0;
	
	CoordinateUtil::ConvertFromECEFToLocalNED(model->InitialXPositionEcef(), model->InitialYPositionEcef(), model->InitialZPositionEcef(),
		model->XEcefCm(), model->YEcefCm(), model->ZEcefCm(), model->EcefToLocalNEDRotationMatrix,
		localNEDX, localNEDY, localNEDZ);
	
	model->XNEDLocalFrameCm(localNEDX);
	model->YNEDLocalFrameCm(localNEDY);
	//model->ZNEDLocalFrameCm(currentAltitudeCmAgl); //don't use localnedz as the z value because the altitude from barometer is more accurate
	
	
	
}

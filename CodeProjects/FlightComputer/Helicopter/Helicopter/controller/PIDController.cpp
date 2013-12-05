/*
 * PIDController.cpp
 *
 * Created: 9/22/2013 2:15:35 PM
 *  Author: HP User
 */ 

#include "PIDController.h"

using namespace helicopter::controller;

PIDController::PIDController(SystemModel *model, ServoDriver *servoDriver): 
model(model),
servoDriver(servoDriver),
yawIntegralGain(0),
yawDerivativeGain(0),
yawProportionalGain(0),
yawAntiWindupGain(0),
minYawServoControlValue(0),
maxYawServoControlValue(0),
yawServoTrim(0),
intervalPeriodSecs(0),
controlMaxValue(0),
controlMinValue(0)
{
	
}

PIDController::~PIDController()
{
	delete model;
}

double PIDController::calculateProportional(double currentValue, double referenceValue)
{
	return currentValue - referenceValue;
}

//TODO refactor to make this common for all PID calculations
double PIDController::calculateYawProportional(double currentYawDegrees, double referenceYawDegrees)
{
	double yawError = currentYawDegrees - referenceYawDegrees;
	
	//Convert 360 degree magnetic heading error to a +/- 180 mag heading error
	
	if (yawError >= 180)
	{
		yawError = yawError - 360;
	}
	else if (yawError < -180)
	{
		yawError = yawError + 360;
	}
	
	return yawError;
}


//TODO refactor to make this common for all PID calculations. I'll want to include the specific
//variables max values as parameters so I can 'generalize' it.
double PIDController::calculateYawIntegralAntiWindup(double oldYawControlPreServoAdj, double oldYawControl)
{
	double antiWindup = 0;
	
	antiWindup = yawAntiWindupGain * (oldYawControlPreServoAdj - oldYawControl);
	
	/*
	if (oldYawControlValue > maxYawServoControlValue)
	{
		antiWindup = yawAntiWindupGain * (oldYawControlValue - maxYawServoControlValue);
	}else if (oldYawControlValue < minYawServoControlValue)
	{
		antiWindup = yawAntiWindupGain * (oldYawControlValue - minYawServoControlValue);
	}
	*/
	
	return antiWindup;
}


/**
 * Anti-windup algorithm provided by Control Systems Design by Karl Johan Astrom 2002. chapter 6
 */
double PIDController::calculateYawIntegral(double yawProportionalDegrees, double oldYawIntegral, double yawAntiWindup)
{
	
	double workingIntegral = 0;
	
	workingIntegral = yawProportionalDegrees * intervalPeriodSecs * yawIntegralGain;
	
	//Integrate (i.e. sum this working value with the current integral value).
	//Note: i'm going out of order from what is defined in the book referenced above.
	//I am summing before subtracting the antiwindup value to make it easier.
	//I also find it odd that the integral 'gain' is being applied before 
	//accounting for the anti-windup. But this could be to compensate for large errors.
	workingIntegral = workingIntegral + oldYawIntegral;
	
	if (yawAntiWindup != 0)
	{
		//We want to know if the integral is greater than 0 or less than 0 so that when we subtract
		//the antiwindup value, we get closer to 0, and don't exceed 0.
		if (workingIntegral > 0 && yawAntiWindup > workingIntegral)
		{
			workingIntegral = 0;
		}else if (workingIntegral < 0 && yawAntiWindup < workingIntegral)
		{
			workingIntegral = 0;
		}
		
		if (workingIntegral != 0)
		{
				//Subtract the anti-windup value from the working integral. 
			workingIntegral = workingIntegral - yawAntiWindup;
		}
	}
	
	return workingIntegral;
}

//TODO refactor to make this common for all PID calculations
//double PIDController::calculateYawIntegral(double yawProportionalDegrees, double oldYawIntegral, double yawAntiWindup)
//{
	//double integral = 0;
	//
	//integral = oldYawIntegral + yawProportionalDegrees * intervalPeriodSecs;
	//
	//if (yawAntiWindup != 0)
	//{
		////We want to know if the integral is greater than 0 or less than 0 so that when we subtract
		////the antiwindup value, we get closer to 0, and don't exceed 0. 
		//if (integral > 0 && yawAntiWindup > integral)
		//{
			//integral = 0;
		//}else if (integral < 0 && yawAntiWindup < integral)
		//{
			//integral = 0;
		//}
		//
		//if (integral != 0)
		//{
			//integral = integral - yawAntiWindup;
		//}
	//}
	//
	//return integral;
//}


double PIDController::calculateYawVelocityError(double yawVelocityDegreesPerSecond, double referenceYawVelocityDegreesPerSecond)
{
	return yawVelocityDegreesPerSecond - referenceYawVelocityDegreesPerSecond;
}

double PIDController::calculateYawControlValue(double yawProportionalDegrees, double yawVelocityErrorDegreesPerSecond, double yawIntegral)
{
	double controlValue = 0;
	
	controlValue = yawIntegral  + yawProportionalDegrees * yawProportionalGain + yawVelocityErrorDegreesPerSecond * yawDerivativeGain;
	
	/*
	if (controlValue > controlMaxValue)
	{
		controlValue = controlMaxValue;
	}else if (controlValue < controlMinValue)
	{
		controlValue = controlMinValue;
	}
	*/
	
	return controlValue;
}


double PIDController::adjustControlForServoLimits( double controlValueToAdjust )
{
	double controlValue = controlValueToAdjust;
	
	//TODO: when generalizing ensure to change this value. 
	controlValue += yawServoTrim;
	
	if (controlValue > maxYawServoControlValue)
	{
		controlValue = maxYawServoControlValue;
	}else if (controlValue < minYawServoControlValue)
	{
		controlValue = minYawServoControlValue;
	}
	
	return controlValue;
}

void PIDController::tailRotorCollectiveOuterLoopUpdate()
{
	double yawProportional = calculateYawProportional(model->MagYawDegrees(), model->ReferenceMagYawDegrees());
	double yawAntiWindup = calculateYawIntegralAntiWindup(model->YawControlBeforeServoLimitsAdjustment(), model->YawControl());
	double weightedYawIntegral = calculateYawIntegral(yawProportional, model->YawIntegral(), yawAntiWindup);
	double yawDerivativeError = calculateYawVelocityError(model->YawVelocityDegreesPerSecond(), model->ReferenceYawVelocityDegreesPerSecond());
	double yawControlBeforeServoLimitsAdjustment = calculateYawControlValue(yawProportional, yawDerivativeError, weightedYawIntegral);
	double yawControl = adjustControlForServoLimits(yawControlBeforeServoLimitsAdjustment);
	
	model->YawControl(yawControl);
	model->YawControlBeforeServoLimitsAdjustment(yawControlBeforeServoLimitsAdjustment);
	model->YawIntegral(weightedYawIntegral);
	model->YawProportional(yawProportional);
	model->YawDerivativeError(yawDerivativeError);
	
	servoDriver->controlTailRotorCollective(yawControl);
}

void PIDController::addBlownFrame()
{
	model->BlownFrames(model->BlownFrames() + 1);
}

/*
 * PIDController.cpp
 *
 * Created: 9/22/2013 2:15:35 PM
 *  Author: HP User
 */ 

#include "PIDController.h"

using namespace helicopter::controller;

PIDController::PIDController(SystemModel *model): 
model(model),
yawIntegralGain(0),
yawDerivativeGain(0),
yawProportionalGain(0),
yawAntiWindupGain(0),
minYawServoControlValue(0),
maxYawServoControlValue(0),
intervalPeriodSecs(0),
controlMaxValue(0),
controlMinValue(0)
{
	
}

PIDController::~PIDController()
{
	delete model;
}

//TODO refactor to make this common for all PID calculations
double PIDController::calculateYawProportional(double currentYaw, double referenceYaw)
{
	double yawError = currentYaw - referenceYaw;
	
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
double PIDController::calculateYawIntegralAntiWindup(double oldYawControlValue)
{
	double antiWindup = 0;
	
	if (oldYawControlValue > maxYawServoControlValue)
	{
		antiWindup = yawAntiWindupGain * (oldYawControlValue - maxYawServoControlValue);
	}else if (oldYawControlValue < minYawServoControlValue)
	{
		antiWindup = yawAntiWindupGain * (oldYawControlValue - minYawServoControlValue);
	}
	
	return antiWindup;
}

//TODO refactor to make this common for all PID calculations
double PIDController::calculateYawIntegral(double yawProportional, double oldYawIntegral, double yawAntiWindup)
{
	double integral = 0;
	
	integral = oldYawIntegral + yawProportional * intervalPeriodSecs;
	
	if (yawAntiWindup != 0)
	{
		//We want to know if the integral is greater than 0 or less than 0 so that when we subtract
		//the antiwindup value, we get closer to 0, and don't exceed 0. 
		if (integral > 0 && yawAntiWindup > integral)
		{
			integral = 0;
		}else if (integral < 0 && yawAntiWindup < integral)
		{
			integral = 0;
		}
		
		if (integral != 0)
		{
			integral = integral - yawAntiWindup;
		}
	}
	
	return integral;
}


double PIDController::calculateYawDerivativeError(double yawVelocity, double referenceYawVelocity)
{
	return yawVelocity - referenceYawVelocity;
}

double PIDController::calculateYawControl(double yawProportional, double yawDerivativeError, double yawIntegral)
{
	double controlValue = 0;
	
	controlValue = yawIntegral * yawIntegralGain + yawProportional * yawProportionalGain + yawDerivativeError * yawDerivativeGain;
	
	if (controlValue > controlMaxValue)
	{
		controlValue = controlMaxValue;
	}else if (controlValue < controlMinValue)
	{
		controlValue = controlMinValue;
	}
	
	return controlValue;
}
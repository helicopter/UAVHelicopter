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

xIntegralGain(0),
xDerivativeGain(0),
xProportionalGain(0),
xAntiWindupGain(0),
longitudeInnerLoopGain(0),

yIntegralGain(0),
yDerivativeGain(0),
yProportionalGain(0),
yAntiWindupGain(0),
lateralInnerLoopGain(0),

zIntegralGain(0),
zDerivativeGain(0),
zProportionalGain(0),
zAntiWindupGain(0),


minYawServoControlValue(0),
maxYawServoControlValue(0),
minLongitudeServoControlValue(0),
maxLongitudeServoControlValue(0),
minLateralServoControlValue(0),
maxLateralServoControlValue(0),
minMainRotorServoControlValue(0),
maxMainRotorServoControlValue(0),

intervalPeriodSecs(0),
controlMaxValue(0),
controlMinValue(0)
{
	
}

PIDController::~PIDController()
{
	delete model;
}

float PIDController::calculateProportional(float currentValue, float referenceValue)
{
	return currentValue - referenceValue;
}

//TODO refactor to make this common for all PID calculations
/*float PIDController::calculateProportional(float currentYawDegrees, float referenceYawDegrees)
{
	float yawError = currentYawDegrees - referenceYawDegrees;
	
	//Convert 360 degree magnetic heading error to a +/- 180 mag heading error
	
	yawError = convertYawErrorFrom360to180(yawError);

	
	return yawError;
}*/


//TODO refactor to make this common for all PID calculations. I'll want to include the specific
//variables max values as parameters so I can 'generalize' it.
float PIDController::calculateIntegralAntiWindup(float oldControlPreServoAdj, float oldControl, float antiWindupGain)
{
	float antiWindup = 0;
	
	antiWindup = antiWindupGain * (oldControlPreServoAdj - oldControl);
	
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
float PIDController::calculateIntegral(float proportional, float oldIntegral, float antiWindup, float integralGain)
{
	
	float workingIntegral = 0;
	
	workingIntegral = proportional * intervalPeriodSecs * integralGain;
	
	//Integrate (i.e. sum this working value with the current integral value).
	//Note: i'm going out of order from what is defined in the book referenced above.
	//I am summing before subtracting the antiwindup value to make it easier.
	//I also find it odd that the integral 'gain' is being applied before 
	//accounting for the anti-windup. But this could be to compensate for large errors.
	workingIntegral = workingIntegral + oldIntegral;
	
	if (antiWindup != 0)
	{
		//We want to know if the integral is greater than 0 or less than 0 so that when we subtract
		//the antiwindup value, we get closer to 0, and don't exceed 0.
		if (workingIntegral > 0 && antiWindup > workingIntegral)
		{
			workingIntegral = 0;
		}else if (workingIntegral < 0 && antiWindup < workingIntegral)
		{
			workingIntegral = 0;
		}
		
		if (workingIntegral != 0)
		{
				//Subtract the anti-windup value from the working integral. 
			workingIntegral = workingIntegral - antiWindup;
		}
	}
	
	return workingIntegral;
}

//TODO refactor to make this common for all PID calculations
//float PIDController::calculateIntegral(float yawProportionalDegrees, float oldYawIntegral, float yawAntiWindup)
//{
	//float integral = 0;
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


float PIDController::calculateVelocityError(float currentVelocity, float referenceVelocity)
{
	return currentVelocity - referenceVelocity;
}

float PIDController::calculateOuterLoopControlValue(float proportionalError, float velocityError, float integral, float proportionalGain, float derivativeGain)
{
	float controlValue = 0;
	
	controlValue = integral  + proportionalError * proportionalGain + velocityError * derivativeGain;
	
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


float PIDController::adjustControlForServoLimits( float controlValueToAdjust, float minServoControlValue, float maxServoControlValue )
{

	//TODO: when generalizing ensure to change this value. 
	//Todo revisit including trim.
	//controlValue += yawServoTrim;
	
	if (controlValueToAdjust > maxServoControlValue)
	{
		controlValueToAdjust = maxServoControlValue;
	}else if (controlValueToAdjust < minServoControlValue)
	{
		controlValueToAdjust = minServoControlValue;
	}
	
	return controlValueToAdjust;
}


float calculateInnerLoopControlValue( float outerLoopSetpoint, float measuredValue, float gain )
{
	//NOTE: IN MY OTHER IMPLEMENTATION I SUBTRACT THE VELOCITY (YES VELOCITY) OF THE ANGULAR MOTION.
	//return gain * (measuredValue - outerLoopSetpoint);
	
	///************* I JUST CHANGED THIS
	return gain * (outerLoopSetpoint - measuredValue);
}


void PIDController::tailRotorCollectiveOuterLoopUpdate()
{
	float yawProportional = calculateProportional(model->MagYawDegrees(), model->ReferenceMagYawDegrees());
	
	yawProportional = convertYawErrorFrom360to180(yawProportional);
	
	float yawIntegralAntiWindup = calculateIntegralAntiWindup(model->YawControlBeforeServoLimitsAdjustment(), model->YawControl(), yawAntiWindupGain);
	float weightedYawIntegral = calculateIntegral(yawProportional, model->YawIntegral(), yawIntegralAntiWindup, yawIntegralGain);
	float yawDerivativeError = calculateVelocityError(model->YawVelocityDegreesPerSecond(), model->ReferenceYawVelocityDegreesPerSecond());
	float yawControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(yawProportional, yawDerivativeError, weightedYawIntegral, yawProportionalGain, yawDerivativeGain);
	float yawControl = adjustControlForServoLimits(yawControlBeforeServoLimitsAdjustment, minYawServoControlValue, maxYawServoControlValue);
	
	model->YawControl(yawControl);
	model->YawControlBeforeServoLimitsAdjustment(yawControlBeforeServoLimitsAdjustment);
	model->YawIntegral(weightedYawIntegral);
	model->YawProportional(yawProportional);
	model->YawDerivativeError(yawDerivativeError);
}

void PIDController::mainRotorCollectiveOuterLoopUpdate()
{
	//I JUST CHANGED THIS
	//float zProportional = calculateProportional(model->AltitudeFeetAgl(), model->ReferenceAltitudeFeet());
	float zProportional = calculateProportional(model->ZNEDBodyFrame(), model->ReferenceZNEDBodyFrameFeet());
	
	float zIntegralAntiWindup = calculateIntegralAntiWindup(model->MainRotorControlBeforeServoLimitsAdjustment(), model->MainRotorCollectiveControl(), zAntiWindupGain);
	float weightedZIntegral = calculateIntegral(zProportional, model->ZIntegral(), zIntegralAntiWindup, zIntegralGain);
	float zDerivativeError = calculateVelocityError(model->ZVelocityFeetPerSecond(), model->ReferenceZVelocityFeetPerSecond());
	float mainRotorControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(zProportional, zDerivativeError, weightedZIntegral, zProportionalGain, zDerivativeGain);
	float mainRotorControl = adjustControlForServoLimits(mainRotorControlBeforeServoLimitsAdjustment, minMainRotorServoControlValue, maxMainRotorServoControlValue);
	
	model->MainRotorCollectiveControl(mainRotorControl);
	model->MainRotorControlBeforeServoLimitsAdjustment(mainRotorControlBeforeServoLimitsAdjustment);
	model->ZIntegral(weightedZIntegral);
	model->ZProportional(zProportional);
	model->ZDerivativeError(zDerivativeError);
}

void PIDController::cyclicLongitudeOuterLoopUpdate()
{
	float xProportional = calculateProportional(model->XNEDBodyFrame(), model->ReferenceXNEDBodyFrame());
	float xIntegralAntiWindup = calculateIntegralAntiWindup(model->LongitudeControlBeforeServoLimitsAdjustment(), model->LongitudeControl(), xAntiWindupGain);
	float weightedXIntegral = calculateIntegral(xProportional, model->XIntegral(), xIntegralAntiWindup, xIntegralGain);
	float xDerivativeError = calculateVelocityError(model->XVelocityMetersPerSecond(), model->ReferenceXVelocityMetersPerSecond());
	float xLongitudinalOuterLoopSetpoint = calculateOuterLoopControlValue(xProportional, xDerivativeError, weightedXIntegral, xProportionalGain, xDerivativeGain);
	
	xLongitudinalOuterLoopSetpoint = adjustForSetpointLimits(xLongitudinalOuterLoopSetpoint, MIN_PITCH_SETPOINT_DEGREES, MAX_PITCH_SETPOINT_DEGREES);

	model->XLongitudeOuterLoopSetpoint(xLongitudinalOuterLoopSetpoint);
	model->XIntegral(weightedXIntegral);
	model->XProportional(xProportional);
	model->XDerivativeError(xDerivativeError);
}

void PIDController::cyclicLateralOuterLoopUpdate()
{
	float yProportional = calculateProportional(model->YNEDBodyFrame(), model->ReferenceYNEDBodyFrame());
	float yIntegralAntiWindup = calculateIntegralAntiWindup(model->LateralControlBeforeServoLimitsAdjustment(), model->LateralControl(), yAntiWindupGain);
	float weightedYIntegral = calculateIntegral(yProportional, model->YIntegral(), yIntegralAntiWindup, yIntegralGain);
	float yDerivativeError = calculateVelocityError(model->YVelocityMetersPerSecond(), model->ReferenceYVelocityMetersPerSecond());
	float yLateralOuterLoopSetpoint = calculateOuterLoopControlValue(yProportional, yDerivativeError, weightedYIntegral, yProportionalGain, yDerivativeGain);

	yLateralOuterLoopSetpoint = adjustForSetpointLimits(yLateralOuterLoopSetpoint, MIN_ROLL_SETPOINT_DEGREES, MAX_ROLL_SETPOINT_DEGREES);
	
	model->YLateralOuterLoopSetpoint(yLateralOuterLoopSetpoint);
	model->YIntegral(weightedYIntegral);
	model->YProportional(yProportional);
	model->YDerivativeError(yDerivativeError);
}

void PIDController::cyclicLongitudeInnerLoopUpdate()
{
	float xLongitudinalInnerLoopControlBeforeServoLimits = calculateInnerLoopControlValue(model->XLongitudeOuterLoopSetpoint(), model->ThetaPitchDegrees(), longitudeInnerLoopGain);
	
	float xLongitudinalInnerLoopControl = adjustControlForServoLimits(xLongitudinalInnerLoopControlBeforeServoLimits, minLongitudeServoControlValue, maxLongitudeServoControlValue);
	
	model->LongitudeControlBeforeServoLimitsAdjustment(xLongitudinalInnerLoopControlBeforeServoLimits);
	
	model->LongitudeControl(xLongitudinalInnerLoopControl);
}

void PIDController::cyclicLateralInnerLoopUpdate()
{
	float yLateralInnerLoopControlBeforeServoLimits = calculateInnerLoopControlValue(model->YLateralOuterLoopSetpoint(), model->PhiRollDegrees(), lateralInnerLoopGain);
	
	float yLateralInnerLoopControl = adjustControlForServoLimits(yLateralInnerLoopControlBeforeServoLimits, minLateralServoControlValue, maxLateralServoControlValue);
	
	model->LateralControlBeforeServoLimitsAdjustment(yLateralInnerLoopControlBeforeServoLimits);
	
	model->LateralControl(yLateralInnerLoopControl);
}



//TODO WTF IS THIS DOING HERE?? THIS IS THE WRONG CONTROLLER!!
void PIDController::addBlownFrame()
{
	model->BlownFrames(model->BlownFrames() + 1);
}

float PIDController::convertYawErrorFrom360to180( float yawError ) 
{
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

float PIDController::adjustForSetpointLimits( float outerLoopControlSetpoint, float minSetpointLimitDegrees, float maxSetpointLimitDegrees ) 
{
	if (outerLoopControlSetpoint < minSetpointLimitDegrees)
	{
		outerLoopControlSetpoint = minSetpointLimitDegrees;
	}else if (outerLoopControlSetpoint > maxSetpointLimitDegrees)
	{
		outerLoopControlSetpoint = maxSetpointLimitDegrees;
	}
	
	return outerLoopControlSetpoint;
}

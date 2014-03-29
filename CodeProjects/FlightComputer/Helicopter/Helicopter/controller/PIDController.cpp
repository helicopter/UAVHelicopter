/*
 * PIDController.cpp
 *
 * Created: 9/22/2013 2:15:35 PM
 *  Author: HP User
 */ 
#include <math.h>

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

//TODO refactor to make this common for all PID calculations. I'll want to include the specific
//variables max values as parameters so I can 'generalize' it.
float PIDController::calculateIntegralAntiWindup(float oldControlPreServoAdj, float oldControl, float antiWindupGain)
{
	float antiWindup = 0;
	
	antiWindup = antiWindupGain * (oldControlPreServoAdj - oldControl);
	
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
	//Note: I'm going out of order from what is defined in the book referenced above.
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

float PIDController::calculateVelocityError(float currentVelocity, float referenceVelocity)
{
	return currentVelocity - referenceVelocity;
}

float PIDController::calculateOuterLoopControlValue(float proportionalError, float velocityError, float integral, float proportionalGain, float derivativeGain, int directionFactor)
{
	float controlValue = 0;
	
	controlValue = directionFactor * (integral  + proportionalError * proportionalGain + velocityError * derivativeGain);
	
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


float calculateInnerLoopControlValue( float outerLoopSetpoint, float measuredValue, float gain, float angularVelocity, float angularVelocityGain )
{
	return (gain * (outerLoopSetpoint - measuredValue)) - (angularVelocity * angularVelocityGain);
}


float PIDController::adjustForSetpointLimits( float outerLoopControlSetpoint, float minSetpointLimitRads, float maxSetpointLimitRads )
{
	if (outerLoopControlSetpoint < minSetpointLimitRads)
	{
		outerLoopControlSetpoint = minSetpointLimitRads;
	}else if (outerLoopControlSetpoint > maxSetpointLimitRads)
	{
		outerLoopControlSetpoint = maxSetpointLimitRads;
	}
	
	return outerLoopControlSetpoint;
}



//TODO WTF IS THIS DOING HERE?? THIS IS THE WRONG CONTROLLER!!
void PIDController::addBlownFrame()
{
	model->BlownFrames(model->BlownFrames() + 1);
}

/**
 * Converts the error from 0-360, to -180 - 180 (in the equivalent of rads - i.e. yawError should be rads).
 */
float PIDController::convertYawErrorFrom2PitoPlusMinusPi( float yawErrorRads )
{
	if (yawErrorRads >= M_PI)
	{
		yawErrorRads = yawErrorRads - (2 * M_PI);
	}
	else if (yawErrorRads < -M_PI)
	{
		yawErrorRads = yawErrorRads + (2 * M_PI); 
	}
	
	return yawErrorRads;
}


void PIDController::tailRotorCollectiveOuterLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		float yawProportional = calculateProportional(model->YawRads(), model->ReferenceMagYawRads());
		
		yawProportional = convertYawErrorFrom2PitoPlusMinusPi(yawProportional);
		
		float yawIntegralAntiWindup = calculateIntegralAntiWindup(model->YawControlBeforeServoLimitsAdjustment(), model->YawControl(), yawAntiWindupGain);
		float weightedYawIntegral = calculateIntegral(yawProportional, model->YawIntegral(), yawIntegralAntiWindup, yawIntegralGain);
		float yawDerivativeError = calculateVelocityError(model->YawAngularVelocityRadsPerSecond(), model->ReferenceYawVelocityRadsPerSecond());
		float yawControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(yawProportional, yawDerivativeError, weightedYawIntegral, yawProportionalGain, yawDerivativeGain, 1);
		float yawControl = adjustControlForServoLimits(yawControlBeforeServoLimitsAdjustment, minYawServoControlValue, maxYawServoControlValue);
		
		model->YawControl(yawControl);
		model->YawControlBeforeServoLimitsAdjustment(yawControlBeforeServoLimitsAdjustment);
		model->YawIntegral(weightedYawIntegral);
		model->YawProportional(yawProportional);
		model->YawDerivativeError(yawDerivativeError);
	}
}

void PIDController::mainRotorCollectiveOuterLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		float zProportional = calculateProportional(model->ZNEDLocalFrame(), model->ReferenceZNEDLocalFrameMeters());
	
		float zIntegralAntiWindup = calculateIntegralAntiWindup(model->MainRotorControlBeforeServoLimitsAdjustment(), model->MainRotorCollectiveControl(), zAntiWindupGain);
		float weightedZIntegral = calculateIntegral(zProportional, model->ZIntegral(), zIntegralAntiWindup, zIntegralGain);
		float zDerivativeError = calculateVelocityError(model->ZVelocityMetersPerSecond(), model->ReferenceZVelocityMetersPerSecond());
		float mainRotorControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(zProportional, zDerivativeError, weightedZIntegral, zProportionalGain, zDerivativeGain, 1);
		float mainRotorControl = adjustControlForServoLimits(mainRotorControlBeforeServoLimitsAdjustment, minMainRotorServoControlValue, maxMainRotorServoControlValue);
	
		model->MainRotorCollectiveControl(mainRotorControl);
		model->MainRotorControlBeforeServoLimitsAdjustment(mainRotorControlBeforeServoLimitsAdjustment);
		model->ZIntegral(weightedZIntegral);
		model->ZProportional(zProportional);
		model->ZDerivativeError(zDerivativeError);
	}
}

void PIDController::cyclicLongitudeOuterLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		float xProportional = calculateProportional(model->XNEDLocalFrame(), model->ReferenceXNEDLocalFrame());
		float xIntegralAntiWindup = calculateIntegralAntiWindup(model->LongitudeControlBeforeServoLimitsAdjustment(), model->LongitudeControl(), xAntiWindupGain);
		float weightedXIntegral = calculateIntegral(xProportional, model->XIntegral(), xIntegralAntiWindup, xIntegralGain);
		float xDerivativeError = calculateVelocityError(model->XVelocityMetersPerSecond(), model->ReferenceXVelocityMetersPerSecond());
		float xLongitudinalOuterLoopSetpoint = calculateOuterLoopControlValue(xProportional, xDerivativeError, weightedXIntegral, xProportionalGain, xDerivativeGain, 1);
	
		xLongitudinalOuterLoopSetpoint = adjustForSetpointLimits(xLongitudinalOuterLoopSetpoint, minPitchSetpointRads, maxPitchSetpointRads);

		model->XLongitudeOuterLoopSetpoint(xLongitudinalOuterLoopSetpoint);
		model->XIntegral(weightedXIntegral);
		model->XProportional(xProportional);
		model->XDerivativeError(xDerivativeError);
	}
}

void PIDController::cyclicLateralOuterLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		float yProportional = calculateProportional(model->YNEDLocalFrame(), model->ReferenceYNEDLocalFrame());
		float yIntegralAntiWindup = calculateIntegralAntiWindup(model->LateralControlBeforeServoLimitsAdjustment(), model->LateralControl(), yAntiWindupGain);
		float weightedYIntegral = calculateIntegral(yProportional, model->YIntegral(), yIntegralAntiWindup, yIntegralGain);
		float yDerivativeError = calculateVelocityError(model->YVelocityMetersPerSecond(), model->ReferenceYVelocityMetersPerSecond());
	
		//we use a -1 direction factor because unlike other controls, if we have a positive proportional error, we actually need a 'negative' desired 
		//roll setpoint in order for the helicopter to track back towards the desired position. So if the helicopter is directly 'east' of the desired position,
		//the proportional error will correctly be positive (in a North-east-down coordinate system), but the helicopter will have to roll counter clockwise
		//to return to the desired position. This differs from if the helicopter was directly north of the desired position. The x proportional would be positive
		//(in NED), but it would take a positive pitch angle setpoint to get the helicopter to return to the desired position.
		float yLateralOuterLoopSetpoint = calculateOuterLoopControlValue(yProportional, yDerivativeError, weightedYIntegral, yProportionalGain, yDerivativeGain, -1);

		yLateralOuterLoopSetpoint = adjustForSetpointLimits(yLateralOuterLoopSetpoint, minRollSetpointRads, maxRollSetpointRads);
	
		model->YLateralOuterLoopSetpoint(yLateralOuterLoopSetpoint);
		model->YIntegral(weightedYIntegral);
		model->YProportional(yProportional);
		model->YDerivativeError(yDerivativeError);
	}
}

void PIDController::cyclicLongitudeInnerLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		float xLongitudinalInnerLoopControlBeforeServoLimits = calculateInnerLoopControlValue(model->XLongitudeOuterLoopSetpoint(), model->PitchRads(), longitudeInnerLoopGain, model->PitchAngularVelocityRadsPerSecond(), pitchAngularVelocityGain);
	
		float xLongitudinalInnerLoopControl = adjustControlForServoLimits(xLongitudinalInnerLoopControlBeforeServoLimits, minLongitudeServoControlValue, maxLongitudeServoControlValue);
	
		model->LongitudeControlBeforeServoLimitsAdjustment(xLongitudinalInnerLoopControlBeforeServoLimits);
	
		model->LongitudeControl(xLongitudinalInnerLoopControl);
	}
}

void PIDController::cyclicLateralInnerLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		float yLateralInnerLoopControlBeforeServoLimits = calculateInnerLoopControlValue(model->YLateralOuterLoopSetpoint(), model->RollRads(), lateralInnerLoopGain, model->RollAngularVelocityRadsPerSecond(), rollAngularVelocityGain);
		
		float yLateralInnerLoopControl = adjustControlForServoLimits(yLateralInnerLoopControlBeforeServoLimits, minLateralServoControlValue, maxLateralServoControlValue);
		
		model->LateralControlBeforeServoLimitsAdjustment(yLateralInnerLoopControlBeforeServoLimits);
		
		model->LateralControl(yLateralInnerLoopControl);		
	}
}



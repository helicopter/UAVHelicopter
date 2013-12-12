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

double PIDController::calculateProportional(double currentValue, double referenceValue)
{
	return currentValue - referenceValue;
}

//TODO refactor to make this common for all PID calculations
/*double PIDController::calculateProportional(double currentYawDegrees, double referenceYawDegrees)
{
	double yawError = currentYawDegrees - referenceYawDegrees;
	
	//Convert 360 degree magnetic heading error to a +/- 180 mag heading error
	
	yawError = convertYawErrorFrom360to180(yawError);

	
	return yawError;
}*/


//TODO refactor to make this common for all PID calculations. I'll want to include the specific
//variables max values as parameters so I can 'generalize' it.
double PIDController::calculateIntegralAntiWindup(double oldControlPreServoAdj, double oldControl, double antiWindupGain)
{
	double antiWindup = 0;
	
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
double PIDController::calculateIntegral(double proportional, double oldIntegral, double antiWindup, double integralGain)
{
	
	double workingIntegral = 0;
	
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
//double PIDController::calculateIntegral(double yawProportionalDegrees, double oldYawIntegral, double yawAntiWindup)
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


double PIDController::calculateVelocityError(double currentVelocity, double referenceVelocity)
{
	return currentVelocity - referenceVelocity;
}

double PIDController::calculateOuterLoopControlValue(double proportionalError, double velocityError, double integral, double proportionalGain, double derivativeGain)
{
	double controlValue = 0;
	
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


double PIDController::adjustControlForServoLimits( double controlValueToAdjust, double minServoControlValue, double maxServoControlValue )
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


double calculateInnerLoopControlValue( double outerLoopSetpoint, double measuredValue, double gain )
{
	//NOTE: IN MY OTHER IMPLEMENTATION I SUBTRACT THE VELOCITY (YES VELOCITY) OF THE ANGULAR MOTION.
	return gain * (measuredValue - outerLoopSetpoint);
}

//side motion / roll / lateral

//forward motion / pitch / longitud
void PIDController::cyclicLongitudeOuterLoopUpdate()
{
	//double yawProportional = calculateYawProportional(model->MagYawDegrees(), model->ReferenceMagYawDegrees());
	double xProportional = calculateProportional(model->XNEDBodyFrame(), model->ReferenceXNEDBodyFrame());
	double xIntegralAntiWindup = calculateIntegralAntiWindup(model->LongitudeControlBeforeServoLimitsAdjustment(), model->LongitudeControl(), xAntiWindupGain);
	double weightedXIntegral = calculateIntegral(xProportional, model->XIntegral(), xIntegralAntiWindup, xIntegralGain);
	double xDerivativeError = calculateVelocityError(model->XVelocityMetersPerSecond(), model->ReferenceXVelocityMetersPerSecond());
	double xLongitudinalOuterLoopControl = calculateOuterLoopControlValue(xProportional, xDerivativeError, weightedXIntegral, xProportionalGain, xDerivativeGain);
	//double xOuterLoopControl = adjustControlForServoLimits(xControlBeforeServoLimitsAdjustment);
	
	model->XLongitudinalOuterLoopControl(xLongitudinalOuterLoopControl);
	//model->YawControlBeforeServoLimitsAdjustment(yawControlBeforeServoLimitsAdjustment);
	model->XIntegral(weightedXIntegral);
	model->XProportional(xProportional);
	model->XDerivativeError(xDerivativeError);
	
//	servoDriver->controlTailRotorCollective(xControl);
}

void PIDController::cyclicLateralOuterLoopUpdate()
{
	//double yawProportional = calculateYawProportional(model->MagYawDegrees(), model->ReferenceMagYawDegrees());
	double yProportional = calculateProportional(model->YNEDBodyFrame(), model->ReferenceYNEDBodyFrame());
	double yIntegralAntiWindup = calculateIntegralAntiWindup(model->LateralControlBeforeServoLimitsAdjustment(), model->LateralControl(), yAntiWindupGain);
	double weightedYIntegral = calculateIntegral(yProportional, model->YIntegral(), yIntegralAntiWindup, yIntegralGain);
	double yDerivativeError = calculateVelocityError(model->YVelocityMetersPerSecond(), model->ReferenceYVelocityMetersPerSecond());
	double yLateralOuterLoopControl = calculateOuterLoopControlValue(yProportional, yDerivativeError, weightedYIntegral, yProportionalGain, yDerivativeGain);
	//double xOuterLoopControl = adjustControlForServoLimits(xControlBeforeServoLimitsAdjustment);
	
	model->YLateralOuterLoopControl(yLateralOuterLoopControl);
	//model->YawControlBeforeServoLimitsAdjustment(yawControlBeforeServoLimitsAdjustment);
	model->YIntegral(weightedYIntegral);
	model->YProportional(yProportional);
	model->YDerivativeError(yDerivativeError);
	
	//	servoDriver->controlTailRotorCollective(xControl);
}

void PIDController::cyclicLongitudeInnerLoopUpdate()
{
	double xLongitudinalInnerLoopControlBeforeServoLimits = calculateInnerLoopControlValue(model->XLongitudinalOuterLoopControl(), model->ThetaPitchDegrees(), longitudeInnerLoopGain);
	
	double xLongitudinalInnerLoopControl = adjustControlForServoLimits(xLongitudinalInnerLoopControlBeforeServoLimits, minLongitudeServoControlValue, maxLongitudeServoControlValue);
	
	model->LongitudeControlBeforeServoLimitsAdjustment(xLongitudinalInnerLoopControlBeforeServoLimits);
	
	model->LongitudeControl(xLongitudinalInnerLoopControl);
	
	servoDriver->controlLongitudinal(xLongitudinalInnerLoopControl);
}

void PIDController::cyclicLateralInnerLoopUpdate()
{
	double yLateralInnerLoopControlBeforeServoLimits = calculateInnerLoopControlValue(model->YLateralOuterLoopControl(), model->PhiRollDegrees(), lateralInnerLoopGain);
	
	double yLateralInnerLoopControl = adjustControlForServoLimits(yLateralInnerLoopControlBeforeServoLimits, minLateralServoControlValue, maxLateralServoControlValue);
	
	model->LateralControlBeforeServoLimitsAdjustment(yLateralInnerLoopControlBeforeServoLimits);
	
	model->LateralControl(yLateralInnerLoopControl);
	
	servoDriver->controlLongitudinal(yLateralInnerLoopControl);
}

void PIDController::tailRotorCollectiveOuterLoopUpdate()
{
	double yawProportional = calculateProportional(model->MagYawDegrees(), model->ReferenceMagYawDegrees());
	
	yawProportional = convertYawErrorFrom360to180(yawProportional);
	
	double yawIntegralAntiWindup = calculateIntegralAntiWindup(model->YawControlBeforeServoLimitsAdjustment(), model->YawControl(), yawAntiWindupGain);
	double weightedYawIntegral = calculateIntegral(yawProportional, model->YawIntegral(), yawIntegralAntiWindup, yawIntegralGain);
	double yawDerivativeError = calculateVelocityError(model->YawVelocityDegreesPerSecond(), model->ReferenceYawVelocityDegreesPerSecond());
	double yawControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(yawProportional, yawDerivativeError, weightedYawIntegral, yawProportionalGain, yawDerivativeGain);
	double yawControl = adjustControlForServoLimits(yawControlBeforeServoLimitsAdjustment, minYawServoControlValue, maxYawServoControlValue);
	
	model->YawControl(yawControl);
	model->YawControlBeforeServoLimitsAdjustment(yawControlBeforeServoLimitsAdjustment);
	model->YawIntegral(weightedYawIntegral);
	model->YawProportional(yawProportional);
	model->YawDerivativeError(yawDerivativeError);
	
	servoDriver->controlTailRotorCollective(yawControl);
}

void PIDController::mainRotorCollectiveOuterLoopUpdate()
{
	double zProportional = calculateProportional(model->AltitudeFeet(), model->ReferenceAltitudeFeet());
	
	double zIntegralAntiWindup = calculateIntegralAntiWindup(model->MainRotorControlBeforeServoLimitsAdjustment(), model->MainRotorCollectiveControl(), zAntiWindupGain);
	double weightedZIntegral = calculateIntegral(zProportional, model->ZIntegral(), zIntegralAntiWindup, zIntegralGain);
	double zDerivativeError = calculateVelocityError(model->ZVelocityFeetPerSecond(), model->ReferenceZVelocityFeetPerSecond());
	double mainRotorControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(zProportional, zDerivativeError, weightedZIntegral, zProportionalGain, zDerivativeGain);
	double mainRotorControl = adjustControlForServoLimits(mainRotorControlBeforeServoLimitsAdjustment, minMainRotorServoControlValue, maxMainRotorServoControlValue);
	
	model->MainRotorCollectiveControl(mainRotorControl);
	model->MainRotorControlBeforeServoLimitsAdjustment(mainRotorControlBeforeServoLimitsAdjustment);
	model->ZIntegral(weightedZIntegral);
	model->ZProportional(zProportional);
	model->ZDerivativeError(zDerivativeError);
	
	servoDriver->controlMainRotorCollective(mainRotorControl);
}



//TODO WTF IS THIS DOING HERE?? THIS IS THE WRONG CONTROLLER!!
void PIDController::addBlownFrame()
{
	model->BlownFrames(model->BlownFrames() + 1);
}

double PIDController::convertYawErrorFrom360to180( double yawError ) 
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

/*
 * PIDController.cpp
 *
 * Created: 9/22/2013 2:15:35 PM
 *  Author: HP User
 */ 
#include <math.h>

#include "PIDController.h"
#include "MatrixUtil.h"

using namespace helicopter::controller;
using namespace helicopter::util;

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
	
	//antiWindup = antiWindupGain * (oldControlPreServoAdj - oldControl);
	antiWindup = antiWindupGain * (oldControl - oldControlPreServoAdj);
	
	return antiWindup;
}


/**
 * Anti-windup algorithms are discussed in Control Systems Design by Karl Johan Astrom 2002. chapter 6
 * I'm not directly using their algorithms
 */
float PIDController::calculateIntegral(float proportional, float oldIntegral, float antiWindup, float integralGain)
{
	
	float workingIntegral = 0;
	
	
	workingIntegral = proportional * integralGain + antiWindup;
	
	
	workingIntegral = workingIntegral * intervalPeriodSecs + oldIntegral;
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//float workingIntegral = 0;
	//
//
	//workingIntegral = proportional * intervalPeriodSecs * integralGain;
	//workingIntegral = workingIntegral + oldIntegral;
	//
	///*
	//if (antiWindup == 0)
	//{
		////If the servos haven't been saturated (theres no anti-windup), then calculate the integral as normal.
		//workingIntegral = proportional * intervalPeriodSecs * integralGain;
		//workingIntegral = workingIntegral + oldIntegral;
	//} else*/
	//if (antiWindup != 0)
	//{
		///**
		 //* Take the absolute values of the working integral and the anti-windup so that the
		 //* anti windup is always subtracted from the integral. Then convert the integral back to a
		 //* negative value if it was already a negative value.
		 //*/
		//float absWorkingIntegral = abs(workingIntegral);
		//float absAntiWindup = abs(antiWindup);
		//
		////If the anti windup value is greater than the integral, just set the integral to 0, and if the working integral
		////is already 0, leave it at zero. i.e. don't subtract out the anti-windup value from a nonexistent integral.
		//if (absAntiWindup > absWorkingIntegral || absWorkingIntegral == 0)
		//{
			//workingIntegral = 0;
		//}else
		//{
			//absWorkingIntegral = absWorkingIntegral - absAntiWindup;
			//
			//if (workingIntegral < 0)
			//{
				//workingIntegral = absWorkingIntegral * -1;
			//}
		//}
		//
		//
		//
		////We want to know if the integral is greater than 0 or less than 0 so that when we subtract
		////the antiwindup value, we get closer to 0, and don't exceed 0.
		///*if (workingIntegral > 0 && antiWindup > workingIntegral)
		//{
			//workingIntegral = 0;
		//}else if (workingIntegral < 0 && antiWindup < workingIntegral)
		//{
			//workingIntegral = 0;
		//}
		//
		//if (workingIntegral != 0)
		//{
			////Subtract the anti-windup value from the working integral. 
			//workingIntegral = workingIntegral - antiWindup;
		//}*/
	//}
	
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
	
	
	
	//float workingPid = integral  + proportionalError * proportionalGain;
	//
	//float multiplier = workingPid < 0 ? -1.0f : 1.0f;
	//
	////Subtract the abs pid from abs velocity. therefore velocity always 'reduces' the pid value.
	//return directionFactor * (multiplier * (fabs(workingPid) - fabs(velocityError * derivativeGain)));
	
	/*
	float controlValue = 0;
	
	controlValue = directionFactor * (integral  + proportionalError * proportionalGain + velocityError * derivativeGain);
	
	return controlValue;
	*/
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
	
	//float workingSetpoint = gain * (outerLoopSetpoint - measuredValue);
	//float multiplier = workingSetpoint < 0 ? -1.0f : 1.0f;
	//
	////Subtract the abs values to ensure that one always 'subtracts' from the other.
	//return multiplier * (fabs(workingSetpoint) - fabs(angularVelocity*angularVelocityGain));
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




void PIDController::outerLoopUpdate()
{
	if (model->OperationalState() == SystemModel::AutoPilot)
	{
		//Calculate the position error
		float xErrorNED = calculateProportional(model->XNEDLocalFrameCm(), model->ReferenceXNEDLocalFrameCm());
		float yErrorNED = calculateProportional(model->YNEDLocalFrameCm(), model->ReferenceYNEDLocalFrameCm());
		float zErrorNED = calculateProportional(model->ZNEDLocalFrameCm(), model->ReferenceZNEDLocalFrameCm());
	
	
		//convert position error from NED to FRD Body frame.
		float bodyFrameErrors[3] = {};
		float nedToBodyFrameRotationMatrix[3][3] = {};
		float positionErrorsLocalNED[3] = {xErrorNED, yErrorNED, zErrorNED};
	
		MatrixUtil::CreateRotationMatrixTransposed(model->RollRads(), model->PitchRads(), model->YawRads(), nedToBodyFrameRotationMatrix);
		MatrixUtil::RotateMatrix(nedToBodyFrameRotationMatrix, positionErrorsLocalNED, bodyFrameErrors);
	

		this->cyclicLongitudeOuterLoopUpdate(bodyFrameErrors[0]);
		this->cyclicLateralOuterLoopUpdate(bodyFrameErrors[1]);	
//		this->mainRotorCollectiveOuterLoopUpdate(bodyFrameErrors[2]);	
		this->mainRotorCollectiveOuterLoopUpdate(zErrorNED);	//Just use regular error NED because altitude doesn't care about orientation.
		this->tailRotorCollectiveOuterLoopUpdate();
		
		/*
		this->cyclicLongitudeOuterLoopUpdate(xErrorNED);
		this->cyclicLateralOuterLoopUpdate(yErrorNED);
		this->mainRotorCollectiveOuterLoopUpdate(zErrorNED);	//Just use regular error NED because altitude doesn't care about orientation.
		this->tailRotorCollectiveOuterLoopUpdate();*/
	}
}


void PIDController::tailRotorCollectiveOuterLoopUpdate()
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

void PIDController::mainRotorCollectiveOuterLoopUpdate(float zProportional)
{

	//float zProportional = calculateProportional(model->ZNEDLocalFrame(), model->ReferenceZNEDLocalFrameMeters());
	
	float zIntegralAntiWindup = calculateIntegralAntiWindup(model->MainRotorControlBeforeServoLimitsAdjustment(), model->MainRotorCollectiveControl(), zAntiWindupGain);
	float weightedZIntegral = calculateIntegral(zProportional, model->ZIntegral(), zIntegralAntiWindup, zIntegralGain);
	float zDerivativeError = calculateVelocityError(model->ZVelocityFRDCms(), model->ReferenceZVelocityCms());
	float mainRotorControlBeforeServoLimitsAdjustment = calculateOuterLoopControlValue(zProportional, zDerivativeError, weightedZIntegral, zProportionalGain, zDerivativeGain, 1);
	float mainRotorControl = adjustControlForServoLimits(mainRotorControlBeforeServoLimitsAdjustment, minMainRotorServoControlValue, maxMainRotorServoControlValue);
	
	model->MainRotorCollectiveControl(mainRotorControl);
	model->MainRotorControlBeforeServoLimitsAdjustment(mainRotorControlBeforeServoLimitsAdjustment);
	model->ZIntegral(weightedZIntegral);
	model->ZProportional(zProportional);
	model->ZDerivativeError(zDerivativeError);
}

void PIDController::cyclicLongitudeOuterLoopUpdate(float xProportional)
{
	//float xProportional = calculateProportional(model->XNEDLocalFrame(), model->ReferenceXNEDLocalFrame());
	float xIntegralAntiWindup = calculateIntegralAntiWindup(model->LongitudeControlBeforeServoLimitsAdjustment(), model->LongitudeControl(), xAntiWindupGain);
	float weightedXIntegral = calculateIntegral(xProportional, model->XIntegral(), xIntegralAntiWindup, xIntegralGain);
	float xDerivativeError = calculateVelocityError(model->XVelocityFRDCms(), model->ReferenceXVelocityCms());
	float xLongitudinalOuterLoopSetpoint = calculateOuterLoopControlValue(xProportional, xDerivativeError, weightedXIntegral, xProportionalGain, xDerivativeGain, 1);

	
	
	xLongitudinalOuterLoopSetpoint = adjustForSetpointLimits(xLongitudinalOuterLoopSetpoint, minPitchSetpointRads, maxPitchSetpointRads);

	model->XLongitudeOuterLoopSetpoint(xLongitudinalOuterLoopSetpoint);
	model->XIntegral(weightedXIntegral);
	model->XProportional(xProportional);
	model->XDerivativeError(xDerivativeError);
}

void PIDController::cyclicLateralOuterLoopUpdate(float yProportional)
{
	//float yProportional = calculateProportional(model->YNEDLocalFrame(), model->ReferenceYNEDLocalFrame());
	float yIntegralAntiWindup = calculateIntegralAntiWindup(model->LateralControlBeforeServoLimitsAdjustment(), model->LateralControl(), yAntiWindupGain);
	float weightedYIntegral = calculateIntegral(yProportional, model->YIntegral(), yIntegralAntiWindup, yIntegralGain);
	float yDerivativeError = calculateVelocityError(model->YVelocityFRDCms(), model->ReferenceYVelocityCms());
	
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



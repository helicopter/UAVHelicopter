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
	
	//model->YawControl(yawControl);
	//Multiply by -1 because in the simulator, +1 results in counter clockwise yaw, but on the helicopter, it's a clockwise yaw. 
	model->YawControl(yawControl * -1);
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
	
//		model->LongitudeControl(xLongitudinalInnerLoopControl);

		//Multiply by -1 because in the simulator, a control value of +1 results in the helicopter pitching up, but the real helicopter pitches down.
		model->LongitudeControl(xLongitudinalInnerLoopControl * -1);
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



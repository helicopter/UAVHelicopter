/*
 * PIDController.h
 *
 * Created: 9/22/2013 2:11:38 PM
 *  Author: HP User
 */ 


#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include "SystemModel.h"
#include "ServoDriver.h"

using namespace helicopter::model;
using namespace helicopter::drivers;

namespace helicopter
{
	namespace controller
	{
		
		/**
		 * This class is the Proportional Integral Derivative (PID) controller which
		 * utilizes sensor data to derive control commands to control the servo actuators.
		 * This is the main class responsible for the helicopter's autonomous flight.
		 *
		 * References:
		 * PID controller: Linear and Nonlinear Control of SmallScaled Unmanned Helicopter
		 * 
		 */
		class PIDController
		{
			private:
				SystemModel *model;
				
				ServoDriver *servoDriver;
				
				double yawIntegralGain;
				double yawDerivativeGain;
				double yawProportionalGain;
				double yawAntiWindupGain;
					
				double minYawServoControlValue;
				double maxYawServoControlValue;
				
				double yawServoTrim;
					
				double intervalPeriodSecs;
					
				double controlMaxValue;
				double controlMinValue;
				
			public:
				PIDController (SystemModel *model, ServoDriver *servoDriver);
					
				~PIDController();
				
				/**
				 * Sets a trim value that should be applied to the yaw control value.
				 * This trim is applied before adjusting for servo limits.
				 * @param yawServoTrim A double value representing how much to adjust the
				 * trim by.
				 * TODO: identify what negative and positive means.
				 */
				void setYawServoTrim(double yawServoTrim)
				{
					this->yawServoTrim = yawServoTrim;
				}
				
				/**
				 * Gain value applied to the yaw proportional error
				 */
				void setYawProportionalGain(double yawProportionalGain)
				{
					this->yawProportionalGain = yawProportionalGain;
				}
				
				/**
				 * Gain value applied to the yaw integral
				 */
				void setYawIntegralGain(double yawIntegralGain)
				{
					this->yawIntegralGain = yawIntegralGain;
				}
				
				/**
				 * Gain value applied to the derivative
				 */
				void setYawDerivativeGain(double yawDerivativeGain)
				{
					this->yawDerivativeGain = yawDerivativeGain;
				}
				
				/**
				 * The number of seconds between pid controller updates. This number
				 * should generally be less than 1. Like .05 for a 20 hz rate.
				 */
				void setIntervalPeriodSecs(double intervalPeriodSecs)
				{
					this->intervalPeriodSecs = intervalPeriodSecs;
				}
				
				/**
				 * This gain represents how fast the integral is 'reset' (goes back to 0)
				 * when the actuators are saturated (I.e. we try to control the actuators beyond
				 * the maximum possible values, indicating that we are far away from the setpoint).
				 * This is used to prevent integral windup issues which happens when the integral
				 * keeps increasing despite the helicopter already at the maximum possible control values.
				 * @param yawAntiWindupGain The gain value which represents how fast the integral will
				 * go back to 0 when the servos are saturated.
				 * Some simple rule that have been suggested for the tracking time are ~ Tt = TiTd and Tt = (Ti + Td)/2.
				 */
				void setYawAntiWindupGain(double yawAntiWindupGain)
				{
					this->yawAntiWindupGain = yawAntiWindupGain;
				}
	
				/**
				 * The minimum control value that the yaw actuators can be set too. This is used for 
				 * integral anti-windup to determine when the actuators would be saturated.
				 * The value should be no smaller than the controlMinValue (e.g. no less than -1)
				 */
				void setMinTailRotorCollectiveControlValue (double minYawServoControlValue)
				{
					this->minYawServoControlValue = minYawServoControlValue;
				}
				
				/**
				 * The maximum control value that the yaw actuators can be set too. This is used for 
				 * integral anti-windup to determine when the actuators would be saturated.
				 * The value should be no larger than the controlMaxValue (e.g. no greater than 1)
				 */
				void setMaxTailRotorCollectiveValue(double maxYawServoControlValue)
				{
					this->maxYawServoControlValue = maxYawServoControlValue;
				}
				
				/**
				 * This is the maximum possible output value for a control signal.
				 * Generally, this should be set to 1.
				 */
				void setControlMaxValue(double controlMaxValue)
				{
					this->controlMaxValue = controlMaxValue;
				}
				
				/**
				 * This is the minimum possible value for a control signal.
				 * Generally, this should be set to -1.
				 */
				void setControlMinValue(double controlMinValue)
				{
					this->controlMinValue = controlMinValue;
				}			
				
				/**
				 * Calculates the proportional error between the current yaw value from the sensors
				 * and the reference yaw value from the mission planner.
				 * @param currentYawDegrees The current heading from the sensors where 0 = Magnetic North. This value should be in degrees between
				 * 0 - 359.99 (it may get set to 360 due to rounding)
				 * @param referenceYawDegrees The desired heading between 0-359.99 in degrees. Where 0 = Magnetic North.
				 * @return the error between the current yaw and the reference yaw (Gain is NOT applied) in degrees.
				 * This value will be between -180 - 179.99. This is because the larger the possible jump in
				 * error, the more it disturbs the calculations. So an error going from 0 then suddenly jumping to
				 * -360 would cause a large disturbance in the PID calculations. 
				 * Positive errors mean the helicopter needs to rotate counterclockwise to reduce the error, negative errors
				 * mean the helicopter should rotate clockwise to reduce the error.
				 */
				double calculateYawProportional(double currentYawDegrees, double referenceYawDegrees);
				
				/**
				 * Calculates the anti windup term using the old yaw control value 
				 * between the values of controlMaxValue and controlMinValue
				 * and using the anti windup gain value. This should be the control value
				 * before the servo limits were applied. This is so that this function
				 * knows if the previously calculated control value tried to control
				 * the control surfaces beyond their limits.
				 * If the previous yaw control value was greater than the servoMin/Max
				 * values, then an antiwindup term will be calculated which will be used
				 * to reset the integral term back to 0 since the servos are being 'saturated'.
				 * This is because the integral term is used to reduce steady state error 
				 * (bring the helicopter closer to the position it should be at) but if
				 * the steady state error is huge (the helicopter is really far away from the target)
				 * then the integral term basically shouldn't be used until the helicopter
				 * gets close to the target.
				 * @param oldYawControlPreServoAdj The control value calculated for the Yaw term
				 * in the previous iteration. Should be between controlMax and Min Values. This value should be
				 * the value before the servo boundaries were applied.
				 * @return returns the anti windup term with the gain applied which should be
				 * applied to this iterations integral term.
				 * 0 if oldYawControlPreServoAdj == oldYawControl
				 */
				double calculateYawIntegralAntiWindup(double oldYawControlPreServoAdj, double oldYawControl);
				
				/**
				 * Calculates the integral of the yaw without weighting.
				 * @param yawProportionalDegrees The proportional error component of the yaw (between -180, 180)
				 * @param oldYawIntegral The previously calculated integral term without weighting
				 * @param yawAntiWindup The anti-windup component which will be subtracted from
				 * the integral term to reset the integral term to 0 if the servos are saturated (at their max value)
				 * @return the calculated integral value (without weighting).
				 */
				double calculateYawIntegral(double yawProportionalDegrees, double oldYawIntegral, double yawAntiWindup);
				
				/**
				 * Subtracts the yawVelocityDegreesPerSecond and referenceYawVelocityDegreesPerSecond.
				 * @yawVelocityDegreesPerSecond the current velocity of rotation in the yaw direction in degrees per second
				 * @referenceYawVelocityDegreesPerSecond the desired velocity of rotation in the yaw direction in degrees per second
				 * @return the difference between the two parameters in degrees per second.
				 */
				double calculateYawVelocityError(double yawVelocityDegreesPerSecond, double referenceYawVelocityDegreesPerSecond);
				
				/**
				 * Calculates the control value for the yaw (pedal) control. The value
				 * will be between controlMaxValue, and controlMinValue. This method takes the
				 * arguments, applies the appropriate weights, and sums the variables to get the control
				 * value.
				 * @param yawProportionalDegrees The proportional error term between the desired yaw heading and the actual yaw heading in degrees.
				 * @param yawDerivativeError The error between the desired speed of rotation, and the reference speed of rotation for yaw rotation in degrees per second.
				 * @param yawIntegral The integral term.
				 * @return a control value between controlMaxValue and controlMinValue (generally between -1 and 1 where 0 is neutral - no blade angle).
				 */
				double calculateYawControlValue(double yawProportionalDegrees, double yawVelocityErrorDegreesPerSecond, double yawIntegral);
				
				/**
				 * Adjusts the given control value by the servo limits and trim values. If the value is above the limit it's adjusted
				 * to the max servo value. If the value is below the limit, it's set to the minimum servo limit.
				 * @param controlValueToAdjust value to adjust
				 * @return the adjusted value.
				 */
				double adjustControlForServoLimits( double controlValueToAdjust );
				
				
				/**
				 * Calculates the control value for the tail rotor given the various parameters defined in the model
				 */
				void tailRotorCollectiveOuterLoopUpdate();

		};
	}
}



#endif /* PIDCONTROLLER_H_ */
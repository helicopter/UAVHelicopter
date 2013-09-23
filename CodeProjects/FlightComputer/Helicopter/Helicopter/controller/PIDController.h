/*
 * PIDController.h
 *
 * Created: 9/22/2013 2:11:38 PM
 *  Author: HP User
 */ 


#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include "SystemModel.h"

using namespace helicopter::model;

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
				
				double yawIntegralGain;
				double yawDerivativeGain;
				double yawProportionalGain;
				double yawAntiWindupGain;
					
				double minYawServoControlValue;
				double maxYawServoControlValue;
					
				double intervalPeriodSecs;
					
				double controlMaxValue;
				double controlMinValue;
				
			public:
				PIDController (SystemModel *model);
					
				~PIDController();
				
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
				void setMinYawServoControlValue (double minYawServoControlValue)
				{
					this->minYawServoControlValue = minYawServoControlValue;
				}
				
				/**
				 * The maximum control value that the yaw actuators can be set too. This is used for 
				 * integral anti-windup to determine when the actuators would be saturated.
				 * The value should be no larger than the controlMaxValue (e.g. no greater than 1)
				 */
				void setMaxYawServoControlValue(double maxYawServoControlValue)
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
				 * @param currentYawValue The current heading from the sensors where 0 = Magnetic North. This value should be between
				 * 0 - 359.99 (it may get set to 360 due to rounding)
				 * @param referenceYawValue The desired heading between 0-359.99. Where 0 = Magnetic North.
				 * @return the error between the current yaw and the reference yaw (Gain is NOT applied).
				 * This value will be between -180 - 179.99. This is because the larger the possible jump in
				 * error, the more it disturbs the calculations. So an error going from 0 then suddenly jumping to
				 * -360 would cause a large disturbance in the PID calculations. 
				 * Positive errors mean the helicopter needs to rotate counterclockwise to reduce the error, negative errors
				 * mean the helicopter should rotate clockwise to reduce the error.
				 */
				double calculateYawProportional(double currentYaw, double referenceYaw);
				
				/**
				 * Calculates the anti windup term using the old yaw control value 
				 * between the values of controlMaxValue and controlMinValue
				 * and using the anti windup gain value.
				 * If the previous yaw control value was greater than the servoMin/Max
				 * values, then an antiwindup term will be calculated which will be used
				 * to reset the integral term back to 0 since the servos are being 'saturated'.
				 * This is because the integral term is used to reduce steady state error 
				 * (bring the helicopter closer to the position it should be at) but if
				 * the steady state error is huge (the helicopter is really far away from the target)
				 * then the integral term basically shouldn't be used until the helicopter
				 * gets close to the target.
				 * @param oldYawControlValue The control value calculated for the Yaw term
				 * in the previous iteration. Should be between controlMax and Min Values.
				 * @return returns the anti windup term with the gain applied which should be
				 * applied to this iterations integral term.
				 */
				double calculateYawIntegralAntiWindup(double oldYawControlValue);
				
				/**
				 * Calculates the integral of the yaw without weighting.
				 * @param yawProportional The proportional error component of the yaw (between -180, 180)
				 * @param oldYawIntegral The previously calculated integral term without weighting
				 * @param yawAntiWindup The anti-windup component which will be subtracted from
				 * the integral term to reset the integral term to 0 if the servos are saturated (at their max value)
				 * @return the calculated integral value (without weighting).
				 */
				double calculateYawIntegral(double yawProportional, double oldYawIntegral, double yawAntiWindup);
		};
	}
}



#endif /* PIDCONTROLLER_H_ */
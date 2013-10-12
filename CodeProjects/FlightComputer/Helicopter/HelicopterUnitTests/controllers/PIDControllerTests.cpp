/*
 * ControlLawOuterLoopTask.cpp
 *
 * Created: 9/21/2013 4:03:19 PM
 *  Author: HP User
 */ 
#include "PIDControllerTests.h"
#include "PIDController.h"
#include "UnitTestUtils.h"
#include "ServoDriver.h"
#include "MockServoDriver.h"

using namespace helicopter::controller;
using namespace helicopter::drivers;

int calculateYaw_test(TestCase *test)
{
	SystemModel *model = new SystemModel();
	
	ServoDriver *servoDriver = new MockServoDriver();
	
	//Semi constants that will be tuned during development
	double yawIntegralGain = .13;
	double yawDerivativeGain = .11;
	double yawProportionalGain = .12;
	double yawAntiWindupGain = .1;
	
	//These will be controlled by the 'servo controller' though.
	double minYawServoControlValue = -.8;
	double maxYawServoControlValue = .8;
	
	double intervalPeriodSecs = .15;
	
	double controlMaxValue = 1;
	double controlMinValue = -1;
	
	servoDriver->MaxControlValue(controlMaxValue);
	servoDriver->MinControlValue(controlMinValue);
	
	//Later I'll have to pass in the servo controller, etc. 
	PIDController *pidController = new PIDController(model, servoDriver);
	
	
	pidController->setYawProportionalGain(yawProportionalGain);
	pidController->setYawIntegralGain(yawIntegralGain);
	pidController->setYawDerivativeGain(yawDerivativeGain);

	pidController->setIntervalPeriodSecs(intervalPeriodSecs);
	pidController->setYawAntiWindupGain(yawAntiWindupGain);
	
	
	pidController->setMinTailRotorCollectiveControlValue (minYawServoControlValue);
	pidController->setMaxTailRotorCollectiveValue(maxYawServoControlValue);
	pidController->setControlMaxValue(controlMaxValue);
	pidController->setControlMinValue(controlMinValue);
	
	double referenceYaw = 0;
	
	double currentYaw = 0;
	
	/**
	 * Test the proportional function.
	 */
	double yawProportional = pidController->calculateYawProportional(currentYaw, referenceYaw);
	
	AssertTrue2(yawProportional == 0,0);
	
	
	AssertTrue2(pidController->calculateYawProportional(0, 1) == -1.0,0);
	AssertTrue2(pidController->calculateYawProportional(0, 179.99) == -179.99,0);
	AssertTrue2(pidController->calculateYawProportional(0, 359) == 1.0,0);
	AssertTrue2(pidController->calculateYawProportional(0, 180) == -180.0,0);
	AssertTrue2(pidController->calculateYawProportional(180, 0) == -180.0,0);
	AssertTrue2(pidController->calculateYawProportional(0, 180.01) == 179.99,0);
	AssertTrue2(pidController->calculateYawProportional(50, 229) == -179,0);
	AssertTrue2(pidController->calculateYawProportional(50, 230) == -180,0);
	AssertTrue2(pidController->calculateYawProportional(0, 360) == 0,0);
	AssertTrue2(pidController->calculateYawProportional(350, 351) == -1,0);
	AssertTrue2(pidController->calculateYawProportional(350, 349) == 1.0,0);
	AssertTrue2(pidController->calculateYawProportional(350, 360) == -10,0);
	AssertTrue2(pidController->calculateYawProportional(360, 350) == 10,0);
	
	AssertTrue2(yawProportional >= -180 && yawProportional <= 180, 0);
	
	double oldYawControlValue = 0;
	
	
	
	/**
	 * Test the yaw anti windup function
	 */
	double yawAntiWindup = pidController->calculateYawIntegralAntiWindup(oldYawControlValue);
	
	AssertTrue2(yawAntiWindup == 0, 0);
	
	//test at .79, .8, .81, -.79, -.8, -.81, 1, -1, .99, -.99
	AssertTrue2(pidController->calculateYawIntegralAntiWindup(.79) == 0, 0);
	AssertTrue2(pidController->calculateYawIntegralAntiWindup(.80) == 0, 0);
	
	//.01 * .1 = .001
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(.81);
	AssertTrue2(yawAntiWindup > .0009, 0);
	AssertTrue2(yawAntiWindup < .0011, 0);
	
	AssertTrue2(pidController->calculateYawIntegralAntiWindup(-.79) == 0, 0);
	AssertTrue2(pidController->calculateYawIntegralAntiWindup(-.8) == 0, 0);
	
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(-.81);
	AssertTrue2(yawAntiWindup < -.0009, 0);
	AssertTrue2(yawAntiWindup > -.0011, 0);
	
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(1.0);
	AssertTrue2(yawAntiWindup > .019, 0);
	AssertTrue2(yawAntiWindup < .021, 0);
	
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(-1.0);
	AssertTrue2(yawAntiWindup < -.019, 0);
	AssertTrue2(yawAntiWindup > -.021, 0);
	
	
	
	/**
	 * Test calculating the integral of the yaw.
	 */
	
	double oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = .2;
	
	//.8 +.4 * .15 - .2 = .66
	double yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
	
	AssertTrue2(yawIntegral > .65, 0);
	AssertTrue2(yawIntegral < .67, 0);
	
	oldYawIntegral = -.8;
	yawProportional = -.4;
	yawAntiWindup = -.2;
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
	
	AssertTrue2(yawIntegral < -.65, 0);
	AssertTrue2(yawIntegral > -.67, 0);
	
	/**
	 * Test that if the anti-windup would cause the integral term to pass 0, that it just gets set to 0 instead.
	 */
	oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = .9;
		
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue2(yawIntegral == 0, 0);
	
	oldYawIntegral = -.8;
	yawProportional = -.4;
	yawAntiWindup = -.9;
		
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue2(yawIntegral == 0, 0);
	
	
	//test with no anti windup
	oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = 0;
		
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue2(yawIntegral == .86, 0);
	
	
	/**
	 * Test the derivative calculations
	 */
	double yawVelocity = 0;
	
	double referenceYawVelocity = 0;
	
	double yawDerivativeError = pidController->calculateYawVelocityError(yawVelocity, referenceYawVelocity);
	
	AssertTrue2(yawDerivativeError == 0, 0);
	
	AssertTrue2(pidController->calculateYawVelocityError(.5, .25) == .25, 0);
	
	
	/**
	 * Test calculating the yaw control value.
	 */
	yawProportional = .25;
	yawDerivativeError = 1.50;
	yawIntegral = .15;
	
	//(-1 * .13*.15 - .12*.25 - .11*1.50)*-1
	double yawControl = pidController->calculateYawControlValue(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue2(yawControl == .2145, 0);
	
	yawProportional = -.25;
	yawDerivativeError = -1.50;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControlValue(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue2(yawControl == -.2145, 0);
	
	yawProportional = -10.25;
	yawDerivativeError = -1.50;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControlValue(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue2(yawControl == controlMinValue, 0);
	
	yawProportional = 10.25;
	yawDerivativeError = -1.50;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControlValue(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue2(yawControl == controlMaxValue, 0);
	
	yawProportional = .25;
	yawDerivativeError = .015;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControlValue(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue2(yawControl < .01215 && yawControl > .01214, 0);
	
	
	double adjustedValue = pidController->adjustControlForServoLimits(.9);
	AssertTrue(adjustedValue == .8);
		
	adjustedValue = pidController->adjustControlForServoLimits(-.9);
	AssertTrue(adjustedValue == -.8);

	adjustedValue = pidController->adjustControlForServoLimits(.3);
	AssertTrue(adjustedValue == .3);
	
	/**
	 * Test that the outer loop properly updates the model.
	 */
	
	model->MagYawDegrees(320.0);
	model->ReferenceMagYawDegrees(323.0);
	model->YawControlBeforeServoLimitsAdjustment(.3);
	model->YawVelocityDegreesPerSecond(1.2);
	model->ReferenceYawVelocityDegreesPerSecond(0.0);
	model->YawIntegral(.12);
	
	pidController->tailRotorCollectiveOuterLoopUpdate();
	
	AssertTrue(model->YawIntegral() == -.33);
	
	AssertTrue(model->YawControlBeforeServoLimitsAdjustment() > -.2709 && model->YawControlBeforeServoLimitsAdjustment() < -.2708);
	AssertTrue(model->YawControl() > -.2709 && model->YawControl() < -.2708);
	
	AssertTrue(((MockServoDriver*)servoDriver)->TailRotorCollectiveControl() == model->YawControl());
	
	
	/**
	 * Test setting the trim value.
	 */
	
	model->MagYawDegrees(320.0);
	model->ReferenceMagYawDegrees(323.0);
	model->YawControlBeforeServoLimitsAdjustment(.3);
	model->YawVelocityDegreesPerSecond(1.2);
	model->ReferenceYawVelocityDegreesPerSecond(0.0);
	model->YawIntegral(.12);
	
	pidController->setYawServoTrim(.1);
		
	pidController->tailRotorCollectiveOuterLoopUpdate();
		
	AssertTrue(model->YawIntegral() == -.33);
		
	AssertTrue(model->YawControlBeforeServoLimitsAdjustment() > -.2709 && model->YawControlBeforeServoLimitsAdjustment() < -.2708);
	AssertTrue(model->YawControl() > -.1709 && model->YawControl() < -.1708);
	
	
	
	
	model->MagYawDegrees(320.0);
	model->ReferenceMagYawDegrees(329.0);
	model->YawControlBeforeServoLimitsAdjustment(.3);
	model->YawVelocityDegreesPerSecond(1.2);
	model->ReferenceYawVelocityDegreesPerSecond(0.0);
	model->YawIntegral(.12);
	
	pidController->tailRotorCollectiveOuterLoopUpdate();
	
	AssertTrue(model->YawControlBeforeServoLimitsAdjustment() == -1.0);
	AssertTrue(model->YawControl() == -.8);
	AssertTrue(((MockServoDriver*)servoDriver)->TailRotorCollectiveControl() == -.8);
	
	/*
	//Have a test which is impacted by servo control boundaries so that the before servo boundaries and yaw control values are the same.
	
	
	
	model->YawIntegral(.12);
	model->CurrentYawDegrees(220.211);
	model->ReferenceYawDegrees(244.310);
	model->TailRotorControlBeforeAdjustment(.6); //adjustment for what? servoMinMax?
	model->YawVelocityDegreesPerSecond(.80);
	model->ReferenceYawVelocityDegreesPerSecond(0);
	
	//set yaw antiwindup to 0 but have other tests that tests it
	
	
	//interval period
	
	
	pidController->tailRotorCollectiveOuterLoopUpdate();
	
	//Run through the first time and ensure the PID variables are correct.

	*/
	
	
	
	
	//run through a second time.
	
	
	//run through testing the servo trim, min, and max states.
	
	
	//pidController->tailRotorCollectiveOuterLoopUpdate()
	//pidController->mainRotorCollectiveOuterLoopUpdate()
	//pidController->mainRotorLateralCyclicOuterLoopUpdate()
	//pidController->mainRotorLongitudinalCyclicOuterLoopUpdate()
	//pidController->mainRotorLateralCyclicInnerLoopUpdate()
	//pidController->mainRotorLongitudinalCyclicInnerLoopUpdate()
	
	
	//what calls the servo controller / controlsurface controller? 
	//(control surface controller -> servo driver) control surface needs to be configured for trim settings, and surface min/max values. then pass to servo driver which doesn't care.
	
	//TODO Problem: I can't describe why we multiply by so many negative 1 terms.
	//TODO: Since it seems so odd that the yaw error is - for counter clockwise, which means that
	//when we calculate yawDerivativeError it's off, which also affects the final summ/addition for
	//yawControl, should we look at reversing that?
	//I think the way i calculate error is also all messed up. It's causing the yaw control to
	//have a bunch of -1 terms multiplied in order to not lose control. generally it
	//seems to be Reference - Actual (AC2-09-AntiWIndup.pdf #2). But I'm doing the opposite. 
	//I think if I reverse the terms I can clean up the negatives. I would need to change
	//my 180 - 180 conversion direction.
	//But i think this also works because you think of everything from the perspective of the 'reference'
	//so from the reference point, if its at 20, and the actual is at 10, then your -10 counter clockwise
	//away from the reference point's perspective.
	
	//in terms of pitch angle, what does -1, and 1 mean?
	//In terms of rotation, what does -1 and 1 mean? -1 means clockwise rotation? 1 means counter?
	//I'll want to align the rotations with the errors in my mind.
	//And I'll want to document how 1 means positive pitch, and -1 means negative pitch (helicopter goes down).
	
	
	//TODO up next I'll want to create and test the method that calls all of the above methods for realz.
	
	
	
	
	
	
	
	
	
	
	//determine what values should truly be floats and what should be doubles because of the inifficiencies of using
	//doubles. - no, lets use all doubles to keep it simple for now. Worry about performance later once you know there is an issue.
	
	//Don't forget to put in reference to PIDwindup document, and explain how the anti-windup works.
	
	//First we test the function which generates the pedal command
	
	//then we test the function which acts as the 'outer loop' (service) which
	//calls the pedal command generation, then calls the the servo controller
	
	
	//Limits.
	//time between outer loop processing
	//time between sensor readings.
	
	return 0;
}


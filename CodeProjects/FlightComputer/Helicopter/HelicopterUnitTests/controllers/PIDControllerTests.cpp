/*
 * ControlLawOuterLoopTask.cpp
 *
 * Created: 9/21/2013 4:03:19 PM
 *  Author: HP User
 */ 
#include "PIDControllerTests.h"
#include "PIDController.h"
#include "UnitTestUtils.h"

using namespace helicopter::controller;

int calculateYaw_test(TestCase *test)
{
	SystemModel *model = new SystemModel();
	
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
	
	
	//Later I'll have to pass in the servo controller, etc. 
	PIDController *pidController = new PIDController(model);
	
	
	pidController->setYawProportionalGain(yawProportionalGain);
	pidController->setYawIntegralGain(yawIntegralGain);
	pidController->setYawDerivativeGain(yawDerivativeGain);

	pidController->setIntervalPeriodSecs(intervalPeriodSecs);
	pidController->setYawAntiWindupGain(yawAntiWindupGain);
	
	
	pidController->setMinYawServoControlValue (minYawServoControlValue);
	pidController->setMaxYawServoControlValue(maxYawServoControlValue);
	pidController->setControlMaxValue(controlMaxValue);
	pidController->setControlMinValue(controlMinValue);
	
	double referenceYaw = 0;
	
	double currentYaw = 0;
	
	/**
	 * Test the proportional function.
	 */
	double yawProportional = pidController->calculateYawProportional(currentYaw, referenceYaw);
	
	AssertTrue(yawProportional == 0,0);
	
	
	AssertTrue(pidController->calculateYawProportional(0, 1) == -1.0,0);
	AssertTrue(pidController->calculateYawProportional(0, 179.99) == -179.99,0);
	AssertTrue(pidController->calculateYawProportional(0, 359) == 1.0,0);
	AssertTrue(pidController->calculateYawProportional(0, 180) == -180.0,0);
	AssertTrue(pidController->calculateYawProportional(180, 0) == -180.0,0);
	AssertTrue(pidController->calculateYawProportional(0, 180.01) == 179.99,0);
	AssertTrue(pidController->calculateYawProportional(50, 229) == -179,0);
	AssertTrue(pidController->calculateYawProportional(50, 230) == -180,0);
	AssertTrue(pidController->calculateYawProportional(0, 360) == 0,0);
	AssertTrue(pidController->calculateYawProportional(350, 351) == -1,0);
	AssertTrue(pidController->calculateYawProportional(350, 349) == 1.0,0);
	AssertTrue(pidController->calculateYawProportional(350, 360) == -10,0);
	AssertTrue(pidController->calculateYawProportional(360, 350) == 10,0);
	
	AssertTrue(yawProportional >= -180 && yawProportional <= 180, 0);
	
	double oldYawControlValue = 0;
	
	
	
	/**
	 * Test the yaw anti windup function
	 */
	double yawAntiWindup = pidController->calculateYawIntegralAntiWindup(oldYawControlValue);
	
	AssertTrue(yawAntiWindup == 0, 0);
	
	//test at .79, .8, .81, -.79, -.8, -.81, 1, -1, .99, -.99
	AssertTrue(pidController->calculateYawIntegralAntiWindup(.79) == 0, 0);
	AssertTrue(pidController->calculateYawIntegralAntiWindup(.80) == 0, 0);
	
	//.01 * .1 = .001
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(.81);
	AssertTrue(yawAntiWindup > .0009, 0);
	AssertTrue(yawAntiWindup < .0011, 0);
	
	AssertTrue(pidController->calculateYawIntegralAntiWindup(-.79) == 0, 0);
	AssertTrue(pidController->calculateYawIntegralAntiWindup(-.8) == 0, 0);
	
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(-.81);
	AssertTrue(yawAntiWindup < -.0009, 0);
	AssertTrue(yawAntiWindup > -.0011, 0);
	
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(1.0);
	AssertTrue(yawAntiWindup > .019, 0);
	AssertTrue(yawAntiWindup < .021, 0);
	
	yawAntiWindup = pidController->calculateYawIntegralAntiWindup(-1.0);
	AssertTrue(yawAntiWindup < -.019, 0);
	AssertTrue(yawAntiWindup > -.021, 0);
	
	
	
	/**
	 * Test calculating the integral of the yaw.
	 */
	
	double oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = .2;
	
	//.8 +.4 * .15 - .2 = .66
	double yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
	
	AssertTrue(yawIntegral > .65, 0);
	AssertTrue(yawIntegral < .67, 0);
	
	oldYawIntegral = -.8;
	yawProportional = -.4;
	yawAntiWindup = -.2;
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
	
	AssertTrue(yawIntegral < -.65, 0);
	AssertTrue(yawIntegral > -.67, 0);
	
	/**
	 * Test that if the anti-windup would cause the integral term to pass 0, that it just gets set to 0 instead.
	 */
	oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = .9;
		
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue(yawIntegral == 0, 0);
	
	oldYawIntegral = -.8;
	yawProportional = -.4;
	yawAntiWindup = -.9;
		
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue(yawIntegral == 0, 0);
	
	
	//test with no anti windup
	oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = 0;
		
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue(yawIntegral == .86, 0);
	
	
	/**
	 * Test the derivative calculations
	 */
	double yawVelocity = 0;
	
	double referenceYawVelocity = 0;
	
	double yawDerivativeError = pidController->calculateYawDerivativeError(yawVelocity, referenceYawVelocity);
	
	AssertTrue(yawDerivativeError == 0, 0);
	
	AssertTrue(pidController->calculateYawDerivativeError(.5, .25) == .25, 0);
	
	
	/**
	 * Test calculating the yaw control value.
	 */
	yawProportional = .25;
	yawDerivativeError = 1.50;
	yawIntegral = .15;
	
	//(-1 * .13*.15 - .12*.25 - .11*1.50)*-1
	double yawControl = pidController->calculateYawControl(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue(yawControl == .2145, 0);
	
	yawProportional = -.25;
	yawDerivativeError = -1.50;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControl(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue(yawControl == -.2145, 0);
	
	yawProportional = -10.25;
	yawDerivativeError = -1.50;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControl(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue(yawControl == controlMinValue, 0);
	
	yawProportional = 10.25;
	yawDerivativeError = -1.50;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControl(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue(yawControl == controlMaxValue, 0);
	
	yawProportional = .25;
	yawDerivativeError = .015;
	yawIntegral = -.15;
	
	yawControl = pidController->calculateYawControl(yawProportional, yawDerivativeError, yawIntegral);
	AssertTrue(yawControl == .05115, 0);
	//.01215
	//Run a test to ensure that the control values are restricted within the bounds of control max and min
	//test max
	//test min
	//Ensure that the control value is within -1, and 1.
	AssertTrue(yawControl >= controlMinValue && yawControl <= controlMaxValue, 0);
	
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


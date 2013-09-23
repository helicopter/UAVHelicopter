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
	double yawIntegralGain = 0;
	double yawDerivativeGain = 0;
	double yawProportionalGain = 0;
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
	
	oldYawIntegral = .8;
	yawProportional = .4;
	yawAntiWindup = .9;
		
	//.8 +.4 * .15 - .2 = .66
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue(yawIntegral == 0, 0);
	
	oldYawIntegral = -.8;
	yawProportional = -.4;
	yawAntiWindup = -.9;
		
	//.8 +.4 * .15 - .2 = .66
	yawIntegral = pidController->calculateYawIntegral(yawProportional, oldYawIntegral, yawAntiWindup);
		
	AssertTrue(yawIntegral == 0, 0);
	
	//Test the yaw antiwindup causing integral to go negative (verify it just gets set to 0.
	//verify 0 in the opposite direction. negative going positive.
	
	
	/*
	double yawVelocity = 0;
	
	double referenceYawVelocity = 0;
	
	double yawDerivativeError = pidController->calculateYawDerivativeError(yawVelocity, referenceYawVelocity);
	
	
	
	//???not done - still need to define.
	double yawControl = pidController->calculateYawControl(yawProportional, yawDerivativeError, yawIntegral);
	
	//Ensure that the control value is within -1, and 1.
	AssertTrue(yawControl >= controlMinValue && yawControl <= controlMaxValue);
	
	
	
	//determine what values should truly be floats and what should be doubles because of the inifficiencies of using
	//doubles. - no, lets use all doubles to keep it simple for now. Worry about performance later once you know there is an issue.
	
	//Don't forget to put in reference to PIDwindup document, and explain how the anti-windup works.
	
	//First we test the function which generates the pedal command
	
	//then we test the function which acts as the 'outer loop' (service) which
	//calls the pedal command generation, then calls the the servo controller
	*/
	
	//Limits.
	//time between outer loop processing
	//time between sensor readings.
	
	return 0;
}
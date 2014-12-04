/*
 * CommonHeader.h
 *
 * Created: 2/24/2013 1:59:27 PM
 *  Author: HP User
 */ 

/**
 * This file represents constants and common opperators used throughout the system.
 */

#ifndef COMMONHEADER_H_
#define COMMONHEADER_H_

//Note: I defined F_CPU in the visual studio project tool chain because I was getting
//redefinition warnings or not defined warnings if I didn't.
//#define F_CPU 16000000UL


#include <stdlib.h>
#include <stdint.h>


/**
 * define new operator
 */
void * operator new(size_t size);

/**
* define delete operator
*/
void operator delete(void * ptr);

/**
 * define new operator for arrays
 */
void * operator new[](size_t size);

/**
 * define delete operator for arrays
 */
void operator delete[](void * ptr);

typedef unsigned char byte;

enum PRESCALER {NO_PRESCALER=1, PRESCALE_BY_EIGHT=8,
	PRESCALE_BY_SIXTYFOUR=64,
	PRESCALE_BY_TWOFIFTYSIX=256,
	PRESCALE_BY_TENTWENTYFOUR=1024};
	
	
/**
 * Common Constants
 */


/**
 * This is the earths average gravity (as opposed to equatorial average)
 * From: http://en.wikipedia.org/wiki/Gravity_of_Earth
 */
static const float GRAVITY_MSS = 9.80665;

/**
 * This is the time interval between barometer readings.
 * This will need to change if the barometer is read faster/slower.
 */
static const float BAROMETER_SENSOR_READ_PERIOD = 1/50.0f;

/**
 * This represents how long a gyro readings represents.
 * This is the interval period on the sensor itself, NOT how long
 * in between the system reading the sensor. 
 */
static const float GYRO_SENSOR_READ_PERIOD = 1/98.0f;

	
	
/**
 * The frequency that the scheduler should run at in terms of ticks per second.
 */
//static const int SCHEDULER_TICK_FREQUENCY_HZ = 200;
//static const int SCHEDULER_TICK_FREQUENCY_HZ = 120; 
//static const int SCHEDULER_TICK_FREQUENCY_HZ = 70; //original - was this 3/29/2014
static const int SCHEDULER_TICK_FREQUENCY_HZ = 200;

//static const int SCHEDULER_TICK_FREQUENCY_HZ = 100; //good when not transmitting to GCS

static const double PID_OUTER_LOOP_PERIOD = .05; //most recent 4/9/2014 // 20 hz
static const double PID_INNER_LOOP_PERIOD = .02;  // 50 hz

//static const double PID_OUTER_LOOP_PERIOD = .02;

#endif /* COMMONHEADER_H_ */
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

#define F_CPU 16000000UL


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
 * The frequency that the scheduler should run at in terms of ticks per second.
 */
static const int SCHEDULER_TICK_FREQUENCY_HZ = 200;

static const double PID_OUTER_LOOP_PERIOD = .05;

#endif /* COMMONHEADER_H_ */
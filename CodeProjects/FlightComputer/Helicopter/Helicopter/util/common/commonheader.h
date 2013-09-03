/*
 * commonheader.h
 *
 * Created: 2/24/2013 1:59:27 PM
 *  Author: HP User
 */ 


#ifndef COMMONHEADER_H_
#define COMMONHEADER_H_

#include <stdlib.h>
#include <stdint.h>

/*
void * operator new(size_t size)
{
	return malloc(size);
}
void operator delete(void * ptr)
{
	free(ptr);
}
*/

#define COMPILE_FOR_MISSIONCOMPUTER
//#define COMPILE_FOR_SIMULATOR

static const int TIMER_FREQUENCY_HZ = 200;

enum PRESCALER {NO_PRESCALER=1, PRESCALE_BY_EIGHT=8,
				PRESCALE_BY_SIXTYFOUR=64,
				PRESCALE_BY_TWOFIFTYSIX=256,
				PRESCALE_BY_TENTWENTYFOUR=1024};
				
		

#ifdef COMPILE_FOR_MISSIONCOMPUTER
	//set your clock speed
	#define F_CPU 16000000UL
	static const PRESCALER TIMER_PRESCALER = PRESCALE_BY_SIXTYFOUR;
#else
	#define F_CPU 1000000UL
	static const PRESCALER TIMER_PRESCALER = PRESCALE_BY_EIGHT;
#endif
	
/*
Target Timer Count = (((Input Frequency / Prescaler) / Target Frequency) 
(((16000000 / 64) / 200)) = 1250
(((1000000 / 8) / 200)) = 625
*/
static const int TARGET_TIMER_COUNT = ((F_CPU / TIMER_PRESCALER) / TIMER_FREQUENCY_HZ);
	
	

void * operator new(size_t size);
void operator delete(void * ptr);

void * operator new[](size_t size);
void operator delete[](void * ptr);

namespace helicopter
{
	namespace util
	{
		namespace common
		{
			typedef char byte;

		}
	}
}



#endif /* COMMONHEADER_H_ */
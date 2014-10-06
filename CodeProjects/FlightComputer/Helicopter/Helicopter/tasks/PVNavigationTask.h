/*
 * NavigationTask.h
 *
 * Created: 3/26/2014 7:31:14 AM
 *  Author: HP User
 */ 


#ifndef PVNAVIGATIONTASK_H_
#define PVNAVIGATIONTASK_H_



#include "Task.h"
#include "AHRS.h"
#include "SystemModel.h"

using namespace helicopter::navigation;
using namespace helicopter::model;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * This class executes the outer loop of the PID controller
		 */
		class PVNavigationTask : public Task
		{
			private:
				static const float WEIGHT;
				
				//The time in seconds (fractions of seconds) between the barometer reads.
				float barometerSensorReadPeriod;
				
				AHRS *ahrs;
				
				SystemModel *model;
				
			public:

				PVNavigationTask (float barometerSensorReadPeriod, AHRS *ahrs, SystemModel *model, int delay, int period);

				void runTaskImpl();
		};
	}
}






#endif /* PVNAVIGATIONTASK_H_ */
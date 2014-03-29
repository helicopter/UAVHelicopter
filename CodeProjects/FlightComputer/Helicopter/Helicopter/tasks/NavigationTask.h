/*
 * NavigationTask.h
 *
 * Created: 3/26/2014 7:31:14 AM
 *  Author: HP User
 */ 


#ifndef NAVIGATIONTASK_H_
#define NAVIGATIONTASK_H_



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
		class NavigationTask : public Task
		{
			private:
				static const float WEIGHT;
				AHRS *ahrs;
				SystemModel *model;
				
			public:

				NavigationTask (AHRS *ahrs, SystemModel *model, int delay, int period);

				void runTaskImpl();
		};
	}
}






#endif /* NAVIGATIONTASK_H_ */
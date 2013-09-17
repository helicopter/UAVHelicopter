/*
 * SimTelemetryTask.h
 *
 * Created: 9/8/2013 3:38:33 PM
 *  Author: HP User
 */ 


#ifndef SIMTELEMETRYTASK_H_
#define SIMTELEMETRYTASK_H_

#include "Task.h"
#include "SystemModel.h"
#include "RadioInterface.h"

using namespace helicopter::model;
using namespace helicopter::tasks;
using namespace helicopter::interfaces;

namespace helicopter
{
	
	namespace tasks
	{
		namespace misctasks
		{
			

			class SimTelemetryTask : public Task
			{
				private:
					RadioInterface *radioInterface;
					SystemModel *model;
				public:

				SimTelemetryTask (RadioInterface *radioInterface, SystemModel *model, int delay, int period);
				
				void runTaskImpl();
			};
		}
	}
}



#endif /* SIMTELEMETRYTASK_H_ */
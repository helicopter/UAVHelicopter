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
#include "GroundControlStationInterface.h"

using namespace helicopter::model;
using namespace helicopter::tasks;
using namespace helicopter::interfaces;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * Task to receive simulator telemetry data from the ground control station.
		 */
		class SimTelemetryTask : public Task
		{
			private:
				GroundControlStationInterface *radioInterface;
				SystemModel *model;
			public:

				SimTelemetryTask (GroundControlStationInterface *radioInterface, SystemModel *model, int delay, int period);
			
				void runTaskImpl();
		};
	}
}



#endif /* SIMTELEMETRYTASK_H_ */
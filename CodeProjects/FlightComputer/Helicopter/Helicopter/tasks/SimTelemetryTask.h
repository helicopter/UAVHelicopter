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
#include "PIDController.h"

using namespace helicopter::model;
using namespace helicopter::tasks;
using namespace helicopter::interfaces;
using namespace helicopter::controller;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * Task to receive simulator telemetry data from the ground control station.
		 */
		class SimTelemetryTask : public Task
		{
			public:
				enum DATATORECEIVE {ALLDATA, SENSORDATA, GAINSDATA};
				
			private:
				GroundControlStationInterface *radioInterface;
				SystemModel *model;
				PIDController *pidController;
				DATATORECEIVE dataToReceive;
				
			public:

				SimTelemetryTask (GroundControlStationInterface *radioInterface, SystemModel *model, PIDController *pidController, DATATORECEIVE dataToReceive, int delay, int period);
			
				void runTaskImpl();
		};
	}
}



#endif /* SIMTELEMETRYTASK_H_ */
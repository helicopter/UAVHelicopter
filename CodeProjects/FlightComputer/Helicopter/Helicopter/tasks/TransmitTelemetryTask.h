/*
 * TransmitTelemetryTask.h
 *
 * Created: 9/8/2013 7:04:10 PM
 *  Author: HP User
 */ 


#ifndef TRANSMITTELEMETRYTASK_H_
#define TRANSMITTELEMETRYTASK_H_

#include "Task.h"
#include "GroundControlStationInterface.h"
#include "SystemModel.h"

using namespace helicopter::interfaces;
using namespace helicopter::model;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * This class transmits flight computer telemetry information to the ground control station.
		 */
		class TransmitTelemetryTask : public Task
		{
			private:
				GroundControlStationInterface *radioInterface;
				SystemModel *model;
				
			public:

				TransmitTelemetryTask (GroundControlStationInterface *radioInterface, SystemModel *model, int delay, int period);

				void runTaskImpl();
		};
	}
}


#endif /* TRANSMITTELEMETRYTASK_H_ */
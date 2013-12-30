/*
 * SensorProcessingTask.h
 *
 * Created: 12/20/2013 10:06:45 PM
 *  Author: HP User
 */ 


#ifndef SENSORPROCESSINGTASK_H_
#define SENSORPROCESSINGTASK_H_

#include "Task.h"
#include "SystemModel.h"

using namespace helicopter::model;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * This class processes and transforms sensor data such as converting latitude/longitude into
		 * x,y,z coordinates
		 */
		class SensorProcessingTask : public Task
		{
			private:
				SystemModel *model;
				
			public:

				SensorProcessingTask (SystemModel *model, int delay, int period);

				void runTaskImpl();
		};		
	}
}




#endif /* SENSORPROCESSINGTASK_H_ */
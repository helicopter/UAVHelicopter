/*
 * ReadBarometerSensorTask.h
 *
 * Created: 4/8/2014 8:32:37 PM
 *  Author: HP User
 */ 


#ifndef READBAROMETERSENSORTASK_H_
#define READBAROMETERSENSORTASK_H_




#include "Task.h"
#include "SystemModel.h"
#include "BarometerSensor.h"

using namespace helicopter::model;
using namespace helicopter::sensors;
using namespace helicopter::tasks;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * This class executes the outer loop of the PID controller
		 */
		class ReadBarometerSensorTask : public Task
		{
			private:
				SystemModel *model;
				BarometerSensor *barometerSensor;
				
				int step;
				
			public:

				ReadBarometerSensorTask (SystemModel *model, BarometerSensor *barometerSensor, int delay, int period);

				void runTaskImpl();
		};
	}
}





#endif /* READBAROMETERSENSORTASK_H_ */
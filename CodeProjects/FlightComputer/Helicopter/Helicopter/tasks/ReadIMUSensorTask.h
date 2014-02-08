/*
 * ReadIMUSensorTask.h
 *
 * Created: 2/6/2014 8:49:37 PM
 *  Author: HP User
 */ 


#ifndef READIMUSENSORTASK_H_
#define READIMUSENSORTASK_H_




#include "Task.h"
#include "SystemModel.h"
#include "IMUSensor.h"

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
		class ReadIMUSensorTask : public Task
		{
			private:
				SystemModel *model;
				IMUSensor *imuSensor;
				
			public:

				ReadIMUSensorTask (SystemModel *model, IMUSensor *imuSensor, int delay, int period);

				void runTaskImpl();
		};
	}
}



#endif /* READIMUSENSORTASK_H_ */
/*
 * ReadGPSSensorTask.h
 *
 * Created: 4/8/2014 8:00:07 PM
 *  Author: HP User
 */ 


#ifndef READGPSSENSORTASK_H_
#define READGPSSENSORTASK_H_


#include "Task.h"
#include "SystemModel.h"
#include "GPSSensor.h"

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
		class ReadGPSSensorTask : public Task
		{
			private:
				SystemModel *model;
				GPSSensor *gpsSensor;
				
			public:

				ReadGPSSensorTask (SystemModel *model, GPSSensor *gpsSensor, int delay, int period);

				void runTaskImpl();
		};
	}
}





#endif /* READGPSSENSORTASK_H_ */
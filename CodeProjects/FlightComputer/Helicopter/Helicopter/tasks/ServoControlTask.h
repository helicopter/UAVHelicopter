/*
 * ServoControlTask.h
 *
 * Created: 4/16/2014 4:43:21 PM
 *  Author: HP User
 */ 


#ifndef SERVOCONTROLTASK_H_
#define SERVOCONTROLTASK_H_


#include "Task.h"
#include "SystemModel.h"
#include "RadioControllerInterface.h"

using namespace helicopter::model;
using namespace helicopter::interfaces;
using namespace helicopter::tasks;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * This class executes the outer loop of the PID controller
		 */
		class ServoControlTask : public Task
		{
			private:
				SystemModel *model;
				RadioControllerInterface *radioControllerInterface;
				
			public:

				ServoControlTask (SystemModel *model, RadioControllerInterface *radioControllerInterface, int delay, int period);

				void runTaskImpl();
		};
	}
}





#endif /* SERVOCONTROLTASK_H_ */
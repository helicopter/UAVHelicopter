/*
 * PIDOuterLoopTask.h
 *
 * Created: 10/13/2013 12:06:31 PM
 *  Author: HP User
 */ 


#ifndef PIDOUTERLOOPTASK_H_
#define PIDOUTERLOOPTASK_H_


#include "Task.h"
#include "PIDController.h"

using namespace helicopter::controller;

namespace helicopter
{
	namespace tasks
	{
		/**
		 * This class executes the outer loop of the PID controller
		 */
		class PIDOuterLoopTask : public Task
		{
			private:
				PIDController *pidController;
				
			public:

				PIDOuterLoopTask (PIDController *pidController, int delay, int period);

				void runTaskImpl();
		};
	}
}




#endif /* PIDOUTERLOOPTASK_H_ */
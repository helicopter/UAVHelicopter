/*
 * PIDInnerLoopTask.h
 *
 * Created: 12/12/2013 7:43:49 PM
 *  Author: HP User
 */ 


#ifndef PIDINNERLOOPTASK_H_
#define PIDINNERLOOPTASK_H_



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
		class PIDInnerLoopTask : public Task
		{
			private:
				PIDController *pidController;
				
			public:

				PIDInnerLoopTask (PIDController *pidController, int delay, int period);

				void runTaskImpl();
		};
	}
}





#endif /* PIDINNERLOOPTASK_H_ */
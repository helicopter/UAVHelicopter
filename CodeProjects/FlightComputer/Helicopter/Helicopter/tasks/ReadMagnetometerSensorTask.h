/*
 * ReadMagnetometerSensor.h
 *
 * Created: 4/8/2014 8:31:31 PM
 *  Author: HP User
 */ 


#ifndef READMAGNETOMETERSENSORTASK_H_
#define READMAGNETOMETERSENSORTASK_H_



#include "Task.h"
#include "SystemModel.h"
#include "MagnetometerSensor.h"

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
		class ReadMagnetometerSensorTask : public Task
		{
			private:
				SystemModel *model;
				MagnetometerSensor *magnetometerSensor;
				
			public:

				ReadMagnetometerSensorTask (SystemModel *model, MagnetometerSensor *magnetometerSensor, int delay, int period);

				void runTaskImpl();
		};
	}
}






#endif /* READMAGNETOMETERSENSORTASK_H_ */
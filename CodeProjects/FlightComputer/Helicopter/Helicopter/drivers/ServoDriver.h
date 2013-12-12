/*
 * ServoDriver.h
 *
 * Created: 9/30/2013 6:53:49 PM
 *  Author: HP User
 */ 


#ifndef SERVODRIVER_H_
#define SERVODRIVER_H_



namespace helicopter 
{
	namespace drivers
	{
		/**
		 * This class is used for interfacing with servos.
		 */
		class ServoDriver
		{
			protected: 
				double minControlValue;
				double maxControlValue;
					
				
			public:
				ServoDriver(): 
					minControlValue(0), 
					maxControlValue(0)
				{
					
				}
				
				virtual ~ServoDriver()
				{
					
				}
				
				double MinControlValue()
				{
					return minControlValue;
				}
				
				void MinControlValue(double minControlValue)
				{
					this->minControlValue = minControlValue;
				}
				
				double MaxControlValue()
				{
					return maxControlValue;
				}
				
				void MaxControlValue(double maxControlValue)
				{
					this->maxControlValue = maxControlValue;
				}
				
				
				/**
				 * Sends a signal to the tail rotor servo to control the tail rotor to the appropriate
				 * control value. This function converts the control value to a servo signal.
				 * This implements the logic for scaling the control commands.
				 * @param controlTailRotorCollective collective value to send to the servo. Generally between
				 * minTailRotorCollectiveControl and maxTailRotorCollectiveControl
				 */
				void controlTailRotorCollective(double controlTailRotorCollective);
				
				/**
				 * Implementation for the actual control of the tail rotor. This will convert
				 * the control signal into pulse width modulation scaling the control value by the
				 * given min and max control values. This method is virtual for unit test purposes.
				 */
				virtual void controlTailRotorCollectiveImpl(double controlTailRotorCollective);
				
				void controlLongitudinal(double controlLogitudinal);
				
				void controlMainRotorCollective( double mainRotorControl );
		};
	}
}


#endif /* SERVODRIVER_H_ */
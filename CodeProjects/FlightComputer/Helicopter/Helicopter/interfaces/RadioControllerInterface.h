/*
 * RadioControllerInterface.h
 *
 * Created: 1/25/2014 12:49:16 PM
 *  Author: HP User
 */ 


#ifndef RADIOCONTROLLERINTERFACE_H_
#define RADIOCONTROLLERINTERFACE_H_

#include <string.h>
#include "SystemModel.h"

using namespace helicopter::model;

namespace helicopter
{
	namespace interfaces
	{
		/**
		 * Interface for communicating with the Radio Controller used to
		 * pilot the helicopter manually. The servo controller send signals
		 * to the Ardupilot board via a variety of pins in Pulse Width Modulation (PWM) format.
		 * However, on the board the ATMEGA32U2-MU processor converts the PWM signal
		 * into Pulse Position Modulation (PPM) and sends that to the Atmega2560 processor
		 * via one pin (the PL1 pin). This is so that the only one pin on the processor is used for
		 * processing servo signals. 
		 * This class interprets those PPM signals and converts it into control values.
		 *
		 * The functionality and code of this class is similar to that defined in:
		 * APM_RC_APM2.cpp of the ardupilot project.
		 */
		class RadioControllerInterface
		{
			
			public: 
			
			
				/**
				 * Servo channel for aileron (roll)
				 * Input pin 1 on the ardupilot 
				 */
				static const int AILERON_CHANNEL = 0;
				
				/**
				 * Servo channel for elevator (pitch)
				 * Input pin 2 on the ardupilot 
				 */
				static const int ELEVATOR_CHANNEL = 1;
				
				/**
				 * Servo channel for throttle (collective heave)
				 * Input pin 3 on the ardupilot 
				 */				
				static const int THROTTLE_CHANNEL = 2;
				
				/**
				 * Servo channel for rudder (yaw)
				 * Input pin 4 on the ardupilot 
				 */				
				static const int RUDDER_CHANNEL = 3;
				
				/**
				 * Servo channel for auxiliary
				 * Input pin 5 on the ardupilot 
				 * This servo channel is used to indicate if the system is in
				 * autopilot mode (using the PID controller), or manual mode
				 * (using the normal radio controller to control the helicopter).
				 */	
				static const int FLIGHT_MODE_AUX_CHANNEL = 4;
			
			
			
			
				/**
				 * This value represents what the servo control value has to be
				 * below in order for the system to be in 'manual mode'.
				 * The servo values go from -1, to 1, so 0 is in the 'middle'.
				 */
				static const float MANUAL_MODE_THRESHOLD = 0.0;
				
				
				
				/*
				* This value represents the maximum value that the ppm timer will reach before restarting
				* This value is somewhat arbitrarily chosen based on an arbitrary frequency.
				* The value is calculated by:
				*  (CPU_Speed / PreScaler) / DesiredFrequency
				* So in this case, the desired frequency for the timer to reset is 50 times a second E.g.
				* (16,000,000 / 8) / 50 = 40,000
				*/
				static const long MAX_PPM_TIMER_VALUE = 40000;
				//static const long MAX_PPM_TIMER_VALUE = (F_CPU / PRESCALEVALUE) / PWMFREQUENCY;
				
				/**
				 * This represents the highest value that the timer will reach before counting down again.
				 */
				//static const int TIMERTOP = 20000;				
				static const int TIMERTOP = MAX_PPM_TIMER_VALUE / 2;				
				
				/**
				 * 'Width' in processor ticks of the maximum pulse that we would want to send or receive 
				 * to the servos. The max servo pulse width is 2 ms, so this is 2 ms in timer ticks.
				 * This is calculated by (Desired pulse width in ms / max ppm timer value in ms) * max ppm timer value in ticks = desired pulse width in ticks
				 * e.g. (2ms / 20 ms (which is 1/50hz)) * 40000 = 4000
				 * A few ticks are added and removed to allow for noise.
				 */
				static const long MAX_USEABLE_PULSE_WIDTH = 4010;
				
				static const long MIN_USEABLE_PULSE_WIDTH = 1990;
				
				
				/**
				 * This represents the compare match value for the timer for the minimum usable value for pwm.
				 * I.e. this is the point in ticks where the timer will hit in order to create a pulse width of 1 ms.
				 * the equation is:
				 * (MAX_PPM_TIMER_VALUE - min pulse width ticks) / 2
				 * e.g. (40,000 - 2000) / 2 = 19000
				 */
				static const long PWM_COMPAREMATCH_MIN_TICKS = 19000;
				

				static const long PWM_COMPAREMATCH_MAX_TICKS = 18000;
				
				
				/**
				 * This number represents the minimum time interval between PPM
				 * pulses to indicate that the PPM has restarted back at
				 * servo channel 1. This number was taken from the
				 * ardupilot project.
				 */
				static const int SYNCH_PULSE_WIDTH = 8000;
				
				/**
				 * The maximum number of channels received by the radio controller
				 */
				static const int MAX_CHANNELS = 8;
				
				
				/**
				 * We need to check to see if the servo channel index is greater than
				 * a minimum number of channels threshold. This is because if
				 * there are some channels not plugged in, it will cause the 
				 * servo channel index to reset back to 0, and re-write the
				 * existing servo value. This could cause a servo control value
				 * to get sent to the wrong servo. 
				 */
				static const int MIN_RECEIVED_CHANNELS = 5;
				
			
			private:
			

			
				long previousInputCaptureRegisterValue;
				
				int servoChannelIndex;

				SystemModel *systemModel;
				
				/**
				 * The value of the servo controls received from the radio transmitter
				 * The pulse widths are converted to a value between -1 and 1 and
				 * stored in this table.
				 */
				float servoChannelValues[MAX_CHANNELS];

				static RadioControllerInterface *radioControllerInterface;
			
				RadioControllerInterface():
				systemModel (NULL)
				{
					memset(servoChannelValues, 0, sizeof(servoChannelValues));
					
					previousInputCaptureRegisterValue = 0;
					
					servoChannelIndex = 0;
				}
						
				~RadioControllerInterface()
				{
				}
				
				/**
				 * Calculates the pwm compare match value for the On Compare Match Register
				 * given a control signal value between -1.0 and 1.0
				 */
				float calculatePWMCompareMatchFromControlValue(float controlValue);

			public: 

					
				/**
				 * If the RadioControllerInterface hasn't been created yet, then construct the 
				 * RadioControllerInterface pseudo-'singleton' and return it.
				 * This is because this class uses interrupt handlers that falls outside of
				 * the scope of this class, so the handlers need a static reference to
				 * this class.
				 */
				static RadioControllerInterface  *getRadioControllerInterface();
				
				long PreviousInputCaptureRegisterValue() const { return previousInputCaptureRegisterValue; }
				void PreviousInputCaptureRegisterValue(long val) { previousInputCaptureRegisterValue = val; }
					
				int ServoChannelIndex() const { return servoChannelIndex; }
				void ServoChannelIndex(int val) { servoChannelIndex = val; }
					
				SystemModel * GetSystemModel() const { return systemModel; }
				void SetSystemModel(SystemModel *val) { systemModel = val; }		
					
				void SetServoChannelValue(int index, float value) 
				{ 
					if (index < MAX_CHANNELS)
					{
						servoChannelValues[index] = value;
					}
				}		
				
				float GetServoChannelValue(int index) const
				{
					if (index < MAX_CHANNELS)
					{
						return servoChannelValues[index];
					}else
					{
						return 0;
					}
				}						
				
				void init();

				void start();
		};
	}
}



#endif /* RADIOCONTROLLERINTERFACE_H_ */
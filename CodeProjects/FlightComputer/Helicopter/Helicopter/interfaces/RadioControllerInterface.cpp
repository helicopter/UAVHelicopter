#include <avr/io.h>
#include <avr/interrupt.h>

#include "RadioControllerInterface.h"

using namespace helicopter::interfaces;

RadioControllerInterface* RadioControllerInterface::radioControllerInterface = NULL;


float ScaleValue(int servoChannelPulseWidth) 
{
	float scaledValue = (1.0f - -1.0f) / (4000.0f - 2000.0f) * (servoChannelPulseWidth - 2000.0f) + -1.0f;
			
	if (scaledValue > 1.0f)
	{
		scaledValue = 1.0f;
	}else if (scaledValue < -1.0f)
	{
		scaledValue = -1.0f;
	}
	
	return scaledValue;
}

/**
 * Interrupt service routine which is called when the PPM timer reaches the top 
 * (when the timer counts up to MAX_PPM_TIMER_VALUE)
 */
//This shouldn't be needed because we shouldn't care if the timer hits the top.
/*ISR(TIMER5_COMPB_vect)
{
	//Intentionally do nothing
}*/

/**
 * Interrupt service routine which is called when a PPM signal's rising edge is detected
 * on the PL1 pin.
 */
ISR(TIMER5_CAPT_vect)
{
	RadioControllerInterface *rcInterface = RadioControllerInterface::getRadioControllerInterface();
	
	//Disable interrupts while reading a 16 bit register to avoid
	//an interrupt happening inbetween reading the two bytes. 
	cli(); 
	
	//The Input Capture Register is set to the value of the timer when the
	//input signal was received.
	long currentInputCaptureRegisterValue = ICR5;
	
	sei();
	
	long servoChannelPulseWidth = 0;
	
	long previousInputCaptureRegisterValue = rcInterface->PreviousInputCaptureRegisterValue();
	
	//If the current value was less, then the timer has overflowed since the last
	//time we received a pulse
	if (currentInputCaptureRegisterValue < previousInputCaptureRegisterValue)
	{
		servoChannelPulseWidth = (currentInputCaptureRegisterValue + RadioControllerInterface::MAX_PPM_TIMER_VALUE) - previousInputCaptureRegisterValue;
	}else
	{
		//Calculate the width of the pulse.
		servoChannelPulseWidth = currentInputCaptureRegisterValue - previousInputCaptureRegisterValue;
	}
	
	rcInterface->PreviousInputCaptureRegisterValue(currentInputCaptureRegisterValue);
	
	//Once all the servo channels values have been sent, a sync pulse is sent indicating
	//a restart back to the first channel.
	//Detect if it's a sync pulse and rest as appropriate
	if (servoChannelPulseWidth > RadioControllerInterface::SYNCH_PULSE_WIDTH)
	{
		rcInterface->ServoChannelIndex(0);
	}else
	{
		if (servoChannelPulseWidth < RadioControllerInterface::MAX_USEABLE_PULSE_WIDTH && 
			servoChannelPulseWidth > RadioControllerInterface::MIN_USEABLE_PULSE_WIDTH)
		{
			int servoChannelIndex = rcInterface->ServoChannelIndex();
		
			if (servoChannelIndex < RadioControllerInterface::MAX_CHANNELS)
			{
				SystemModel *model = rcInterface->GetSystemModel();
				
				//Scale the servo channel pulse widths to between
				//-1, and 1 which is what the system uses for servo control.
				//-1 is 1.0 milliseconds, and 1 is 2.0 milliseconds (0 is 1.5 milliseconds)
				//new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
				float servoControlValue = ScaleValue(servoChannelPulseWidth);

				rcInterface->SetServoChannelValue(servoChannelIndex, servoControlValue);
			
				/**
				 * We need to check to see if the servo channel index is greater than
				 * a minimum number of channels threshold. This is because if
				 * there are some channels not plugged in, it will cause the 
				 * servo channel index to reset back to 0, and re-write the
				 * existing servo value. This could cause a servo control value
				 * to get sent to the wrong servo. 
				 */
				if (servoChannelIndex >= RadioControllerInterface::MIN_RECEIVED_CHANNELS)
				{
					//Manual mode
					if (rcInterface->GetServoChannelValue(RadioControllerInterface::FLIGHT_MODE_AUX_CHANNEL)
						 < RadioControllerInterface::MANUAL_MODE_THRESHOLD)
					{
						model->OperationalState(SystemModel::ManualControl);
					}else
					{
						model->OperationalState(SystemModel::AutoPilot);
					}

					//If in Manual Control, set the servo control values.
					if (model->OperationalState() == SystemModel::ManualControl)
					{
						model->LateralControl(rcInterface->GetServoChannelValue(RadioControllerInterface::AILERON_CHANNEL));
						model->LongitudeControl(rcInterface->GetServoChannelValue(RadioControllerInterface::ELEVATOR_CHANNEL));
						model->MainRotorCollectiveControl(rcInterface->GetServoChannelValue(RadioControllerInterface::THROTTLE_CHANNEL));
						model->YawControl(rcInterface->GetServoChannelValue(RadioControllerInterface::RUDDER_CHANNEL));
					}
					
					rcInterface->ServoChannelIndex(0);
				}			
			}
		}

		rcInterface->ServoChannelIndex(rcInterface->ServoChannelIndex() + 1);
	}
	
}



RadioControllerInterface* RadioControllerInterface::getRadioControllerInterface()
{
	if (radioControllerInterface == NULL)
	{
		radioControllerInterface = new RadioControllerInterface();
	}

	return radioControllerInterface;
}




/**
 * Provides a high frequency single slope wave form.
 * (The timer only counts up, then resets, it's doesn't count back down like other PWM waveforms).
 * This counter will be used to measure the time intervals between the PPM pulses
 * to calculate the value of the control signal from the radio being sent to the servos.
 * See page 148 of atmelAVR2560_datasheet.pdf
 */
void RadioControllerInterface::init()
{
	//set PL1 pin to input. This will receive the
	//pulse position modulation signal from the pwm-ppm encoder
	DDRL &= ~(1<<PL1);
	
	//Set the Wave Form Generation Mode pins (WGM)
	//Setup timer for Fast Pulse Width Modulation where the top
	//of the timer (when the timer resets) is set to the 
	//value given to the OCR5A register.
	//Timer Counter Overflow (TOV) flag is set when the timer reaches the TOP
	//
	TCCR5A = (1<<WGM50)|(1<<WGM51);
	TCCR5B = (1<<WGM52)|(1<<WGM53);
	
	//ICES5 is the input capture edge select pin. when set to 1, it triggers a capture event on a rising edge.
	TCCR5B |= (1<<ICES5); 
	
	//Setup the timer to reset once it reaches the MAX_PPM_TIMER_VALUE value 
	// (i.e. the timer will reset 50 times a second because the timer will reach 40,000 in 1/50th of a second)
	OCR5A = MAX_PPM_TIMER_VALUE; 

	//Enable global interrupts
	sei();
	
	// Enable Input Capture interrupt so the interrupt will fire when a PPM signal is received.
	TIMSK5 |= (1<<ICIE5);
}

void RadioControllerInterface::start()
{
	//Set the timer prescaler to 8. (CS = Clock Select) which starts the timer.
	TCCR5B |= (1<<CS51);	
}

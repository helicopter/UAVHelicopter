#include <avr/io.h>
#include <avr/interrupt.h>

#include "RadioControllerInterface.h"

using namespace helicopter::interfaces;

RadioControllerInterface* RadioControllerInterface::radioControllerInterface = NULL;


/*
//const float RadioControllerInterface::SUBTRIM_POINT_PER_TENTH_MS = .1f/50.0f;
const float RadioControllerInterface::SUBTRIM_POINT_PER_TENTH_MS = .1f/47.0f;
//const float RadioControllerInterface::SUBTRIM_POINT_PER_TENTH_MS = .1f/20.0f;
//const float RadioControllerInterface::SUBTRIM_POINT_PER_TENTH_MS = .1f/7.0f;

const float RadioControllerInterface::AILERON_SUBTRIM = 43.0f;
const float RadioControllerInterface::PITCH_SUBTRIM = 10.0f;
//const float RadioControllerInterface::PITCH_SUBTRIM = 10.0f * -2.0f;
const float RadioControllerInterface::ELEVATOR_SUBTRIM = -24.0f;
//const float RadioControllerInterface::ELEVATOR_SUBTRIM = -50.0f;
*/




//const float RadioControllerInterface::MANUAL_MODE_THRESHOLD = 0.0;
const float RadioControllerInterface::MANUAL_MODE_THRESHOLD = 0.7;
//const float RadioControllerInterface::MANUAL_MODE_THRESHOLD = 0.3;


//const float RadioControllerInterface::THROTTLE_VALUE = .80;
//const float RadioControllerInterface::THROTTLE_VALUE = .20;
const float RadioControllerInterface::THROTTLE_VALUE = -.55f;
				
const float RadioControllerInterface::GEAR_VALUE = 0.0;
				
const float RadioControllerInterface::AUX3_VALUE = 0.0;



const long RadioControllerInterface::TIMERTOP = (long) (MAX_PPM_TIMER_VALUE / 2.0f);


const long RadioControllerInterface::MAX_PULSE_WIDTH = (long)((2.0f/20.0f)*MAX_PPM_TIMER_VALUE); //the 2 is the number of milliseconds for the max pwm value.i.e. max servo control is a 2ms pulse. The 20 is how many milliseconds in the frame. i.e. 50 hz rate is 20 ms per frame. 
const long RadioControllerInterface::MIN_PULSE_WIDTH = (long)((1.0f/20.0f)*MAX_PPM_TIMER_VALUE);
	
const long RadioControllerInterface::MAX_USEABLE_PULSE_WIDTH = MAX_PULSE_WIDTH + 10;
const long RadioControllerInterface::MIN_USEABLE_PULSE_WIDTH = MIN_PULSE_WIDTH - 10;


	

//The values coming from the PWM-PPM is from 2120 us to 920us according to code found here:https://github.com/diydrones/ardupilot/blob/2874ec67c7a111f09fd57add336a99461e567b73/Tools/ArduPPM/Libraries/PPM_Encoder_v3.h
/*const long RadioControllerInterface::MAX_PULSE_WIDTH = (long)((2.12f/20.0f)*MAX_PPM_TIMER_VALUE);
const long RadioControllerInterface::MIN_PULSE_WIDTH = (long)((.920f/20.0f)*MAX_PPM_TIMER_VALUE);

const long RadioControllerInterface::MAX_USEABLE_PULSE_WIDTH = MAX_PULSE_WIDTH;
const long RadioControllerInterface::MIN_USEABLE_PULSE_WIDTH = MIN_PULSE_WIDTH;
*/
	
				
const long RadioControllerInterface::PWM_COMPAREMATCH_MIN_TICKS = (long)((RadioControllerInterface::MAX_PPM_TIMER_VALUE - RadioControllerInterface::MIN_PULSE_WIDTH) / 2.0f);
const long RadioControllerInterface::PWM_COMPAREMATCH_MAX_TICKS = (long)((RadioControllerInterface::MAX_PPM_TIMER_VALUE - RadioControllerInterface::MAX_PULSE_WIDTH) / 2.0f);

const long RadioControllerInterface::PWM_NEUTRAL_POSITION = (long) ((PWM_COMPAREMATCH_MAX_TICKS + PWM_COMPAREMATCH_MIN_TICKS)/2);


const float RadioControllerInterface::SWASHOFFSETPITCHMULTIPLYER = 4.0f;
const float RadioControllerInterface::SWASHOFFSETELEVATORMULTIPLYER = 2.0f;
const float RadioControllerInterface::SWASHOFFSETELEVATORELEVATORMULTIPLYER = 4.0f;
const float RadioControllerInterface::SWASHOFFSETSAILERONMULTIPLIER = 3.37209302325581;

float RadioControllerInterface::calculatePWMCompareMatchFromControlValue(float controlValue)
{
	//Equation for scaling a value: new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
	float scaledValue = (PWM_COMPAREMATCH_MAX_TICKS - PWM_COMPAREMATCH_MIN_TICKS) / (1.0f - -1.0f) * (controlValue - -1.0f) + PWM_COMPAREMATCH_MIN_TICKS;
	
	if (scaledValue > PWM_COMPAREMATCH_MIN_TICKS)
	{
		scaledValue = PWM_COMPAREMATCH_MIN_TICKS;
	}else if (scaledValue < PWM_COMPAREMATCH_MAX_TICKS)
	{
		scaledValue = PWM_COMPAREMATCH_MAX_TICKS;
	}
	
	return scaledValue;	
	
}


/**
 * Equation for scaling values new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
 */
float RadioControllerInterface::ScaleValue(long servoChannelPulseWidth) 
{
	//float scaledValue = (1.0f - -1.0f) / (4000.0f - 2000.0f) * (servoChannelPulseWidth - 2000.0f) + -1.0f;
	float scaledValue = (1.0f - -1.0f) / (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * (servoChannelPulseWidth - MIN_PULSE_WIDTH) + -1.0f;
			
	if (scaledValue > 1.0f)
	{
		scaledValue = 1.0f;
	}else if (scaledValue < -1.0f)
	{
		scaledValue = -1.0f;
	}
	
	return scaledValue;
}

float RadioControllerInterface::convertPulseWidthToCompareMatch(long pulseWidth)
{
	return (MAX_PPM_TIMER_VALUE - pulseWidth) / 2.0;
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
	//cli(); //not necessary because intterupts are already disabled within an ISR.
	
	//The Input Capture Register is set to the value of the timer when the
	//input signal was received.
	long currentInputCaptureRegisterValue = ICR5;
	
	//sei();
	
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
				//float servoControlValue = ScaleValue(servoChannelPulseWidth);

				//rcInterface->SetServoChannelPulseWidth(servoChannelIndex, servoControlValue);
				rcInterface->SetServoChannelPulseWidth(servoChannelIndex, servoChannelPulseWidth);
			
				/**
				 * We need to check to see if the servo channel index is greater than
				 * a minimum number of channels threshold. This is because if
				 * there are some channels not plugged in, it will cause the 
				 * servo channel index to reset back to 0, and re-write the
				 * existing servo value. This could cause a servo control value
				 * to get sent to the wrong servo. 
				 */
				//if (servoChannelIndex >= RadioControllerInterface::MIN_RECEIVED_CHANNELS)
				if (servoChannelIndex == RadioControllerInterface::MIN_RECEIVED_CHANNELS)
				{
					//Manual mode
					float auxChannelValue = rcInterface->ScaleValue(rcInterface->GetServoChannelPulseWidth(RadioControllerInterface::FLIGHT_MODE_AUX_CHANNEL));
					
					if (auxChannelValue
						 <= RadioControllerInterface::MANUAL_MODE_THRESHOLD)
					{
						model->OperationalState(SystemModel::ManualControl);
					}else
					{
						model->OperationalState(SystemModel::AutoPilot);
					}
					
					model->AuxChannelValue(auxChannelValue);
					
					if (!rcInterface->ARRAYLOCK)
					{
						rcInterface->copyPulseWidthArrays();
					}
					

					//If in Manual Control, set the servo control values.
					/*if (model->OperationalState() == SystemModel::ManualControl)
					{
						//TODO: CCPM->nonCCPM conversion
						
						model->LateralControl(rcInterface->ScaleValue(rcInterface->GetServoChannelPulseWidth(RadioControllerInterface::AILERON_CHANNEL)));
						model->LongitudeControl(rcInterface->ScaleValue(rcInterface->GetServoChannelPulseWidth(RadioControllerInterface::ELEVATOR_CHANNEL)));
						model->MainRotorCollectiveControl(rcInterface->ScaleValue(rcInterface->GetServoChannelPulseWidth(RadioControllerInterface::THROTTLE_CHANNEL)));
						model->YawControl(rcInterface->ScaleValue(rcInterface->GetServoChannelPulseWidth(RadioControllerInterface::RUDDER_CHANNEL)));
						
					}*/
					
					//rcInterface->ServoChannelIndex(0);
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
	/**
	 * Setup code for converting control values to pulse width modulation values for the
	 * servos.
	 * Output:
	 * Port Pin | on compare counter | output type					| Output pin on Board
	 * PB6      | OC1B               | Aileron (roll)				| 1
	 * PB5      | OC1A               | Elevator (pitch)				| 2
	 * PH5      | OC4C               | Throttle (collective)		| 3
	 * PH4      | OC4B               | Rudder (yaw)					| 4
	 * PH3      | OC4A               | Aux							| 5
	 * PE5      | OC3C               | N/A							| 6
	 * PE4      | OC3B               | N/A							| 7
	 * PE3      | OC3A               | N/A							| 8
	 */
	
	/**
	 * Setup code for converting control values to pulse width modulation values for the
	 * servos.
	 * Output:
	 * Port Pin | on compare counter | output type					| Output pin on Board
	 * PB6      | OC1B               | Thro							| 1
	 * PB5      | OC1A               | Aile							| 2
	 * PH5      | OC4C               | Elev							| 3
	 * PH4      | OC4B               | Rudd							| 4
	 * PH3      | OC4A               | Gear/Gain					| 5
	 * PE5      | OC3C               | Aux1/Pit						| 6
	 * PE4      | OC3B               | Aux 2 (manual override)		| 7
	 * PE3      | OC3A               | Aux 3						| 8
	 */	
	
	//Setup pins for output.
	DDRB |= (1<<PB6) | (1<<PB5);
	DDRH |= (1<<PH5) | (1<<PH4) | (1<<PH3);
	DDRE |= (1<<PE5) | (1<<PE4) | (1<<PE3);
	
	
	/**
	 * Setup timers for phase correct pwm.
	 * wgmn3, wgmn2, wgmn1, wgmn0 = 1,0,1,0 (page 148)
	 */
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13);
	TCCR3A |= (1<<WGM31);
	TCCR3B |= (1<<WGM33);	
	TCCR4A |= (1<<WGM41);
	TCCR4B |= (1<<WGM43);
	
	/**
	 * Setup OC1A,B compare match when counting up on compare, clear compare match on compare when counting down.
	 */
	TCCR1A |= (1<<COM1B1) | (1<<COM1B0) | (1<<COM1A1) | (1<<COM1A0);
	TCCR3A |= (1<<COM3C1) | (1<<COM3C0) | (1<<COM3B1) | (1<<COM3B0) | (1<<COM3A1) | (1<<COM3A0);
	TCCR4A |= (1<<COM4C1) | (1<<COM4C0) | (1<<COM4B1) | (1<<COM4B0) | (1<<COM4A1) | (1<<COM4A0);
	
	
	
	//TODO: Set default values to '0' before initialized so that things don't power up.
	/**
	 * Set the compare value so that the servos are set to a neutral position
	 */
	OCR1B = calculatePWMCompareMatchFromControlValue(0);
	OCR1A = calculatePWMCompareMatchFromControlValue(0);
	OCR3C = calculatePWMCompareMatchFromControlValue(0);
	OCR3B = calculatePWMCompareMatchFromControlValue(0);
	OCR3A = calculatePWMCompareMatchFromControlValue(0);
	OCR4C = calculatePWMCompareMatchFromControlValue(0);
	OCR4B = calculatePWMCompareMatchFromControlValue(0);
	OCR4A = calculatePWMCompareMatchFromControlValue(0);
	
	ICR1 = TIMERTOP;
	ICR3 = TIMERTOP;
	ICR4 = TIMERTOP;
	
	




/*
 TCCR1A =((1<<WGM11)|(1<<COM1A1)|(1<<COM1B1)|(1<<COM1C1)); //Please read page 131 of DataSheet, we are changing the registers settings of WGM11,COM1B1,COM1A1 to 1 thats all...
 TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11); //Prescaler set to 8, that give us a resolution of 2us, read page 134 of data sheet
 OCR1A = 3000; //PB5, none
 OCR1B = 3000; //PB6, OUT2
 OCR1C = 3000; //PB7  OUT3
 ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,

  TCCR3A =((1<<WGM31)|(1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1)); //Please read page 131 of DataSheet, we are changing the registers settings of WGM11,COM1B1,COM1A1 to 1 thats all...
  TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31); //Prescaler set to 8, that give us a resolution of 2us, read page 134 of data sheet
  OCR3A = 3000; //PE3, NONE
  OCR3B = 3000; //PE4, OUT7
  OCR3C = 3000; //PE5,  OUT6
  ICR3 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,



  TCCR4A =((1<<WGM41)|(1<<COM4A1)|(1<<COM4B1)|(1<<COM4C1));
  TCCR4B = (1<<WGM43)|(1<<WGM42)|(1<<CS41); //Prescaler set to 8
  OCR4A = 3000; //PL3,
  OCR4B = 3000; //PL4, OUT0
  OCR4C = 3000; //PL5  OUT1
  ICR4 = 40000;




  TCCR5A =((1<<WGM50)|(1<<WGM51)|(1<<COM5C1)|(1<<COM5B1)|(1<<COM5A1));
  TCCR5B = ((1<<WGM53)|(1<<WGM52)|(1<<CS51)|(1<<ICES5)); //Prescaler set to 8, that give us a resolution of 2us, read page 135 of data sheet
  OCR5A = 50000; ///50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=50000,
  //must be 50hz because is the servo standard (every 20 ms, and 1hz = 1sec) 1000ms/20ms=50hz, elementary school stuff...
  OCR5B = 3000; //PH5, OUT5
  OCR5C = 3000; //PH5, OUT5
  
  TIMSK5 |= (1<<ICIE5); //Timer interrupt mask
*/















	
	/**
	 * Setup code for receiving pulse position modulation (PPM) information and converting
	 * it to control values
	 */
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
	//Starts the PPM input timer
	TCCR5B |= (1<<CS51);	
	
	
	
	//Wait for a few readings to come in, and then measure the offset between what the system calculates as 'zero' and 
	//what the transmitter is transmitting as 0. This requires the sticks to be at neutral (0) at startup (sept for throttle)
	//This is done because the 3gx system has some sort of algorithm which will cause the swashplate to freak out if you switch from
	//manual mode, to autopilot and i think it's because the '0' position between manual mode and autopilot is off.
	bool haveInitialData = false;
	
	while (!haveInitialData)
	{
		_delay_ms(1000);

		if (servoChannelPulseWidths[7] != 0)
		{
			haveInitialData = true;
		}
	}
	

	
	
	/**
	 * Calculate an offset between what the system thinks is the starting position, and what the
	 * radio controller thinks is the starting position. The sticks on the controller should be neutral
	 * except for the collective which should be all the way down. 
	 */
	float outPitch = 0;
	float outAileron = 0;
	float outElevator = 0;
		
	/**
	 * Calculate what the system thinks is the value of having the collective stick all the way down (-1),
	 * and the other sticks neutral (0)
	 */	
	CCPM(0, 0, -1, outAileron, outElevator, outPitch);
	
	int zeroPoint = calculatePWMCompareMatchFromControlValue(0);
	int negativeOnePoint = calculatePWMCompareMatchFromControlValue(-1);
	
	/*
	channel1Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[0]);
	channel2Offset = calculatePWMCompareMatchFromControlValue(outAileron) - convertPulseWidthToCompareMatch(servoChannelPulseWidths[1]);
	channel3Offset = calculatePWMCompareMatchFromControlValue(outElevator) - convertPulseWidthToCompareMatch(servoChannelPulseWidths[2]);
	channel4Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[3]);
	channel5Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[4]);
	channel6Offset = calculatePWMCompareMatchFromControlValue(outPitch) - convertPulseWidthToCompareMatch(servoChannelPulseWidths[5]);
	channel7Offset = 0;//AUX don't want an offset. 
	channel8Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[7]);
	*/
	
	channel1Offset = negativeOnePoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[0]); //Throttle starts at -1. 
	channel2Offset = outAileron - convertPulseWidthToCompareMatch(servoChannelPulseWidths[1]);
	channel3Offset = outElevator - convertPulseWidthToCompareMatch(servoChannelPulseWidths[2]);
	channel4Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[3]);
	channel5Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[4]);
	channel6Offset = outPitch - convertPulseWidthToCompareMatch(servoChannelPulseWidths[5]);
	channel7Offset = 0;//AUX don't want an offset.
	channel8Offset = zeroPoint - convertPulseWidthToCompareMatch(servoChannelPulseWidths[7]);
	
	
	

	

	

			
	
	

	//Starts the PWM output timers
	TCCR1B |= (1<<CS11);
	TCCR3B |= (1<<CS31);
	TCCR4B |= (1<<CS41);

}


void RadioControllerInterface::CCPM(float inAileron, float inElevator, float inCollective, float &outAileron, float &outElevator, float &outPitch)
{
	//look at spreadsheet for the math behind this (swashmixingaltorithm.xlsx). calculated based on observation of the behavior of my remote control.
	outPitch = PWM_NEUTRAL_POSITION+PITCH_COLLECTIVE_SCALER*SWASHOFFSETPITCHMULTIPLYER*inCollective*-1 + ELEVATOR_SCALER*SWASHOFFSETELEVATORMULTIPLYER*inElevator+AILERON_SCALER*SWASHOFFSETSAILERONMULTIPLIER*inAileron*-1;
	outAileron = PWM_NEUTRAL_POSITION+PITCH_COLLECTIVE_SCALER*SWASHOFFSETPITCHMULTIPLYER*inCollective+ELEVATOR_SCALER*SWASHOFFSETELEVATORMULTIPLYER*inElevator*-1+AILERON_SCALER*SWASHOFFSETSAILERONMULTIPLIER*inAileron*-1;
	outElevator = PWM_NEUTRAL_POSITION+PITCH_COLLECTIVE_SCALER*SWASHOFFSETPITCHMULTIPLYER*inCollective*-1 + ELEVATOR_SCALER*SWASHOFFSETELEVATORELEVATORMULTIPLYER*inElevator*-1;
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	/*inAileron = inAileron + AILERON_SUBTRIM * SUBTRIM_POINT_PER_TENTH_MS;
	inElevator = inElevator + ELEVATOR_SUBTRIM * SUBTRIM_POINT_PER_TENTH_MS;
	inCollective = inCollective + PITCH_SUBTRIM * SUBTRIM_POINT_PER_TENTH_MS;*/
	
	
	///**
	 //* Scale the input values by the given scaler offsets. 
	 //*/
	//float adjustedAileron = (float) inAileron * (AILERON_SCALER / (float)ZERO_SCALER);
	//float adjustedElevator = (float) inElevator * (ELEVATOR_SCALER / (float)ZERO_SCALER);
	//float adjustedPitchCollective = (float) inCollective * (PITCH_COLLECTIVE_SCALER / (float)ZERO_SCALER);
	//
	///**
	 //* Mix the inputs together
	 //*/
    //float p = adjustedPitchCollective + adjustedAileron - adjustedElevator / 2.0f;
    //float a = -adjustedPitchCollective + adjustedAileron + adjustedElevator / 2.0f;
    //float el = adjustedPitchCollective + adjustedElevator;	
	//
	///**
	 //* Scale the outputs
	 //*/
	//outPitch = (1.0f - -1.0f) / (2.5f - -2.5f) * (p - -2.5f) + -1.0f;
	//outAileron = (1.0f - -1.0f) / (2.5f - -2.5f) * (a - -2.5f) + -1.0f;
	//outElevator = (1.0f - -1.0f) / (2.0f - -2.0f) * (el - -2.0f) + -1.0f;
	
	
	
	/*
	outPitch = outPitch + PITCH_SUBTRIM * SUBTRIM_POINT_PER_TENTH_MS;
	outAileron = outAileron + AILERON_SUBTRIM * SUBTRIM_POINT_PER_TENTH_MS;
	outElevator =  outElevator + ELEVATOR_SUBTRIM * SUBTRIM_POINT_PER_TENTH_MS;
	*/
	
}

void RadioControllerInterface::controlServos( float lateralControl, float longitudeControl, float mainRotorControl, float yawControl, float auxChannelValue )
{

	
	//If in manual control, just forward the pulse width values from the input pins to the output pins.
	if (systemModel->OperationalState() == SystemModel::ManualControl)
	{
		
		
//long jj = PWM_COMPAREMATCH_MIN_TICKS;
//long bb = PWM_COMPAREMATCH_MAX_TICKS;


		/*
		OCR1B = 3000;
		OCR1A = 3000;
		OCR4C = 3000;
		OCR4B = 3000;
		OCR4A = 3000;
		OCR3C = 3000;
		OCR3B = 3000;
		OCR3A = 3000;
		*/
		
		
		
		
		
		
		
		
		
		
		
		
		/*
		OCR1B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[0]);
		OCR1A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[1]);
		OCR4C = convertPulseWidthToCompareMatch(servoChannelPulseWidths[2]);
		OCR4B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[3]);
		OCR4A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[4]);
		OCR3C = convertPulseWidthToCompareMatch(servoChannelPulseWidths[5]);
		OCR3B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[6]);
		OCR3A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[7]);
		*/
		
		ARRAYLOCK = true;
		OCR1B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[0]);
		OCR1A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[1]);
		OCR4C = convertPulseWidthToCompareMatch(servoChannelPulseWidths[2]);
		OCR4B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[3]);
		OCR4A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[4]);
		OCR3C = convertPulseWidthToCompareMatch(servoChannelPulseWidths[5]);
		OCR3B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[6]);
		OCR3A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[7]);
		ARRAYLOCK = false;
		
		/* Removed because without ccpm -> non ccpm, this doesn't make sense.
		systemModel->LateralControl(ScaleValue(GetServoChannelPulseWidth(RadioControllerInterface::AILERON_CHANNEL)));
		systemModel->LongitudeControl(ScaleValue(GetServoChannelPulseWidth(RadioControllerInterface::ELEVATOR_CHANNEL)));
		systemModel->MainRotorCollectiveControl(ScaleValue(GetServoChannelPulseWidth(RadioControllerInterface::THROTTLE_CHANNEL)));
		systemModel->YawControl(ScaleValue(GetServoChannelPulseWidth(RadioControllerInterface::RUDDER_CHANNEL)));
		*/
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/*
		OCR1B = (servoChannelPulseWidths[0]);
		OCR1A = (servoChannelPulseWidths[1]);
		OCR4C = (servoChannelPulseWidths[2]);
		OCR4B = (servoChannelPulseWidths[3]);
		OCR4A = (servoChannelPulseWidths[4]);
		OCR3C = (servoChannelPulseWidths[5]);
		OCR3B = (servoChannelPulseWidths[6]);
		OCR3A = (servoChannelPulseWidths[7]);*/
	} else if (systemModel->OperationalState() == SystemModel::AutoPilot)
	{
		//TODO - nonccpm-ccpm this stuff. 

		float outPitch = 0;
		float outAileron = 0;
		float outElevator = 0;
		
		
		
/*

CCPM(-0.015898047, -0.07133783, 0.467764974, outAileron, outElevator, outPitch);

OCR1B = calculatePWMCompareMatchFromControlValue(THROTTLE_VALUE);
OCR1A = calculatePWMCompareMatchFromControlValue(outAileron);
OCR4C = calculatePWMCompareMatchFromControlValue(outElevator);
OCR4B = calculatePWMCompareMatchFromControlValue(0.533155441);
OCR4A = calculatePWMCompareMatchFromControlValue(GEAR_VALUE);
OCR3C = calculatePWMCompareMatchFromControlValue(outPitch);
OCR3B = calculatePWMCompareMatchFromControlValue(auxChannelValue);
OCR3A = calculatePWMCompareMatchFromControlValue(AUX3_VALUE);
	*/	
		
		
		
		
		
		
		
		
		
		
		
		
		
		/**
			* Setup code for converting control values to pulse width modulation values for the
			* servos.
			* Output:
			* Port Pin | on compare counter | output type					| Output pin on Board
			* PB6      | OC1B               | Thro							| 1
			* PB5      | OC1A               | Aile							| 2
			* PH5      | OC4C               | Elev							| 3
			* PH4      | OC4B               | Rudd							| 4
			* PH3      | OC4A               | Gear/Gain					| 5
			* PE5      | OC3C               | Aux1/Pit						| 6
			* PE4      | OC3B               | Aux 2 (manual override)		| 7
			* PE3      | OC3A               | Aux 3						| 8
			*/			
		
		
		
		CCPM(lateralControl, longitudeControl, mainRotorControl, outAileron, outElevator, outPitch);
		
		
		//OCR1B = calculatePWMCompareMatchFromControlValue(THROTTLE_VALUE) - channel1Offset;
		
		ARRAYLOCK = true;
		OCR1B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[0]);
		ARRAYLOCK = false;
		
		OCR1A = outAileron - channel2Offset;
		OCR4C = outElevator - channel3Offset;
		OCR4B = calculatePWMCompareMatchFromControlValue(yawControl) - channel4Offset;
		OCR4A = calculatePWMCompareMatchFromControlValue(GEAR_VALUE) - channel5Offset;
		OCR3C = outPitch - channel6Offset;
		OCR3B = calculatePWMCompareMatchFromControlValue(auxChannelValue) - channel7Offset;
		OCR3A = calculatePWMCompareMatchFromControlValue(AUX3_VALUE) - channel8Offset;
		
		/*
		OCR1B = calculatePWMCompareMatchFromControlValue(THROTTLE_VALUE) - channel1Offset;
		OCR1A = calculatePWMCompareMatchFromControlValue(outAileron) - channel2Offset;
		OCR4C = calculatePWMCompareMatchFromControlValue(outElevator) - channel3Offset;
		OCR4B = calculatePWMCompareMatchFromControlValue(yawControl) - channel4Offset;
		OCR4A = calculatePWMCompareMatchFromControlValue(GEAR_VALUE) - channel5Offset;
		OCR3C = calculatePWMCompareMatchFromControlValue(outPitch) - channel6Offset;
		OCR3B = calculatePWMCompareMatchFromControlValue(auxChannelValue) - channel7Offset;
		OCR3A = calculatePWMCompareMatchFromControlValue(AUX3_VALUE) - channel8Offset;
		*/
		
		
		
		
		
		
		
		
		
		/*
		OCR1B = calculatePWMCompareMatchFromControlValue(THROTTLE_VALUE);
		OCR1A = calculatePWMCompareMatchFromControlValue(outAileron);
		OCR4C = convertPulseWidthToCompareMatch(servoChannelPulseWidths[2]);
		OCR4B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[3]);
		OCR4A = convertPulseWidthToCompareMatch(servoChannelPulseWidths[4]);
		OCR3C = calculatePWMCompareMatchFromControlValue(outPitch);
		OCR3B = convertPulseWidthToCompareMatch(servoChannelPulseWidths[6]);
		OCR3A = calculatePWMCompareMatchFromControlValue(AUX3_VALUE);
		*/
		
		
		
		
		
		
		
		
		
		
		
		/*OCR1B = calculatePWMCompareMatchFromControlValue(1.0f);
		OCR1A = calculatePWMCompareMatchFromControlValue(.50);
		OCR4C = calculatePWMCompareMatchFromControlValue(0);
		OCR4B = calculatePWMCompareMatchFromControlValue(-.50f);
		OCR4A = calculatePWMCompareMatchFromControlValue(-.8f);
		OCR3C = calculatePWMCompareMatchFromControlValue(-.8f);
		OCR3B = calculatePWMCompareMatchFromControlValue(-.8f);
		OCR3A = calculatePWMCompareMatchFromControlValue(-.8f);*/

	}

}

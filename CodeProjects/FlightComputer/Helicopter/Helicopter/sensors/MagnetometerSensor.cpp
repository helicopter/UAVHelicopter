/*
 * MagnetometerSensor.cpp
 *
 * Created: 8/22/2013 12:15:07 AM
 *  Author: HP User
 */ 
#include <math.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "MagnetometerSensor.h"
#include "MatrixUtil.h"

using namespace helicopter::sensors;
using namespace helicopter::util;

#define COMPASS_MAX_INSTANCES 1
    bool _null_init_done = false;                           ///< first-time-around flag used by offset nulling


//Implements bus clear method to clear out the i2c bus in the event of an error. 
//http://davinci-linux-open-source.1494791.n2.nabble.com/PATCH-v1-i2c-Davinci-i2c-bus-recovery-procedure-to-come-out-of-time-out-conditions-td4051690.html
/*#define CHECK(A) if(!A){ \
	TWCR &= ~(1<<TWEN); \
	TWCR = (1<<TWINT) | (1<<TWEN); \
	PORTD |= (1<<PD1); \
	for (int i = 0; i < 9; i++) \
	{ \
		PORTD &= ~(1<<PD0); \
		_delay_us(20); \
		PORTD |= (1<<PD0); \
		_delay_us(20); \
	} \
	driver->stop(); \
	return -1;\
	}
*/
//I disable the i2c bus ~(1<<TWEN) because this somehow 'frees' up the line with the magnetometer. 
//Before, i was running into an issue where the radio was interfering with the magnetometer i2c lines
//causing it to lock up and nothing would unlike the line besides a reset. I couldn't re-initiate a start command
//or anything. freeing the SCL (twen) solved that issue. This isn't documented anywhere it was through trial and error. 
#define CHECK(A) if(!A){ \
	driver->reset(); \
	return -1;\
}

static const uint8_t _mag_history_size = 20;


float _mag_history[_mag_history_size][3] = {};
float _field[3] = {};     ///< magnetic field strength
	
uint8_t _mag_history_index = 0;

const float min_diff = 50.0;


// don't allow any axis of the offset to go above 2000
#define COMPASS_OFS_LIMIT 2000

float MagnetometerSensor::_offset[3] = {};
float MagnetometerSensor::debug[3] = {};
	
float constrain_float(float amt, float low, float high)
{
	// the check for NaN as a float prevents propogation of
	// floating point errors through any function that uses
	// constrain_float(). The normal float semantics already handle -Inf
	// and +Inf
	if (isnan(amt)) {
		return (low+high)*0.5f;
	}
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}



/**
 * Correct for magnetic interference. Due to time, I'm basically copying the code from ardupilot found here:
 * https://github.com/diydrones/ardupilot/blob/6af705d4554defc27aa475dab99f918b26de3ce1/libraries/AP_Compass/Compass_learn.cpp
 */
void MagnetometerSensor::learn_offsets(void)
{

			
	_field[0] = frdMagX;
	_field[1] = frdMagY;
	_field[2] = frdMagZ;
	
	
    //const float gain = 0.01;
	const float gain = 0.01;
    const float max_change = 10.0;
    //const float min_diff = 50.0;
	const float min_diff = 10.0;
	
	if (!_null_init_done) {
		// first time through
		_null_init_done = true;

			for (uint8_t i=0; i<_mag_history_size; i++) {
				// fill the history buffer with the current mag vector,
				// with the offset removed
				float curmagvec[3] = {(_field[0]+0.5f) - _offset[0], (_field[1]+0.5f) - _offset[1], (_field[2]+0.5f) - _offset[2]};
				memcpy(_mag_history[i], curmagvec, sizeof(curmagvec));
				
			}
		_mag_history_index = 0;
		return;
	}
	
	
	
	
        
        float b1[3], diff[3];
        float length;

        if (isnan(_offset[0]) || isnan(_offset[1]) || isnan(_offset[2])) {
	        // offsets are bad possibly due to a past bug - zero them
	        _offset[0] = 0;
			_offset[1] = 0;
			_offset[2] = 0;
        }

		
        // get a past element
		b1[0] = _mag_history[_mag_history_index][0];
		b1[1] = _mag_history[_mag_history_index][1];
		b1[2] = _mag_history[_mag_history_index][2];

        // the history buffer doesn't have the offsets
		/*b1[0] += _offset[0];
		b1[1] += _offset[1];
		b1[2] += _offset[2];*/

        // get the current vector
		float b2[3] = {_field[0], _field[1], _field[2]};
      

        // calculate the delta for this sample
		diff[0] = b2[0] - b1[0];
		diff[1] = b2[1] - b1[1];
		diff[2] = b2[2] - b1[2];
		
        //length = diff.length();
		length = sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
/*debug[0] = diff[0];		
debug[1] = diff[1];
debug[2] = diff[2];*/
        if (length < min_diff) {
	        // the mag vector hasn't changed enough - we don't get
	        // enough information from this vector to use it.
	        // Note that we don't put the current vector into the mag
	        // history here. We want to wait for a larger rotation to
	        // build up before calculating an offset change, as accuracy
	        // of the offset change is highly dependent on the size of the
	        // rotation.
	        _mag_history_index = (_mag_history_index + 1) % _mag_history_size;
	        return;
        }

        // put the vector in the history
		//float temp[3] = {(_field[0]+0.5f) - _offset[0],  (_field[1]+0.5f) - _offset[1], (_field[2]+0.5f) - _offset[2]};
			float temp[3] = {(_field[0]) - _offset[0],  (_field[1]) - _offset[1], (_field[2]) - _offset[2]};
		memcpy(_mag_history[_mag_history_index], temp, sizeof(temp));

        _mag_history_index = (_mag_history_index + 1) % _mag_history_size;

        // equation 6 of Bills paper
		float b1length = sqrt(b1[0]*b1[0] + b1[1]*b1[1] + b1[2]*b1[2]);
		float b2length = sqrt(b2[0]*b2[0] + b2[1]*b2[1] + b2[2]*b2[2]);
		
		float lengthGain = (gain * (b2length - b1length) / length);
		
		diff[0] = diff[0] * lengthGain;
		diff[1] = diff[1] * lengthGain;
		diff[2] = diff[2] * lengthGain;
		
		

        // limit the change from any one reading. This is to prevent
        // single crazy readings from throwing off the offsets for a long
        // time
		float difflength = sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
        length = difflength;
        if (length > max_change) {
	        diff[0] *= max_change / length;
			diff[1] *= max_change / length;
			diff[2] *= max_change / length;
        }

		float new_offsets[3] = {_offset[0]-diff[0], _offset[1]-diff[1], _offset[2] - diff[2]};

        if (isnan(new_offsets[0]) || isnan(new_offsets[1]) || isnan(new_offsets[2])) {
	        // don't apply bad offsets
	        return;
        }

        // constrain offsets
        new_offsets[0] = constrain_float(new_offsets[0], -COMPASS_OFS_LIMIT, COMPASS_OFS_LIMIT);
        new_offsets[1] = constrain_float(new_offsets[1], -COMPASS_OFS_LIMIT, COMPASS_OFS_LIMIT);
        new_offsets[2] = constrain_float(new_offsets[2], -COMPASS_OFS_LIMIT, COMPASS_OFS_LIMIT);
        
        // set the new offsets	
		_offset[0] = new_offsets[0];
		_offset[1] = new_offsets[1];
		_offset[2] = new_offsets[2];
	
}

MagnetometerSensor::MagnetometerSensor(TWIDriver *driver):
	rawMagX(0),
	rawMagY(0),
	rawMagZ(0),
	frdMagX(0),
	frdMagY(0),
	frdMagZ(0),
	driver(driver)
{
	/**
		* Create a rotation matrix to rotate the magnetometer sensor data by
		* 180 degrees on the x axis, then 0 degrees on the y axis, then 90 degrees 
		* counterclockwise on the Z axis.
		*/
	memset(magLBUToFRDRotationMatrix,0,sizeof(magLBUToFRDRotationMatrix));
					
	//MatrixUtil::CreateRotationMatrix(M_PI, 0.0f, -1 * (M_PI/2), magLBUToFRDRotationMatrix);
	MatrixUtil::CreateRotationMatrix(0.0f, 0.0f, (M_PI/2.0f), magLBUToFRDRotationMatrix);
	//MatrixUtil::CreateRotationMatrix(M_PI, 0.0f, -1 * (M_PI/2), magLBUToFRDRotationMatrix);
}




float calibration[3] = {};
bool readyForOffsets = false;

bool MagnetometerSensor::init()
{

//driver->stop();
	/**
	* Configure the output rate of the sensor, and number of samples that are averaged
	* per measurement output
	*/
	
	//Start a two wire transaction
	if(!driver->start())
	{
		return false;
	}
	
	//Indicate that the next value is a write operation 
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}
		
	//Write to the device so that the 'pointer' moves to register A
	if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}
		
	//Write the configuration value to register A
	if (!driver->write(REGISTER_A_CONFIGURATION_75HZ_8AVG, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}	

	//end the transaction
	driver->stop();
	
	
	
	
	
	
	
	
	
	//Start a two wire transaction
	if(!driver->start())
	{
		return false;
	}
	
	//Indicate that the next value is a write operation
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}
	
	//Write to the device so that the 'pointer' moves to register B
	if (!driver->write(CONFIGURATION_REGISTER_B, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}
	
	//Write the configuration value to register B
	if (!driver->write(REGISTER_B_CONFIGURATION_660_GAIN, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//end the transaction
	driver->stop();	
	
	
	
	
	
	
calibration[0] = 1;
calibration[1] = 1;
calibration[2] = 1;	
	/*
	float expected_x = 766;
	float expected_yz = 713;
	*/
	//
	float expected_x = -713;//766;
	float expected_y = 766;//713;
	float expected_z = 713;
	
	    int numAttempts = 0, good_count = 0;
	    bool success = false;
		byte PositiveBiasConfig = 0x11;
		byte calibration_gain = 0x60;
	
	calibration[0] = 0;
	calibration[1] = 0;
	calibration[2] = 0;

    while ( success == 0 && numAttempts < 25 && good_count < 5)
    {
        // record number of attempts at initialisation
        numAttempts++;



// force positiveBias (compass should return 715 for all channels)
	//Start a two wire transaction
	if(!driver->start())
	{
		return false;
	}
	
	//Indicate that the next value is a write operation
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}
	
	//Write to the device so that the 'pointer' moves to register B
	if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}
	
	//Write the configuration value to register B
	if (!driver->write(PositiveBiasConfig, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//end the transaction
	driver->stop();




        
        _delay_ms(50);

        // set gains
	if(!driver->start())
	{
		return false;
	}
	
	//Indicate that the next value is a write operation
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}
	
	//Write to the device so that the 'pointer' moves to register B
	if (!driver->write(CONFIGURATION_REGISTER_B, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}
	
	//Write the configuration value to register B
	if (!driver->write(calibration_gain, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//end the transaction
	driver->stop();		
	
	
	        _delay_ms(50);

	        // set gains
	        if(!driver->start())
	        {
		        return false;
	        }
	        
	        //Indicate that the next value is a write operation
	        if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	        {
		        return false;
	        }
	        
			
	        //Write to the device so that the 'pointer' moves to register B
	        if (!driver->write(MODE_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	        {
		        return false;
	        }
	        byte singleConversion = 0x01;
	        //Write the configuration value to register B
	        if (!driver->write(singleConversion, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	        {
		        return false;
	        }

	        //end the transaction
	        driver->stop();
	        
	        
	
	
	
	
	
	
	
	
		
		_delay_ms(50);
		
        // read values from the compass
       readSensor();

		_delay_ms(50);

        float cal[3];

        // hal.console->printf_P(PSTR("mag %d %d %d\n"), _mag_x, _mag_y, _mag_z);
        cal[0] = fabsf(expected_x / (float)frdMagX);
        cal[1] = fabsf(expected_y / (float)frdMagY);
        cal[2] = fabsf(expected_z / (float)frdMagZ);





        // hal.console->printf_P(PSTR("cal=%.2f %.2f %.2f\n"), cal[0], cal[1], cal[2]);

        // we throw away the first two samples as the compass may
        // still be changing its state from the application of the
        // strap excitation. After that we accept values in a
        // reasonable range
        if (numAttempts > 2 &&
            cal[0] > 0.7f && cal[0] < 1.35f &&
            cal[1] > 0.7f && cal[1] < 1.35f &&
            cal[2] > 0.7f && cal[2] < 1.35f) {
//        if (numAttempts > 2 ) {				
            // hal.console->printf_P(PSTR("cal=%.2f %.2f %.2f good\n"), cal[0], cal[1], cal[2]);
            good_count++;
            calibration[0] += cal[0];
            calibration[1] += cal[1];
            calibration[2] += cal[2];
        }


    }

    if (good_count >= 5) {
        /*
          The use of gain_multiple below is incorrect, as the gain
          difference between 2.5Ga mode and 1Ga mode is already taken
          into account by the expected_x and expected_yz values.  We
          are not going to fix it however as it would mean all
          APM1/APM2 users redoing their compass calibration. The
          impact is that the values we report on APM1/APM2 are lower
          than they should be (by a multiple of about 0.6). This
          doesn't have any impact other than the learned compass
          offsets
         */
        calibration[0] = calibration[0]  / good_count;
        calibration[1] = calibration[1]  / good_count;
        calibration[2] = calibration[2]  / good_count;
        success = true;
    } else {
        /* best guess */
        calibration[0] = 1.0;
        calibration[1] = 1.0;
        calibration[2] = 1.0;
    }
	
	
	
	
	
	
	readyForOffsets = true;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//Start a two wire transaction
	if(!driver->start())
	{
		return false;
	}
	
	//Indicate that the next value is a write operation
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}
	
	//Write to the device so that the 'pointer' moves to register A
	if (!driver->write(CONFIGURATION_REGISTER_A, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}
	
	//Write the configuration value to register A
	if (!driver->write(REGISTER_A_CONFIGURATION_75HZ_8AVG, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//end the transaction
	driver->stop();	
	
	
	byte primary_gain = 0x20;
	       // set gains
	       if(!driver->start())
	       {
		       return false;
	       }
	       
	       //Indicate that the next value is a write operation
	       if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	       {
		       return false;
	       }
	       
	       //Write to the device so that the 'pointer' moves to register B
	       if (!driver->write(CONFIGURATION_REGISTER_B, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	       {
		       return false;
	       }
	       
	       //Write the configuration value to register B
	       if (!driver->write(primary_gain, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	       {
		       return false;
	       }

	       //end the transaction
	       driver->stop();
	
	
	
	
	
	
	
	
	
	
	


	/**
	* Set the compass mode for continuous measurement
	*/
	//start a transaction
	if(!driver->start())
	{
		return false;
	}
		
	//Indicate that the next value is a write operation
	if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		return false;
	}		
		
	//Write to the device so that the 'pointer' moves to the mode register
	if (!driver->write(MODE_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//Write the configuration value to the mode register
	if (!driver->write(COMPASS_MODE_CONTINUOUS_MEASUREMENT_MODE, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		return false;
	}

	//end the transaction
	driver->stop();
	
	return true;
}















float _offsets2[3] = {0};
float previousMag[3] = {.001, .001, .001};
int MagnetometerSensor::readSensor()
{
		
//driver->stop();
		
	/**
	* Setup device to start reading mag data
	*/
	/*if (!driver->start())
	{
		driver->stop();
		return -1;
	}*/
	CHECK(driver->start());
	
	/**
		* Send a command to the sensor indicating that the next value
		* is a write operation. I.e. we will tell the sensor what internal address
		* to jump to.
		*/
	/*if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK))
	{
		driver->stop();
		return -1;
	}*/		
	CHECK(driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | WRITE_OPERATION, TWIDriver::MASTERTRANSMIT_SLAVE_WRITE_ACK));
	
	

		
	/**
		* Send the address of the magnetic X register to the sensor
		* to move the internal 'cursor' to that position so the subsequent 
		* read command will return the value at that address
		*/
	/*if (!driver->write(DATA_OUTPUT_X_MSG_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK))
	{
		driver->stop();
		return -1;
	}*/
	CHECK(driver->write(DATA_OUTPUT_X_MSG_REGISTER, TWIDriver::MASTERTRANSMIT_DATA_ACK));
		

	//(from atmega2560 doc)
	//In order to enter 'master receiver' mode (the mode we enter into in the next command), a start condition must be entered.
	/*if (!driver->start())
	{
		driver->stop();
		return -1;
	}*/
	CHECK(driver->start());
	
	/**
		* Tell the sensor to read data from the address given above, and send it
		* to us.
		*/
	/*if (!driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | READ_OPERATION, TWIDriver::MASTERRECEIVER_SLAVE_READ_ACK))
	{
		driver->stop();
		return -1;
	}*/
	CHECK(driver->write(MAGNETOMETER_TWOWIRE_SENSOR_ADDRESS | READ_OPERATION, TWIDriver::MASTERRECEIVER_SLAVE_READ_ACK));
	
	
		
	byte highByte = 0;
	byte lowByte = 0;
		
	CHECK(driver->readByte(true, highByte));
	CHECK(driver->readByte(true, lowByte));
	rawMagX = (highByte << 8) | (lowByte);
		
	CHECK(driver->readByte(true, highByte));
	CHECK(driver->readByte(true, lowByte));
	rawMagZ = (highByte << 8) | (lowByte);
		
	CHECK(driver->readByte(true, highByte));
		
	//We don't send an acknowledgment on the last byte to be read (the magnetic Y values low byte) because of an apparent undocumented requirement by the device
	//that the nack needs to be sent to have the internal pointer go back to the magnetic X variable.
	//According to wikipedia, when in masterreceive mode, the master sends an ack bit after every byte read except for the last one. 
	CHECK(driver->readByte(false, lowByte));
	rawMagY = (highByte << 8) | (lowByte);		
	
	//transmit stop condition
	driver->stop();
	
	//Convert the raw values to FRD values.
	float rotatedValues[3] = {0};
		
		
		
		
		
		
	/**
	 * correct for magnetic interference. Code from ardupilot file found here:
	 * https://github.com/diydrones/ardupilot/blob/6af705d4554defc27aa475dab99f918b26de3ce1/libraries/AP_Compass/Compass_learn.cpp
	 */
	if (readyForOffsets)
	{
		rawMagX *= calibration[0];
		rawMagY *= calibration[1];
		rawMagZ *= calibration[2];
		
	}
		
		
		
	
	int values[3] = {rawMagX, rawMagY, rawMagZ};
	
	MatrixUtil::RotateMatrix(magLBUToFRDRotationMatrix, values, rotatedValues);
	
	
	
	frdMagX = rotatedValues[0];
	frdMagY = rotatedValues[1];
	frdMagZ = rotatedValues[2];
		
		
		
	
		
	//x2+y2+z2=mag2
	//mag2-x2-y2=z2
	//sqrroot	
	
	/*
	float mx = frdMagX / 100.0f;
	float my = frdMagY / 100.0f;
	float magnitude = 570.0f / 100.0f;
	*/
	
	//This code is used as a work around for when my Z axis wasn't working on my magnetometer. 
	//float neg = frdMagZ<0? -1.0f:1.0f;	
	//frdMagZ = sqrt(fabsf(570.0f*570.0f - frdMagX*frdMagX-frdMagY*frdMagY)) * neg;
	////frdMagZ = (sqrt(fabsf(magnitude*magnitude - mx*mx-my*my)) * neg) * 100.0f;
		
		
		
		
		
		
		
		
		
		
		
		
	/**
	 * correct for magnetic interference. Code from ardupilot file found here:
	 * https://github.com/diydrones/ardupilot/blob/6af705d4554defc27aa475dab99f918b26de3ce1/libraries/AP_Compass/Compass_learn.cpp
	 */
	if (readyForOffsets)
	{
		/*
		debug[0] = frdMagX;
		debug[1] = calibration[0];
		debug[2] = _offset[0];
		*/

/*		latest 6/14/2014
		frdMagX *= calibration[0];
		frdMagY *= calibration[1];
		frdMagZ *= calibration[2];
	*/	
		
		
		
	//apply offsets
	/*frdMagX += _offset[0];
	frdMagY += _offset[1];
	frdMagZ += _offset[2];*/	
	
	/*
debug[0] = calibration[0];
debug[1] = calibration[1];
debug[2] = calibration[2];
*/
	

	
	//
	//
	//
	//const float gain2 = .10f;
	//const float errorThreshold = 5.0f;
	//
	//
	//if (previousMag[0] != 0 && previousMag[1] != 0 && previousMag[2] !=0)
	//{
		///**
		//* Add offsets now. Don't store with offsets. this way we have a 'baseline' of values to add offsets to.
		//* if you add the offsets before the mag values are stored, then you keep 'adding' to values that are then 
		//* added onto again. if you don't store the offsets, then you have a historical 'baseline' of values. 
		//*/
		//float b2[3] = {frdMagX - _offsets2[0], frdMagY - _offsets2[1], frdMagZ - _offsets2[2]};
		//float b1[3] = {previousMag[0] - _offsets2[0], previousMag[1] - _offsets2[1], previousMag[2] - _offsets2[2]};
	//
		//float diff[3] = {b2[0] - b1[0], b2[1] - b1[1], b2[2] - b1[2]};
		//float magDiff = sqrt(diff[0] * diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
	//
		////have a threshold so the offsets dont keep increasing. 
		////if (magDiff > errorThreshold)
		//{
			//
		//
			//float magb2 = sqrt(b2[0] * b2[0] + b2[1]*b2[1] + b2[2]*b2[2]);
			//float magb1 = sqrt(b1[0]*b1[0] + b1[1]*b1[1] + b1[2]*b1[2]);
			//float diffMags = magb2 - magb1;
	//
			//if (magDiff != 0)
			//{
				///*
				//_offsets2[0] = _offsets2[0] + gain2 * ((diff[0] / magDiff) * diffMags);
				//_offsets2[1] = _offsets2[1] + gain2 * ((diff[1] / magDiff) * diffMags);
				//_offsets2[2] = _offsets2[2] + gain2 * ((diff[2] / magDiff) * diffMags);
				//*/
				//
				//_offsets2[0] = gain2 * ((diff[0] / magDiff) * diffMags);
				//_offsets2[1] = gain2 * ((diff[1] / magDiff) * diffMags);
				//_offsets2[2] = gain2 * ((diff[2] / magDiff) * diffMags);
			//}
		//}
		//
			//debug[0] = magDiff;
			//debug[1] = diff[1];
			//debug[2] = diff[2];
			//
	//}
	//
	//
	//previousMag[0] = frdMagX;
	//previousMag[1] = frdMagY;
	//previousMag[2] = frdMagZ;
	//
	///*
	//frdMagX -= _offsets2[0];
	//frdMagY -= _offsets2[1];
	//frdMagZ -= _offsets2[2];
	//*/
	//
	//
	//
	////debug[0] = _offsets2[0];
	//debug[1] = _offsets2[1];
	//debug[2] = _offsets2[2];
	//
	//
//
	//
	//
	//
	//
	//
	///*
			//debug[0] = _offset[0];
			//debug[1] = _offset[1];
			//debug[2] = _offset[2];*/
		//learn_offsets();
	}
	
		
		
		
		
		
		
		
		
		
	return 0;
}
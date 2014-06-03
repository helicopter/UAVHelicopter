/*
 * GainsMessage.cpp
 *
 * Created: 5/29/2014 4:57:08 PM
 *  Author: HP User
 */ 

#include "GainsMessage.h"

#include <string.h>

using namespace helicopter::messages;


byte *GainsMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	
	encode (msgPtr, msgType);
	
	encode (msgPtr, YawIntegralGain);
	encode (msgPtr, YawDerivativeGain);
	encode (msgPtr, YawProportionalGain);
	encode (msgPtr, YawAntiWindupGain);
	
	encode (msgPtr, XIntegralGain);
	encode (msgPtr, XDerivativeGain);
	encode (msgPtr, XProportionalGain);
	encode (msgPtr, XAntiWindupGain);
	encode (msgPtr, LongitudeInnerLoopGain);
	encode (msgPtr, PitchAngularVelocityGain);	
	
	
	
	encode (msgPtr, YIntegralGain);
	encode (msgPtr, YDerivativeGain);
	encode (msgPtr, YProportionalGain);
	encode (msgPtr, YAntiWindupGain);
	encode (msgPtr, LateralInnerLoopGain);
	encode (msgPtr, RollAngularVelocityGain);	
	
	
	encode (msgPtr, ZIntegralGain);
	encode (msgPtr, ZDerivativeGain);
	encode (msgPtr, ZProportionalGain);
	encode (msgPtr, ZAntiWindupGain);	
	
	encode (msgPtr, XRefSetpoint);
	encode (msgPtr, YRefSetpoint);
	encode (msgPtr, YawRefSetpoint);
	
	return msg;
}

void GainsMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		decode (message,msgType);
		

		decode (message, YawIntegralGain);
		decode (message, YawDerivativeGain);
		decode (message, YawProportionalGain);
		decode (message, YawAntiWindupGain);
		
		
		decode (message, XIntegralGain);
		decode (message, XDerivativeGain);
		decode (message, XProportionalGain);
		decode (message, XAntiWindupGain);
		decode (message, LongitudeInnerLoopGain);
		decode (message, PitchAngularVelocityGain);	
		
		
		decode (message, YIntegralGain);
		decode (message, YDerivativeGain);
		decode (message, YProportionalGain);
		decode (message, YAntiWindupGain);
		decode (message, LateralInnerLoopGain);
		decode (message, RollAngularVelocityGain);		
			
			
		decode (message, ZIntegralGain);
		decode (message, ZDerivativeGain);
		decode (message, ZProportionalGain);
		decode (message, ZAntiWindupGain);			
			
			
		decode (message, XRefSetpoint);
		decode (message, YRefSetpoint);
		decode (message, YawRefSetpoint);
			
	}
}

GainsMessage* GainsMessage::buildMessageSt(byte *message)
{
	GainsMessage *msg = new GainsMessage();
	msg->buildMessage(message);
	
	return msg;
}

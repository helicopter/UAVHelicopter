/*
 * SensorDataMessage.cpp
 *
 * Created: 4/11/2014 8:38:55 PM
 *  Author: HP User
 */ 
#include "SensorDataMessage.h"

using namespace helicopter::messages;

byte *SensorDataMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	
	encode (msgPtr, msgType);
	encode (msgPtr, XAccelFrdMss);
	encode (msgPtr, YAccelFrdMss);
	encode (msgPtr, ZAccelFrdMss);
	encode (msgPtr, YawAngularVelocityRadsPerSecond);
	encode (msgPtr, PitchAngularVelocityRadsPerSecond);
	encode (msgPtr, RollAngularVelocityRadsPerSecond);
	encode (msgPtr, XMagFrd);
	encode (msgPtr, YMagFrd);
	encode (msgPtr, ZMagFrd);
	encode (msgPtr, XEcefCm);
	encode (msgPtr, YEcefCm);
	encode (msgPtr, ZEcefCm);
	encode (msgPtr, XVEcefCms);
	encode (msgPtr, YVEcefCms);
	encode (msgPtr, ZVEcefCms);
	encode (msgPtr, PressureMillibars);
	
	
	return msg;
}

void SensorDataMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		decode (message,msgType);
		decode (message, XAccelFrdMss);
		decode (message, YAccelFrdMss);
		decode (message, ZAccelFrdMss);
		decode (message, YawAngularVelocityRadsPerSecond);
		decode (message, PitchAngularVelocityRadsPerSecond);
		decode (message, RollAngularVelocityRadsPerSecond);
		decode (message, XMagFrd);
		decode (message, YMagFrd);
		decode (message, ZMagFrd);
		decode (message, XEcefCm);
		decode (message, YEcefCm);
		decode (message, ZEcefCm);
		decode (message, XVEcefCms);
		decode (message, YVEcefCms);
		decode (message, ZVEcefCms);
		decode (message, PressureMillibars);
	}
}

SensorDataMessage* SensorDataMessage::buildMessageSt(byte *message)
{
	SensorDataMessage *msg = new SensorDataMessage();
	msg->buildMessage(message);
	
	return msg;
}
/*
 * SimpleTelemetryMessage.cpp
 *
 * Created: 4/19/2014 8:06:07 PM
 *  Author: HP User
 */ 

#include "SimpleTelemetryMessage.h"

#include <string.h>

using namespace helicopter::messages;


byte *SimpleTelemetryMessage::getBytes()
{
	byte *msg = new byte[MessageSize];
	byte *msgPtr = msg;
	
	encode (msgPtr, msgType);
	

	encode (msgPtr, XNEDLocalFrame);
	encode (msgPtr, XVelocityFRDCms);
	encode (msgPtr, PitchRads);
	
	
	encode (msgPtr, YNEDLocalFrame);
	encode (msgPtr, YVelocityFRDCms);
	encode (msgPtr, RollRads);
	

	encode (msgPtr, ZVelocityFRDCms);
	encode (msgPtr, YawRads);
	encode (msgPtr, ZNEDLocalFrame);
	
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
	
	
	
	
	
	
	encode (msgPtr, Timeouts);
	encode (msgPtr, UnrecognizedMsgTypes);
	encode (msgPtr, ChecksumErrors);
	encode (msgPtr, NumOfBlownFrames);
	encode (msgPtr, SerialCommunicationBufferOverruns);
	
	return msg;
}

void SimpleTelemetryMessage::buildMessage(byte *message)
{
	if (message != NULL)
	{
		decode (message,msgType);
		

		decode (message, XNEDLocalFrame);
		decode (message, XVelocityFRDCms);
		decode (message, PitchRads);
		

		decode (message, YNEDLocalFrame);
		decode (message, YVelocityFRDCms);
		decode (message, RollRads);
		
		

		decode (message, ZVelocityFRDCms);
		decode (message, YawRads);
		decode (message, ZNEDLocalFrame);
		
						
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

		
		
		
		decode (message, Timeouts);
		decode (message, UnrecognizedMsgTypes);
		decode (message, ChecksumErrors);
		decode (message, NumOfBlownFrames);
		decode (message, SerialCommunicationBufferOverruns);
	}
}

SimpleTelemetryMessage* SimpleTelemetryMessage::buildMessageSt(byte *message)
{
	SimpleTelemetryMessage *msg = new SimpleTelemetryMessage();
	msg->buildMessage(message);
	
	return msg;
}

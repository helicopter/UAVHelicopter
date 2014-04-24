using GroundControlStation.Model;
using System;
using System.Collections.Generic;
using System.Text;
using GroundControlStation.Util;

namespace GroundControlStation.Messages
{
    public class SimpleTelemetryMessage : Message
    {
        public const byte MessageType = 6;





        public float XNEDLocalFrame;
        public float XVelocityFRDCms;
        public float PitchRads;

        

        public float YNEDLocalFrame;
        public float YVelocityFRDCms;
        public float RollRads;
        

        public float ZVelocityFRDCms;
        public float YawRads;
        public float ZNEDLocalFrame;


        public float XAccelFrdMss;
        public float YAccelFrdMss;
        public float ZAccelFrdMss;
        public float YawAngularVelocityRadsPerSecond;
        public float PitchAngularVelocityRadsPerSecond;
        public float RollAngularVelocityRadsPerSecond;
        public float XMagFrd;
        public float YMagFrd;
        public float ZMagFrd;
        public int XEcefCm;
        public int YEcefCm;
        public int ZEcefCm;
        public int XVEcefCms;
        public int YVEcefCms;
        public int ZVEcefCms;
        public float PressureMillibars;


        public int Timeouts;
        public int UnrecognizedMsgTypes;
        public int ChecksumErrors;
        public int NumOfBlownFrames;
        public int SerialCommunicationBufferOverruns;

        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        public const int NumOfBytesInMsg =
            sizeof(byte) +
            
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +





            

            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            

            sizeof(float) +
            sizeof(float) +
            sizeof(float) +

            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) +
            sizeof(int) +
            sizeof(int) +  
            sizeof(int) + 
            sizeof(int) + 
            sizeof(int) +
            sizeof(int) + 
            sizeof(float) + 

            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int);


        public SimpleTelemetryMessage()
            : base(MessageType, NumOfBytesInMsg)
        {
        }

        public static SimpleTelemetryMessage BuildMessageSt(byte[] byteBuffer)
        {
            SimpleTelemetryMessage fct = new SimpleTelemetryMessage();
            fct.BuildMessage(byteBuffer);

            return fct;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);



            XNEDLocalFrame = decodeFloat(byteBuffer, ref positionCounter);
            XVelocityFRDCms = decodeFloat(byteBuffer, ref positionCounter);
            PitchRads = decodeFloat(byteBuffer, ref positionCounter);
            

            YNEDLocalFrame = decodeFloat(byteBuffer, ref positionCounter);
            YVelocityFRDCms = decodeFloat(byteBuffer, ref positionCounter);
            RollRads = decodeFloat(byteBuffer, ref positionCounter);
            
            ZVelocityFRDCms = decodeFloat(byteBuffer, ref positionCounter);
            YawRads = decodeFloat(byteBuffer, ref positionCounter);
            
            ZNEDLocalFrame = decodeFloat(byteBuffer, ref positionCounter);



            XAccelFrdMss = decodeFloat(byteBuffer, ref positionCounter);
            YAccelFrdMss = decodeFloat(byteBuffer, ref positionCounter);
            ZAccelFrdMss = decodeFloat(byteBuffer, ref positionCounter);
            YawAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            PitchAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            RollAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            XMagFrd = decodeFloat(byteBuffer, ref positionCounter);
            YMagFrd = decodeFloat(byteBuffer, ref positionCounter);
            ZMagFrd = decodeFloat(byteBuffer, ref positionCounter);
            XEcefCm = decodeInt(byteBuffer, ref positionCounter);
            YEcefCm = decodeInt(byteBuffer, ref positionCounter);
            ZEcefCm = decodeInt(byteBuffer, ref positionCounter);
            XVEcefCms = decodeInt(byteBuffer, ref positionCounter);
            YVEcefCms = decodeInt(byteBuffer, ref positionCounter);
            ZVEcefCms = decodeInt(byteBuffer, ref positionCounter);
            PressureMillibars = decodeFloat(byteBuffer, ref positionCounter);







            Timeouts = decodeInt(byteBuffer, ref positionCounter);
            UnrecognizedMsgTypes = decodeInt(byteBuffer, ref positionCounter);
            ChecksumErrors = decodeInt(byteBuffer, ref positionCounter);
            NumOfBlownFrames = decodeInt(byteBuffer, ref positionCounter);
            SerialCommunicationBufferOverruns = decodeInt(byteBuffer, ref positionCounter);
        }


        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);
         

            encode(ref rawMsg, XNEDLocalFrame, ref positionCounter);
            encode(ref rawMsg, XVelocityFRDCms, ref positionCounter);
            encode(ref rawMsg, PitchRads, ref positionCounter);
            
            encode(ref rawMsg, YNEDLocalFrame, ref positionCounter);
            encode(ref rawMsg, YVelocityFRDCms, ref positionCounter);
            encode(ref rawMsg, RollRads, ref positionCounter);
            

            
            encode(ref rawMsg, ZVelocityFRDCms, ref positionCounter);
            encode(ref rawMsg, YawRads, ref positionCounter);
            encode(ref rawMsg, ZNEDLocalFrame, ref positionCounter);


            encode(ref rawMsg,  XAccelFrdMss, ref positionCounter);
            encode(ref rawMsg,  YAccelFrdMss, ref positionCounter);
            encode(ref rawMsg,  ZAccelFrdMss, ref positionCounter);
            encode(ref rawMsg,  YawAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg,  PitchAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg,  RollAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg,  XMagFrd, ref positionCounter);
            encode(ref rawMsg,  YMagFrd, ref positionCounter);
            encode(ref rawMsg,  ZMagFrd, ref positionCounter);
            encode(ref rawMsg,  XEcefCm, ref positionCounter);
            encode(ref rawMsg,  YEcefCm, ref positionCounter);
            encode(ref rawMsg,  ZEcefCm, ref positionCounter);
            encode(ref rawMsg,  XVEcefCms, ref positionCounter);
            encode(ref rawMsg,  YVEcefCms, ref positionCounter);
            encode(ref rawMsg,  ZVEcefCms, ref positionCounter);
            encode(ref rawMsg,  PressureMillibars, ref positionCounter);









            encode(ref rawMsg, Timeouts, ref positionCounter);
            encode(ref rawMsg, UnrecognizedMsgTypes, ref positionCounter);
            encode(ref rawMsg, ChecksumErrors, ref positionCounter);
            encode(ref rawMsg, NumOfBlownFrames, ref positionCounter);
            encode(ref rawMsg, SerialCommunicationBufferOverruns, ref positionCounter);

            return rawMsg;
        }

    }
}

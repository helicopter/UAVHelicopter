using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Messages
{
    public class SensorDataMessage : Message
    {
        public const byte MessageType = 4;

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
           sizeof(int) +
           sizeof(int) +
           sizeof(int) +
           sizeof(int) +
           sizeof(int) +
           sizeof(int) +
           sizeof(float); 

        public SensorDataMessage()
            : base(MessageType, NumOfBytesInMsg)
        {
        }


        public static SensorDataMessage BuildMessageSt(byte[] byteBuffer)
        {
            SensorDataMessage data = new SensorDataMessage();
            data.BuildMessage(byteBuffer);

            return data;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);
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
        }

        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);
            encode(ref rawMsg, XAccelFrdMss, ref positionCounter);
            encode(ref rawMsg, YAccelFrdMss, ref positionCounter);
            encode(ref rawMsg, ZAccelFrdMss, ref positionCounter);
            encode(ref rawMsg, YawAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg, PitchAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg, RollAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg, XMagFrd, ref positionCounter);
            encode(ref rawMsg, YMagFrd, ref positionCounter);
            encode(ref rawMsg, ZMagFrd, ref positionCounter);
            encode(ref rawMsg, XEcefCm, ref positionCounter);
            encode(ref rawMsg, YEcefCm, ref positionCounter);
            encode(ref rawMsg, ZEcefCm, ref positionCounter);
            encode(ref rawMsg, XVEcefCms, ref positionCounter);
            encode(ref rawMsg, YVEcefCms, ref positionCounter);
            encode(ref rawMsg, ZVEcefCms, ref positionCounter);
            encode(ref rawMsg, PressureMillibars, ref positionCounter);

            return rawMsg;
        }
    }
}

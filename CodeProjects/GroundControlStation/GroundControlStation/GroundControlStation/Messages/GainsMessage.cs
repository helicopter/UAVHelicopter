using GroundControlStation.Model;
using System;
using System.Collections.Generic;
using System.Text;
using GroundControlStation.Util;

namespace GroundControlStation.Messages
{
    public class GainsMessage : Message
    {
        public const byte MessageType = 7;


        public float YawIntegralGain;
        public float YawDerivativeGain;
        public float YawProportionalGain;
        public float YawAntiWindupGain;

				
        public float XIntegralGain;
        public float XDerivativeGain;
        public float XProportionalGain;
        public float XAntiWindupGain;
        public float LongitudeInnerLoopGain;
        public float PitchAngularVelocityGain;				
				
				
        public float YIntegralGain;
        public float YDerivativeGain;
        public float YProportionalGain;
        public float YAntiWindupGain;
        public float LateralInnerLoopGain;
        public float RollAngularVelocityGain;				
				
        public float ZIntegralGain;
        public float ZDerivativeGain;
        public float ZProportionalGain;
        public float ZAntiWindupGain;

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
            sizeof(float) +
            sizeof(float);


        public GainsMessage()
            : base(MessageType, NumOfBytesInMsg)
        {
        }

        public static GainsMessage BuildMessageSt(byte[] byteBuffer)
        {
            GainsMessage fct = new GainsMessage();
            fct.BuildMessage(byteBuffer);

            return fct;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);

            YawIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            YawDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            YawProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            YawAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);


            XIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            XDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            XProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            XAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeInnerLoopGain = decodeFloat(byteBuffer, ref positionCounter);
            PitchAngularVelocityGain = decodeFloat(byteBuffer, ref positionCounter);

            YIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            YDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            YProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            YAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            LateralInnerLoopGain = decodeFloat(byteBuffer, ref positionCounter);
            RollAngularVelocityGain = decodeFloat(byteBuffer, ref positionCounter);


            ZIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            ZDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            ZProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            ZAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);

        }


        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);

            encode(ref rawMsg, YawIntegralGain, ref positionCounter);
            encode(ref rawMsg, YawDerivativeGain, ref positionCounter);
            encode(ref rawMsg, YawProportionalGain, ref positionCounter);
            encode(ref rawMsg, YawAntiWindupGain, ref positionCounter);

            encode(ref rawMsg, XIntegralGain, ref positionCounter);
            encode(ref rawMsg, XDerivativeGain, ref positionCounter);
            encode(ref rawMsg, XProportionalGain, ref positionCounter);
            encode(ref rawMsg, XAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, LongitudeInnerLoopGain, ref positionCounter);
            encode(ref rawMsg, PitchAngularVelocityGain, ref positionCounter);


            encode(ref rawMsg, YIntegralGain, ref positionCounter);
            encode(ref rawMsg, YDerivativeGain, ref positionCounter);
            encode(ref rawMsg, YProportionalGain, ref positionCounter);
            encode(ref rawMsg, YAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, LateralInnerLoopGain, ref positionCounter);
            encode(ref rawMsg, RollAngularVelocityGain, ref positionCounter);


            encode(ref rawMsg, ZIntegralGain, ref positionCounter);
            encode(ref rawMsg, ZDerivativeGain, ref positionCounter);
            encode(ref rawMsg, ZProportionalGain, ref positionCounter);
            encode(ref rawMsg, ZAntiWindupGain, ref positionCounter);


            return rawMsg;
        }
    }
}

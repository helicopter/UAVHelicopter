using GroundControlStation.Model;
using System;
using System.Collections.Generic;
using System.Text;

namespace GroundControlStation.Messages
{
    public class FlightComputerTelemetryMessage : Message
    {
        public const byte MessageType = 2;

        public uint MagYaw;

        public int YawVelocityDegreesPerSecond;


        public int YawIntegral;

        public int YawProportional;

        public int YawDerivativeError;

        public int YawControl;


        public int YawIntegralGain;

        public int YawDerivativeGain;

        public int YawProportionalGain;

        public int YawAntiWindupGain;







        public int XNEDBodyFrame;
        public int XVelocityMetersPerSecond;
        public int ThetaPitchDegrees;

        public int XIntegral;
        public int XProportional;
        public int XDerivativeError;
        public int LongitudeControl;
        public int XIntegralGain;
        public int XDerivativeGain;
        public int XProportionalGain;
        public int XAntiWindupGain;
        public int LongitudeInnerLoopGain;

        public int YNEDBodyFrame;
        public int YVelocityMetersPerSecond;
        public int PhiRollDegrees;
        public int YIntegral;
        public int YProportional;
        public int YDerivativeError;
        public int LateralControl;
        public int YIntegralGain;
        public int YDerivativeGain;
        public int YProportionalGain;
        public int YAntiWindupGain;
        public int LateralInnerLoopGain;



        public int AltitudeFeet;
        public int ZVelocityFeetPerSecond;
        public int ZIntegral;
        public int ZProportional;
        public int ZDerivativeError;
        public int MainRotorCollectiveControl;
        public int ZIntegralGain;
        public int ZDerivativeGain;
        public int ZProportionalGain;
        public int ZAntiWindupGain;
																		
				




















        public int Timeouts;

        public int UnrecognizedMsgTypes;

        public int ChecksumErrors;

        public int NumOfBlownFrames;

        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        public const int NumOfBytesInMsg =
            sizeof(byte) +
            sizeof(uint) +
            sizeof(int) +

            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +





            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +

            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +

            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +






            sizeof(int) +
            sizeof(int);


        public FlightComputerTelemetryMessage()
            : base(MessageType, NumOfBytesInMsg)
        {
        }

        public static FlightComputerTelemetryMessage BuildMessageSt(byte[] byteBuffer)
        {
            FlightComputerTelemetryMessage fct = new FlightComputerTelemetryMessage();
            fct.BuildMessage(byteBuffer);

            return fct;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);

            MagYaw = decodeUInt(byteBuffer, ref positionCounter);
            YawVelocityDegreesPerSecond = decodeInt(byteBuffer, ref positionCounter);
            YawIntegral = decodeInt(byteBuffer, ref positionCounter);
            YawProportional = decodeInt(byteBuffer, ref positionCounter);
            YawDerivativeError = decodeInt(byteBuffer, ref positionCounter);
            YawControl = decodeInt(byteBuffer, ref positionCounter);
            YawIntegralGain = decodeInt(byteBuffer, ref positionCounter);
            YawDerivativeGain = decodeInt(byteBuffer, ref positionCounter);
            YawProportionalGain = decodeInt(byteBuffer, ref positionCounter);
            YawAntiWindupGain = decodeInt(byteBuffer, ref positionCounter);




            XNEDBodyFrame = decodeInt(byteBuffer, ref positionCounter);
            XVelocityMetersPerSecond = decodeInt(byteBuffer, ref positionCounter);
            ThetaPitchDegrees = decodeInt(byteBuffer, ref positionCounter);
            XIntegral = decodeInt(byteBuffer, ref positionCounter);
            XProportional = decodeInt(byteBuffer, ref positionCounter);
            XDerivativeError = decodeInt(byteBuffer, ref positionCounter);
            LongitudeControl = decodeInt(byteBuffer, ref positionCounter);
            XIntegralGain = decodeInt(byteBuffer, ref positionCounter);
            XDerivativeGain = decodeInt(byteBuffer, ref positionCounter);
            XProportionalGain = decodeInt(byteBuffer, ref positionCounter);
            XAntiWindupGain = decodeInt(byteBuffer, ref positionCounter);
            LongitudeInnerLoopGain = decodeInt(byteBuffer, ref positionCounter);



            YNEDBodyFrame = decodeInt(byteBuffer, ref positionCounter);
            YVelocityMetersPerSecond = decodeInt(byteBuffer, ref positionCounter);
            PhiRollDegrees = decodeInt(byteBuffer, ref positionCounter);
            YIntegral = decodeInt(byteBuffer, ref positionCounter);
            YProportional = decodeInt(byteBuffer, ref positionCounter);
            YDerivativeError = decodeInt(byteBuffer, ref positionCounter);
            LateralControl = decodeInt(byteBuffer, ref positionCounter);
            YIntegralGain = decodeInt(byteBuffer, ref positionCounter);
            YDerivativeGain = decodeInt(byteBuffer, ref positionCounter);
            YProportionalGain = decodeInt(byteBuffer, ref positionCounter);
            YAntiWindupGain = decodeInt(byteBuffer, ref positionCounter);
            LateralInnerLoopGain = decodeInt(byteBuffer, ref positionCounter);



            AltitudeFeet = decodeInt(byteBuffer, ref positionCounter);
            ZVelocityFeetPerSecond = decodeInt(byteBuffer, ref positionCounter);
            ZIntegral = decodeInt(byteBuffer, ref positionCounter);
            ZProportional = decodeInt(byteBuffer, ref positionCounter);
            ZDerivativeError = decodeInt(byteBuffer, ref positionCounter);
            MainRotorCollectiveControl = decodeInt(byteBuffer, ref positionCounter);
            ZIntegralGain = decodeInt(byteBuffer, ref positionCounter);
            ZDerivativeGain = decodeInt(byteBuffer, ref positionCounter);
            ZProportionalGain = decodeInt(byteBuffer, ref positionCounter);
            ZAntiWindupGain = decodeInt(byteBuffer, ref positionCounter);






            Timeouts = decodeInt(byteBuffer, ref positionCounter);
            UnrecognizedMsgTypes = decodeInt(byteBuffer, ref positionCounter);
            ChecksumErrors = decodeInt(byteBuffer, ref positionCounter);
            NumOfBlownFrames = decodeInt(byteBuffer, ref positionCounter);
        }


        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);
            encode(ref rawMsg, MagYaw, ref positionCounter);
            encode(ref rawMsg, YawVelocityDegreesPerSecond, ref positionCounter);
            encode(ref rawMsg, YawIntegral, ref positionCounter);
            encode(ref rawMsg, YawProportional, ref positionCounter);
            encode(ref rawMsg, YawDerivativeError, ref positionCounter);
            encode(ref rawMsg, YawControl, ref positionCounter);
            encode(ref rawMsg, YawIntegralGain, ref positionCounter);
            encode(ref rawMsg, YawDerivativeGain, ref positionCounter);
            encode(ref rawMsg, YawProportionalGain, ref positionCounter);
            encode(ref rawMsg, YawAntiWindupGain, ref positionCounter);



            encode(ref rawMsg, XNEDBodyFrame, ref positionCounter);
            encode(ref rawMsg, XVelocityMetersPerSecond, ref positionCounter);
            encode(ref rawMsg, ThetaPitchDegrees, ref positionCounter);
            encode(ref rawMsg, XIntegral, ref positionCounter);
            encode(ref rawMsg, XProportional, ref positionCounter);
            encode(ref rawMsg, XDerivativeError, ref positionCounter);
            encode(ref rawMsg, LongitudeControl, ref positionCounter);
            encode(ref rawMsg, XIntegralGain, ref positionCounter);
            encode(ref rawMsg, XDerivativeGain, ref positionCounter);
            encode(ref rawMsg, XProportionalGain, ref positionCounter);
            encode(ref rawMsg, XAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, LongitudeInnerLoopGain, ref positionCounter);

            encode(ref rawMsg, YNEDBodyFrame, ref positionCounter);
            encode(ref rawMsg, YVelocityMetersPerSecond, ref positionCounter);
            encode(ref rawMsg, PhiRollDegrees, ref positionCounter);
            encode(ref rawMsg, YIntegral, ref positionCounter);
            encode(ref rawMsg, YProportional, ref positionCounter);
            encode(ref rawMsg, YDerivativeError, ref positionCounter);
            encode(ref rawMsg, LateralControl, ref positionCounter);
            encode(ref rawMsg, YIntegralGain, ref positionCounter);
            encode(ref rawMsg, YDerivativeGain, ref positionCounter);
            encode(ref rawMsg, YProportionalGain, ref positionCounter);
            encode(ref rawMsg, YAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, LateralInnerLoopGain, ref positionCounter);




            encode(ref rawMsg, AltitudeFeet, ref positionCounter);
            encode(ref rawMsg, ZVelocityFeetPerSecond, ref positionCounter);
            encode(ref rawMsg, ZIntegral, ref positionCounter);
            encode(ref rawMsg, ZProportional, ref positionCounter);
            encode(ref rawMsg, ZDerivativeError, ref positionCounter);
            encode(ref rawMsg, MainRotorCollectiveControl, ref positionCounter);
            encode(ref rawMsg, ZIntegralGain, ref positionCounter);
            encode(ref rawMsg, ZDerivativeGain, ref positionCounter);
            encode(ref rawMsg, ZProportionalGain, ref positionCounter);
            encode(ref rawMsg, ZAntiWindupGain, ref positionCounter);




            encode(ref rawMsg, Timeouts, ref positionCounter);
            encode(ref rawMsg, UnrecognizedMsgTypes, ref positionCounter);
            encode(ref rawMsg, ChecksumErrors, ref positionCounter);
            encode(ref rawMsg, NumOfBlownFrames, ref positionCounter);

            return rawMsg;
        }


        private uint decodeUInt(byte[] byteBuffer, ref int positionCounter)
        {
            uint val = (uint) BitConverter.ToInt32(byteBuffer, positionCounter);
            positionCounter += sizeof(uint);
            return val;
        }

        private int decodeInt(byte[] byteBuffer, ref int positionCounter)
        {
            int val = BitConverter.ToInt32(byteBuffer, positionCounter);
            positionCounter += sizeof(int);
            return val;
        }

        private byte decodeByte(byte[] byteBuffer, ref int positionCounter)
        {
            byte val = byteBuffer[positionCounter];
            positionCounter += sizeof(byte);
            return val;
        }

        private void encode(ref byte[] rawMsg, byte data, ref int positionCounter)
        {
            rawMsg[positionCounter++] = data;
        }

        private void encode(ref byte[] rawMsg, int data, ref int positionCounter)
        {
            byte[] temp = BitConverter.GetBytes(data);

            rawMsg[positionCounter++] = temp[0];
            rawMsg[positionCounter++] = temp[1];
            rawMsg[positionCounter++] = temp[2];
            rawMsg[positionCounter++] = temp[3];
        }


        private void encode(ref byte[] rawMsg, uint data, ref int positionCounter)
        {
            encode(ref rawMsg, (int)data, ref positionCounter);
        }

        public void UpdateModel(GroundControlStationModel model)
        {
            model.MagYaw = (float) MagYaw / 100;
            model.YawVelocityDegreesPerSecond = (float) YawVelocityDegreesPerSecond / 100;
            model.YawIntegral = (float)YawIntegral / 100;
            model.YawProportional = (float)YawProportional / 100;
            model.YawDerivativeError = (float)YawDerivativeError / 100;



            model.XNEDBodyFrame = (float)XNEDBodyFrame / 100;
            model.XVelocityMetersPerSecond = (float)XVelocityMetersPerSecond / 100;
            model.ThetaPitchDegrees = (float)ThetaPitchDegrees / 100;
            model.XIntegral = (float)XIntegral / 100;
            model.XProportional = (float)XProportional / 100;
            model.XDerivativeError = (float)XDerivativeError / 100;
            model.LongitudeControl = (float)LongitudeControl / 100;
            model.XIntegralGain = (float)XIntegralGain / 100;
            model.XDerivativeGain = (float)XDerivativeGain / 100;
            model.XProportionalGain = (float)XProportionalGain / 100;
            model.XAntiWindupGain = (float)XAntiWindupGain / 100;
            model.LongitudeInnerLoopGain = (float)LongitudeInnerLoopGain / 100;

            model.YNEDBodyFrame = (float)YNEDBodyFrame / 100;
            model.YVelocityMetersPerSecond = (float)YVelocityMetersPerSecond / 100;
            model.PhiRollDegrees = (float)PhiRollDegrees / 100;
            model.YIntegral = (float)YIntegral / 100;
            model.YProportional = (float)YProportional / 100;
            model.YDerivativeError = (float)YDerivativeError / 100;
            model.LateralControl = (float)LateralControl / 100;
            model.YIntegralGain = (float)YIntegralGain / 100;
            model.YDerivativeGain = (float)YDerivativeGain / 100;
            model.YProportionalGain = (float)YProportionalGain / 100;
            model.YAntiWindupGain = (float)YAntiWindupGain / 100;
            model.LateralInnerLoopGain = (float)LateralInnerLoopGain / 100;



            model.AltitudeFeet = (float)AltitudeFeet / 100;
            model.ZVelocityFeetPerSecond = (float)ZVelocityFeetPerSecond / 100;
            model.ZIntegral = (float)ZIntegral / 100;
            model.ZProportional = (float)ZProportional / 100;
            model.ZDerivativeError = (float)ZDerivativeError / 100;
            model.MainRotorCollectiveControl = (float)MainRotorCollectiveControl / 100;
            model.ZIntegralGain = (float)ZIntegralGain / 100;
            model.ZDerivativeGain = (float)ZDerivativeGain / 100;
            model.ZProportionalGain = (float)ZProportionalGain / 100;
            model.ZAntiWindupGain = (float)ZAntiWindupGain / 100;




            /**
             * Rescale yaw control to appropriate values. 
             * from -1/1 to -10/10
             * new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
             */
            float scaledYaw = (10 - -10) / (1 - -1) * (((float) YawControl / 100) - -1) + -10;

            //model.YawControl = (float)YawControl / 100;
            model.YawControl = scaledYaw;














            model.Timeouts = Timeouts;
            model.UnrecognizedMsgTypes = UnrecognizedMsgTypes;
            model.ChecksumErrors = ChecksumErrors;
            model.NumOfBlownFrames = NumOfBlownFrames;
        }

        public static FlightComputerTelemetryMessage CreateFromModel(GroundControlStationModel model)
        {
            FlightComputerTelemetryMessage msg = new FlightComputerTelemetryMessage();
            msg.MagYaw = (uint)(model.SimTelm.MagHeadingDegrees * 100);
            msg.YawVelocityDegreesPerSecond = (int)(ConvertToDegPerSec(model.SimTelm.YawVelocityRadsPerS) * 100);

            msg.YawIntegralGain = (int)(model.YawIntegralGain * 100);
            msg.YawDerivativeGain = (int)(model.YawDerivativeGain * 100);
            msg.YawProportionalGain = (int)(model.YawProportionalGain * 100);
            msg.YawAntiWindupGain = (int)(model.YawAntiWindupGain * 100);

            return msg;
        }

        private static double ConvertToDegPerSec(float radsPerSecValue)
        {
            return radsPerSecValue * (180.0 / Math.PI);
        }
    }
}

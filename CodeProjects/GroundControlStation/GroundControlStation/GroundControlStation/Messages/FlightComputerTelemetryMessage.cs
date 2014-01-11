using GroundControlStation.Model;
using System;
using System.Collections.Generic;
using System.Text;

namespace GroundControlStation.Messages
{
    public class FlightComputerTelemetryMessage : Message
    {
        public const byte MessageType = 2;

        public float MagYaw;

        public float YawVelocityDegreesPerSecond;


        public float YawIntegral;

        public float YawProportional;

        public float YawDerivativeError;

        public float YawControl;


        public float YawIntegralGain;

        public float YawDerivativeGain;

        public float YawProportionalGain;

        public float YawAntiWindupGain;







        public float XNEDBodyFrame;
        public float XVelocityMetersPerSecond;
        public float ThetaPitchDegrees;

        public float XIntegral;
        public float XProportional;
        public float XDerivativeError;
        public float LongitudeControl;
        public float XIntegralGain;
        public float XDerivativeGain;
        public float XProportionalGain;
        public float XAntiWindupGain;
        public float LongitudeInnerLoopGain;
        public float XLongitudeOuterLoopSetpoint;
        public float PitchAngularVelocityRadsPerSecond;
        public float PitchAngularVelocityGain;

        public float YNEDBodyFrame;
        public float YVelocityMetersPerSecond;
        public float PhiRollDegrees;
        public float YIntegral;
        public float YProportional;
        public float YDerivativeError;
        public float LateralControl;
        public float YIntegralGain;
        public float YDerivativeGain;
        public float YProportionalGain;
        public float YAntiWindupGain;
        public float LateralInnerLoopGain;
        public float YLateralOuterLoopSetpoint;
        public float RollAngularVelocityRadsPerSecond;
        public float RollAngularVelocityGain;

        public float AltitudeFeetAgl;
        public float ZVelocityMetersPerSecond;
        public float ZIntegral;
        public float ZProportional;
        public float ZDerivativeError;
        public float MainRotorCollectiveControl;
        public float ZIntegralGain;
        public float ZDerivativeGain;
        public float ZProportionalGain;
        public float ZAntiWindupGain;
        public float ZNEDBodyFrame;

        public float LatitudeDegrees;
        public float LongitudeDegrees;


        public int Timeouts;

        public int UnrecognizedMsgTypes;

        public int ChecksumErrors;

        public int NumOfBlownFrames;

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

            MagYaw = decodeFloat(byteBuffer, ref positionCounter);
            YawVelocityDegreesPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            YawIntegral = decodeFloat(byteBuffer, ref positionCounter);
            YawProportional = decodeFloat(byteBuffer, ref positionCounter);
            YawDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            YawControl = decodeFloat(byteBuffer, ref positionCounter);
            YawIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            YawDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            YawProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            YawAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);




            XNEDBodyFrame = decodeFloat(byteBuffer, ref positionCounter);
            XVelocityMetersPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            ThetaPitchDegrees = decodeFloat(byteBuffer, ref positionCounter);
            XIntegral = decodeFloat(byteBuffer, ref positionCounter);
            XProportional = decodeFloat(byteBuffer, ref positionCounter);
            XDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeControl = decodeFloat(byteBuffer, ref positionCounter);
            XIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            XDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            XProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            XAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeInnerLoopGain = decodeFloat(byteBuffer, ref positionCounter);
            XLongitudeOuterLoopSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            PitchAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            PitchAngularVelocityGain = decodeFloat(byteBuffer, ref positionCounter);

            YNEDBodyFrame = decodeFloat(byteBuffer, ref positionCounter);
            YVelocityMetersPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            PhiRollDegrees = decodeFloat(byteBuffer, ref positionCounter);
            YIntegral = decodeFloat(byteBuffer, ref positionCounter);
            YProportional = decodeFloat(byteBuffer, ref positionCounter);
            YDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            LateralControl = decodeFloat(byteBuffer, ref positionCounter);
            YIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            YDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            YProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            YAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            LateralInnerLoopGain = decodeFloat(byteBuffer, ref positionCounter);
            YLateralOuterLoopSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            RollAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            RollAngularVelocityGain = decodeFloat(byteBuffer, ref positionCounter);

            AltitudeFeetAgl = decodeFloat(byteBuffer, ref positionCounter);
            ZVelocityMetersPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            ZIntegral = decodeFloat(byteBuffer, ref positionCounter);
            ZProportional = decodeFloat(byteBuffer, ref positionCounter);
            ZDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            MainRotorCollectiveControl = decodeFloat(byteBuffer, ref positionCounter);
            ZIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            ZDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            ZProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            ZAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            ZNEDBodyFrame = decodeFloat(byteBuffer, ref positionCounter);

            LatitudeDegrees = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeDegrees = decodeFloat(byteBuffer, ref positionCounter);




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
            encode(ref rawMsg, XLongitudeOuterLoopSetpoint, ref positionCounter);
            encode(ref rawMsg, PitchAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg, PitchAngularVelocityGain, ref positionCounter);

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
            encode(ref rawMsg, YLateralOuterLoopSetpoint, ref positionCounter);
            encode(ref rawMsg, RollAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg, RollAngularVelocityGain, ref positionCounter);


            encode(ref rawMsg, AltitudeFeetAgl, ref positionCounter);
            encode(ref rawMsg, ZVelocityMetersPerSecond, ref positionCounter);
            encode(ref rawMsg, ZIntegral, ref positionCounter);
            encode(ref rawMsg, ZProportional, ref positionCounter);
            encode(ref rawMsg, ZDerivativeError, ref positionCounter);
            encode(ref rawMsg, MainRotorCollectiveControl, ref positionCounter);
            encode(ref rawMsg, ZIntegralGain, ref positionCounter);
            encode(ref rawMsg, ZDerivativeGain, ref positionCounter);
            encode(ref rawMsg, ZProportionalGain, ref positionCounter);
            encode(ref rawMsg, ZAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, ZNEDBodyFrame, ref positionCounter);

            encode(ref rawMsg, LatitudeDegrees, ref positionCounter);
            encode(ref rawMsg, LongitudeDegrees, ref positionCounter);


            encode(ref rawMsg, Timeouts, ref positionCounter);
            encode(ref rawMsg, UnrecognizedMsgTypes, ref positionCounter);
            encode(ref rawMsg, ChecksumErrors, ref positionCounter);
            encode(ref rawMsg, NumOfBlownFrames, ref positionCounter);

            return rawMsg;
        }

        private float decodeFloat(byte[] byteBuffer, ref int positionCounter)
        {
            float val = BitConverter.ToSingle(byteBuffer, positionCounter);
            positionCounter += sizeof(float);
            return val;
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

        private void encode(ref byte[] rawMsg, float data, ref int positionCounter)
        {
            byte[] temp = BitConverter.GetBytes(data);

            rawMsg[positionCounter++] = temp[0];
            rawMsg[positionCounter++] = temp[1];
            rawMsg[positionCounter++] = temp[2];
            rawMsg[positionCounter++] = temp[3];
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
            model.MagYaw =  MagYaw;
            model.YawVelocityDegreesPerSecond =  YawVelocityDegreesPerSecond;
            model.YawIntegral = YawIntegral;
            model.YawProportional = YawProportional;
            model.YawDerivativeError = YawDerivativeError;



            model.XNEDBodyFrame = XNEDBodyFrame;
            model.XVelocityMetersPerSecond = XVelocityMetersPerSecond;
            model.ThetaPitchDegrees = ThetaPitchDegrees;
            model.XIntegral = XIntegral;
            model.XProportional = XProportional;
            model.XDerivativeError = XDerivativeError;
            model.LongitudeControl = LongitudeControl;
            model.XLongitudeOuterLoopSetpoint = XLongitudeOuterLoopSetpoint;
            model.PitchAngularVelocityRadsPerSecond = PitchAngularVelocityRadsPerSecond;
            //model.XIntegralGain = XIntegralGain;
            //model.XDerivativeGain = XDerivativeGain;
            //model.XProportionalGain = XProportionalGain;
            //model.XAntiWindupGain = XAntiWindupGain;
            //model.LongitudeInnerLoopGain = LongitudeInnerLoopGain;

            model.YNEDBodyFrame = YNEDBodyFrame;
            model.YVelocityMetersPerSecond = YVelocityMetersPerSecond;
            model.PhiRollDegrees = PhiRollDegrees;
            model.YIntegral = YIntegral;
            model.YProportional = YProportional;
            model.YDerivativeError = YDerivativeError;
            model.LateralControl = LateralControl;
            model.YLateralOuterLoopSetpoint = YLateralOuterLoopSetpoint;
            model.RollAngularVelocityRadsPerSecond = RollAngularVelocityRadsPerSecond;
            //model.YIntegralGain = YIntegralGain;
            //model.YDerivativeGain = YDerivativeGain;
            //model.YProportionalGain = YProportionalGain;
            //model.YAntiWindupGain = YAntiWindupGain;
            //model.LateralInnerLoopGain = LateralInnerLoopGain;



            model.AltitudeFeetAgl = AltitudeFeetAgl;
            model.ZVelocityFeetPerSecond = ZVelocityMetersPerSecond;
            model.ZIntegral = ZIntegral;
            model.ZProportional = ZProportional;
            model.ZDerivativeError = ZDerivativeError;
            model.MainRotorCollectiveControl = MainRotorCollectiveControl;
            model.ZNEDBodyFrame = ZNEDBodyFrame;
            //model.ZIntegralGain = ZIntegralGain;
            //model.ZDerivativeGain = ZDerivativeGain;
            //model.ZProportionalGain = ZProportionalGain;
            //model.ZAntiWindupGain = ZAntiWindupGain;

            model.YawControl = YawControl;


            model.Timeouts = Timeouts;
            model.UnrecognizedMsgTypes = UnrecognizedMsgTypes;
            model.ChecksumErrors = ChecksumErrors;
            model.NumOfBlownFrames = NumOfBlownFrames;
        }


        public static FlightComputerTelemetryMessage CreateFromModel(GroundControlStationModel model)
        {
            FlightComputerTelemetryMessage msg = new FlightComputerTelemetryMessage();
            msg.MagYaw = model.SimTelm.MagHeadingDegrees;
            msg.YawVelocityDegreesPerSecond = (float) ConvertToDegPerSec(model.SimTelm.YawVelocityRadsPerS);

            msg.YawIntegralGain = model.YawIntegralGain;
            msg.YawDerivativeGain = model.YawDerivativeGain;
            msg.YawProportionalGain = model.YawProportionalGain;
            msg.YawAntiWindupGain = model.YawAntiWindupGain;

            msg.XIntegralGain = model.XIntegralGain;
            msg.XDerivativeGain = model.XDerivativeGain;
            msg.XProportionalGain = model.XProportionalGain;
            msg.XAntiWindupGain = model.XAntiWindupGain;
            msg.LongitudeInnerLoopGain = model.LongitudeInnerLoopGain;
            msg.PitchAngularVelocityRadsPerSecond = model.PitchAngularVelocityRadsPerSecond;
            msg.PitchAngularVelocityGain = model.PitchAngularVelocityGain;

            msg.YIntegralGain = model.YIntegralGain;
            msg.YDerivativeGain = model.YDerivativeGain;
            msg.YProportionalGain = model.YProportionalGain;
            msg.YAntiWindupGain = model.YAntiWindupGain;
            msg.LateralInnerLoopGain = model.LateralInnerLoopGain;
            msg.RollAngularVelocityRadsPerSecond = model.RollAngularVelocityRadsPerSecond;
            msg.RollAngularVelocityGain = model.RollAngularVelocityGain;

            msg.ZIntegralGain = model.ZIntegralGain;
            msg.ZDerivativeGain = model.ZDerivativeGain;
            msg.ZProportionalGain = model.ZProportionalGain;
            msg.ZAntiWindupGain = model.ZAntiWindupGain;

            msg.LatitudeDegrees = model.SimTelm.LatitudeDegrees;
            msg.LongitudeDegrees = model.SimTelm.LongitudeDegrees;
            msg.AltitudeFeetAgl = model.SimTelm.ZAltitudeFtAgl;

            msg.XVelocityMetersPerSecond = model.SimTelm.XVelocityNEDFrameMs;
            msg.YVelocityMetersPerSecond = model.SimTelm.YVelocityNEDFrameMs;
            msg.ZVelocityMetersPerSecond = model.SimTelm.ZVelocityNEDFrameMs;

            msg.ThetaPitchDegrees = model.SimTelm.PitchDegrees;
            msg.PhiRollDegrees = model.SimTelm.RollDegrees;

            return msg;
        }

        private static double ConvertToDegPerSec(float radsPerSecValue)
        {
            return radsPerSecValue * (180.0 / Math.PI);
        }
    }
}

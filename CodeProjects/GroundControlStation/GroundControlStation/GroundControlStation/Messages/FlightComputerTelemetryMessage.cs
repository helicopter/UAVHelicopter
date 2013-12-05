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

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

        //public float YawControl;
        public int YawControl;

        public int Timeouts;

        public int UnrecognizedMsgTypes;

        public int ChecksumErrors;

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

            //YawControl = decodeFloat(byteBuffer, ref positionCounter);
            YawControl = decodeInt(byteBuffer, ref positionCounter);

            Timeouts = decodeInt(byteBuffer, ref positionCounter);
            UnrecognizedMsgTypes = decodeInt(byteBuffer, ref positionCounter);
            ChecksumErrors = decodeInt(byteBuffer, ref positionCounter);
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
            encode(ref rawMsg, Timeouts, ref positionCounter);
            encode(ref rawMsg, UnrecognizedMsgTypes, ref positionCounter);
            encode(ref rawMsg, ChecksumErrors, ref positionCounter);

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

        public void UpdateModel(GroundControlStationModel Model)
        {
            Model.MagYaw = (float) MagYaw / 100;
            Model.YawVelocityDegreesPerSecond = (float) YawVelocityDegreesPerSecond / 100;
            Model.YawIntegral = (float)YawIntegral / 100;
            Model.YawProportional = (float)YawProportional / 100;
            Model.YawDerivativeError = (float)YawDerivativeError / 100;
            Model.YawControl = (float)YawControl / 100;
            Model.Timeouts = Timeouts;
            Model.UnrecognizedMsgTypes = UnrecognizedMsgTypes;
            Model.ChecksumErrors = ChecksumErrors;
        }

        public static FlightComputerTelemetryMessage CreateFromModel(GroundControlStationModel Model)
        {
            FlightComputerTelemetryMessage msg = new FlightComputerTelemetryMessage();
            msg.MagYaw = (uint)(Model.SimTelm.MagHeadingDegrees * 100);
            msg.YawVelocityDegreesPerSecond = (int)(Model.SimTelm.YawVelocityMs * 100);

            return msg;
        }
    }
}

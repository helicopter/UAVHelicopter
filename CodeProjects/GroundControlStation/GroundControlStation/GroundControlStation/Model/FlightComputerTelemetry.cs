using System;
using System.Collections.Generic;
using System.Text;

namespace GroundControlStation.Model
{
    public class FlightComputerTelemetry : Message
    {
        public const byte MessageType = 2;

        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        public const int NumOfBytesInMsg = 15;

        public short MagX { get; set; }

        public short MagY { get; set; }

        public short MagZ { get; set; }


        public short MagYaw { get; set; }


        public short Timeouts { get; set; }

        public short UnrecognizedMsgTypes { get; set; }

        public short ChecksumErrors { get; set; }

        public FlightComputerTelemetry()
            : base(MessageType, NumOfBytesInMsg)
        {
        }

        public static FlightComputerTelemetry BuildMessageSt(byte[] byteBuffer)
        {
            FlightComputerTelemetry fct = new FlightComputerTelemetry();
            fct.BuildMessage(byteBuffer);

            return fct;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);
            MagX = decodeShort(byteBuffer, ref positionCounter);
            MagY = decodeShort(byteBuffer, ref positionCounter);
            MagZ = decodeShort(byteBuffer, ref positionCounter);

            MagYaw = decodeShort(byteBuffer, ref positionCounter);

            Timeouts = decodeShort(byteBuffer, ref positionCounter);
            UnrecognizedMsgTypes = decodeShort(byteBuffer, ref positionCounter);
            ChecksumErrors = decodeShort(byteBuffer, ref positionCounter);
        }


        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);
            encode(ref rawMsg, MagX, ref positionCounter);
            encode(ref rawMsg, MagY, ref positionCounter);
            encode(ref rawMsg, MagZ, ref positionCounter);
            encode(ref rawMsg, MagYaw, ref positionCounter);
            encode(ref rawMsg, Timeouts, ref positionCounter);
            encode(ref rawMsg, UnrecognizedMsgTypes, ref positionCounter);
            encode(ref rawMsg, ChecksumErrors, ref positionCounter);

            return rawMsg;
        }

        public System.Collections.Generic.IEnumerable<Tuple<string, string>> ListValues()
        {
            List<Tuple<string,string>> lstValues = new List<Tuple<string, string>>();

            lstValues.Add(new Tuple<string, string>("FC Mag X", MagX.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Mag Y", MagY.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Mag Z", MagZ.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Mag Yaw", MagYaw.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Timeouts", Timeouts.ToString()));
            lstValues.Add(new Tuple<string, string>("FC UnrecognizedMsgTypes", UnrecognizedMsgTypes.ToString()));
            lstValues.Add(new Tuple<string, string>("FC ChecksumErrors", ChecksumErrors.ToString()));


            return lstValues;
        }


        private short decodeShort(byte[] byteBuffer, ref int positionCounter)
        {
            short val = BitConverter.ToInt16(byteBuffer, positionCounter);
            positionCounter += sizeof(short);
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

        private void encode(ref byte[] rawMsg, short data, ref int positionCounter)
        {
            byte[] temp = BitConverter.GetBytes(data);

            rawMsg[positionCounter++] = temp[0];
            rawMsg[positionCounter++] = temp[1];
        }
    }
}

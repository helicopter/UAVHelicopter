using System;
using System.Collections.Generic;

namespace GroundControlStation.Model
{
    public class FlightControllerTelemetryData
    {
        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        private const int NumOfBytesInMsg = 7;

        public byte MsgType { get; set; }

        public short MagX { get; set; }

        public short MagY { get; set; }

        public short MagZ { get; set; }

        public static int GetNumOfBytesInMsg()
        {
            return NumOfBytesInMsg;
        }


        internal void BuildMessage(byte[] byteBuffer)
        {
            this.MsgType = byteBuffer[0];
            this.MagX = BitConverter.ToInt16(byteBuffer, 1);
            this.MagY = BitConverter.ToInt16(byteBuffer, 3);
            this.MagZ = BitConverter.ToInt16(byteBuffer, 5);
        }

        internal byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            byte[] temp = null;

            temp = BitConverter.GetBytes(MagX);

            rawMsg[0] = MsgType;
            rawMsg[1] = temp[0];
            rawMsg[2] = temp[1];

            temp = BitConverter.GetBytes(MagY);
            rawMsg[3] = temp[0];
            rawMsg[4] = temp[1];

            temp = BitConverter.GetBytes(MagX);
            rawMsg[5] = temp[0];
            rawMsg[6] = temp[0];

            return rawMsg;
        }

        internal System.Collections.Generic.IEnumerable<Tuple<string, string>> ListValues()
        {
            List<Tuple<string,string>> lstValues = new List<Tuple<string, string>>();

            lstValues.Add(new Tuple<string, string>("FC Mag X", MagX.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Mag Y", MagY.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Mag Z", MagZ.ToString()));

            return lstValues;
        }
    }
}

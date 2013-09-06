using System;

namespace GroundControlStation.Model
{
    public class FlightControllerTelemetryData
    {
        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        private const int NumOfBytesInMsg = 3;

        public byte MsgType { get; set; }

        public short HeadingDegrees { get; set; }

        public static int GetNumOfBytesInMsg()
        {
            return NumOfBytesInMsg;
        }


        internal void BuildMessage(byte[] byteBuffer)
        {
            this.MsgType = byteBuffer[0];
            this.HeadingDegrees = BitConverter.ToInt16(byteBuffer, 1);
        }

        internal byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            byte[] temp = null;

            temp = BitConverter.GetBytes(HeadingDegrees);

            rawMsg[0] = MsgType;
            rawMsg[1] = temp[0];
            rawMsg[2] = temp[1];

            return rawMsg;
        }
    }
}

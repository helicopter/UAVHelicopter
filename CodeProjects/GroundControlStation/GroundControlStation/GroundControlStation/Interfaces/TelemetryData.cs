using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Interfaces
{
    public class TelemetryData
    {
        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        private const int NumOfBytesInMsg = 3;

        public byte MsgType { get; set; }

        public short Yaw { get; set; }

        public static int GetNumOfBytesInMsg()
        {
            return NumOfBytesInMsg;
        }


        internal void BuildMessage(byte[] byteBuffer)
        {
            this.MsgType = byteBuffer[0];
            this.Yaw = BitConverter.ToInt16(byteBuffer, 1);
        }

        internal byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            byte[] temp = null;

            temp = BitConverter.GetBytes(Yaw);

            rawMsg[0] = MsgType;
            rawMsg[1] = temp[0];
            rawMsg[2] = temp[1];

            return rawMsg;
        }
    }
}

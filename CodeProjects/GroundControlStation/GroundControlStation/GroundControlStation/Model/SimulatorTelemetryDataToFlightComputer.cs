using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Model
{
    public class SimulatorTelemetryDataToFlightComputer
    {
        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        private const int NumOfBytesInMsg = 7;

        public byte MsgType { get; set; }

        public float MagX { get; set; }

        public float MagY { get; set; }

        public float MagZ { get; set; }

        public SimulatorTelemetryDataToFlightComputer()
        {
            MsgType = 1;
        }

        public static int GetNumOfBytesInMsg()
        {
            return NumOfBytesInMsg;
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

            temp = BitConverter.GetBytes(MagZ);
            rawMsg[5] = temp[0];
            rawMsg[6] = temp[1];

            return rawMsg;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Interfaces;

namespace UnitTests.mocks
{
    public class MockSerialPortInterface : SerialPortInterface
    {
        public List<byte> bytes;
 
        public List<byte> Bytes { get { return bytes; } set { bytes = value; } }

        private int receiveCounter = 0;

        private int transmitCounter = 0;

        public MockSerialPortInterface() : base(null)
        {
            bytes = new List<byte>();
        }

        public override bool IsOpen
        {
            get
            {
                return true;
            }
        }

        public override byte ReadByte()
        {
            if (receiveCounter == bytes.Count)
            {
                //simulate a 'timeout' because the system is trying to read more bytes then are available
                throw new TimeoutException("Timed out reading bytes");
            }
            return bytes[receiveCounter++];
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            //yes i'm ignoring offset but since this is a mock and I don't use it in the real code i'm ok.
            foreach (byte b in buffer)
            {
                bytes.Add(b);
                transmitCounter++;
            }
        }

        internal void Reset()
        {
            transmitCounter = 0;
            receiveCounter = 0;
        }
    }
}

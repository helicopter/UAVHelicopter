using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Interfaces
{
    /// <summary>
    /// This class wraps the SerialPort class. This is so that Mock classes
    /// can be introduced for testing.
    /// </summary>
    public class SerialPortInterface
    {
        private SerialPort port;

        public virtual bool IsOpen
        {
            get
            {
                return port.IsOpen;
            }
        }

        public SerialPortInterface(SerialPort port)
        {
            this.port = port;
        }

        public virtual void Open()
        {
            port.Open();
        }

        public virtual void Close()
        {
            port.Close();
        }

        public virtual byte ReadByte()
        {
            return (byte) port.ReadByte();
        }

        public virtual void Write(byte[] buffer, int offset, int count )
        {
            port.Write(buffer, offset, count);
        }
    }
}

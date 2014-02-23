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

        public short ReadShort(SerialPortInterface portInterface)
        {
            byte b1 = (byte)(ReadByte() << 8);
            byte b2 = (byte)ReadByte();

            return (short)(b1 | b2);
        }

        public int ReadInt(SerialPortInterface portInterface)
        {
            byte b1 = ReadByte();
            byte b2 = ReadByte();
            byte b3 = ReadByte();
            byte b4 = ReadByte();

            int val = (int)(b1 << 24);
            val |= (int)(b2 << 16);
            val |= (int)(b3 << 8);
            val |= (int)b4;

            return val;
            /*byte b1 = (byte)(ReadByte() << 24);
            byte b2 = (byte)(ReadByte() << 16);
            byte b3 = (byte)(ReadByte() << 8);
            byte b4 = (byte) ReadByte();*/

            //return (int)(b1 | b2 | b3 | b4);
        }

        public uint ReadUInt(SerialPortInterface portInterface)
        {
            byte b1 = ReadByte();
            byte b2 = ReadByte();
            byte b3 = ReadByte();
            byte b4 = ReadByte();

            uint val = (uint)(b1 << 24);
            val |= (uint)(b2 << 16);
            val |= (uint)(b3 << 8);
            val |= (uint)b4;

            /*uint val = (uint)(ReadByte() << 24);
            val |= (uint)(ReadByte() << 16);
            val |= (uint)(ReadByte() << 8);
            val |= (uint)ReadByte();*/

            return val;
        }


        public float ReadFloat(SerialPortInterface portInterface)
        {
            byte[] bytes = new byte[4];

            bytes[0] = ReadByte();
            bytes[1] = ReadByte();
            bytes[2] = ReadByte();
            bytes[3] = ReadByte();

            float f = BitConverter.ToSingle(bytes, 0);

            return BitConverter.ToSingle(bytes, 0);

            /*
            byte b1 = ReadByte();
            byte b2 = ReadByte();
            byte b3 = ReadByte();
            byte b4 = ReadByte();*/
            /*
            byte b1 = (byte)(ReadByte() << 24);
            byte b2 = (byte)(ReadByte() << 16);
            byte b3 = (byte)(ReadByte() << 8);
            byte b4 = (byte)ReadByte();
             * */

          //  return (float)(b1 | b2 | b3 | b4);
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

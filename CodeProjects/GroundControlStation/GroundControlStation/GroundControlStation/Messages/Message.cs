using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Messages
{
    public class Message
    {
        private byte msgType;

        protected int msgSize;

        public Message(byte msgType, int msgSize)
        {
            this.msgSize = msgSize;
            this.msgType = msgType;
        }

        public byte MsgType
        {
            get { return msgType; }
            set { msgType = value; }
        }

        public int MsgSize
        {
            get { return msgSize; }
            set { msgSize = value; }
        }


        public virtual void BuildMessage(byte[] byteBuffer)
        {
            
        }

        public virtual byte[] GetRawBytes()
        {
            return null;
        }


        protected float decodeFloat(byte[] byteBuffer, ref int positionCounter)
        {
            float val = BitConverter.ToSingle(byteBuffer, positionCounter);
            positionCounter += sizeof(float);
            return val;
        }


        protected uint decodeUInt(byte[] byteBuffer, ref int positionCounter)
        {
            uint val = (uint)BitConverter.ToInt32(byteBuffer, positionCounter);
            positionCounter += sizeof(uint);
            return val;
        }

        protected int decodeInt(byte[] byteBuffer, ref int positionCounter)
        {
            int val = BitConverter.ToInt32(byteBuffer, positionCounter);
            positionCounter += sizeof(int);
            return val;
        }

        protected byte decodeByte(byte[] byteBuffer, ref int positionCounter)
        {
            byte val = byteBuffer[positionCounter];
            positionCounter += sizeof(byte);
            return val;
        }

        protected void encode(ref byte[] rawMsg, byte data, ref int positionCounter)
        {
            rawMsg[positionCounter++] = data;
        }

        protected void encode(ref byte[] rawMsg, float data, ref int positionCounter)
        {
            byte[] temp = BitConverter.GetBytes(data);

            rawMsg[positionCounter++] = temp[0];
            rawMsg[positionCounter++] = temp[1];
            rawMsg[positionCounter++] = temp[2];
            rawMsg[positionCounter++] = temp[3];
        }

        protected void encode(ref byte[] rawMsg, int data, ref int positionCounter)
        {
            byte[] temp = BitConverter.GetBytes(data);

            rawMsg[positionCounter++] = temp[0];
            rawMsg[positionCounter++] = temp[1];
            rawMsg[positionCounter++] = temp[2];
            rawMsg[positionCounter++] = temp[3];
        }


        protected void encode(ref byte[] rawMsg, uint data, ref int positionCounter)
        {
            encode(ref rawMsg, (int)data, ref positionCounter);
        }

    }
}

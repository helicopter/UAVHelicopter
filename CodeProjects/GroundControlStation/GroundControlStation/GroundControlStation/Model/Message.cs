using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Model
{
    public class Message
    {
        private byte msgType;

        private int msgSize;

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
    }
}

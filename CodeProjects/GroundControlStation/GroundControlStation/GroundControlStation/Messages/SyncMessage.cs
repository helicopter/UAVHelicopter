using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Messages
{
    public class SyncMessage: Message
    {
        public const byte MessageType = 3;

        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        public const int NumOfBytesInMsg = sizeof(byte);

        public SyncMessage() 
            : base(MessageType, NumOfBytesInMsg)
        {
            MsgType = MessageType;
        }

        public static SyncMessage BuildMessageSt(byte[] byteBuffer)
        {
            SyncMessage sync = new SyncMessage();
            sync.BuildMessage(byteBuffer);

            return sync;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            this.MsgType = byteBuffer[0];
        }

        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            rawMsg[0] = MsgType;

            return rawMsg;
        }
    }
}

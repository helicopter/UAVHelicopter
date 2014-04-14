using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Messages
{
    public class ControlMessage : Message
    {
        public const byte MessageType = 5;

        public float MainRotorCollectiveControl;
        public float YawControl;
        public float LongitudeControl;
        public float LateralControl;


        public const int NumOfBytesInMsg =
           sizeof(byte) +
           sizeof(float) +
           sizeof(float) +
           sizeof(float) +
           sizeof(float);

        public ControlMessage()
            : base(MessageType, NumOfBytesInMsg)
        {
        }


        public static ControlMessage BuildMessageSt(byte[] byteBuffer)
        {
            ControlMessage data = new ControlMessage();
            data.BuildMessage(byteBuffer);

            return data;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);
            MainRotorCollectiveControl = decodeFloat(byteBuffer, ref positionCounter);
            YawControl = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeControl = decodeFloat(byteBuffer, ref positionCounter);
            LateralControl = decodeFloat(byteBuffer, ref positionCounter);
        }

        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);
            encode(ref rawMsg, MainRotorCollectiveControl, ref positionCounter);
            encode(ref rawMsg, YawControl, ref positionCounter);
            encode(ref rawMsg, LongitudeControl, ref positionCounter);
            encode(ref rawMsg, LateralControl, ref positionCounter);

            return rawMsg;
        }
    }
}

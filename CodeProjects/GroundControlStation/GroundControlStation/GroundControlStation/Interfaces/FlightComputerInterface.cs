using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Model;
using GroundControlStation.Messages;

namespace GroundControlStation.Interfaces
{
    /// <summary>
    /// This class interfaces with the flight computer for sending and receiving
    /// data.
    /// </summary>
    public class FlightComputerInterface : IDisposable
    {
        public const byte SyncByte1 = 0xB5;
        public const byte SyncByte2 = 0x62;

        /// <summary>
        /// I needed to introduce the third checksum byte because the buffer is apparently 3 bytes large
        /// on the avr processor. So the first two sync bytes were getting put on, and then the
        /// third byte was just constantly getting overwritten. So on the receiving end the
        /// processor thought it had a valid message because of the sync bytes but would then
        /// puke because the third byte was an invalid msg type. 
        /// </summary>

        public const byte SyncByte3 = SyncByte1 ^ SyncByte2; //0xD7, 215

        /// <summary>
        /// The number of bytes of the header and footer information.
        /// 3 header bytes (the three sync bytes) and 2 checksum bytes.
        /// </summary>
        static int MsgHeaderFooterSize = 5;

        private SerialPortInterface port;

        public FlightComputerInterface(SerialPortInterface port)
        {
            this.port = port;
        }

        public virtual void Open()
        {
            if (!port.IsOpen)
            {
                port.Open();
            }
        }

        public virtual void Close()
        {
            if (port.IsOpen)
            {
                port.Close();
            }
        }

        public void Dispose()
        {
            Close();
        }

        /// <summary>
        /// Receives telemetry data from the helicopter and parses it
        /// into telemetry messages
        /// </summary>
        /// <returns>
        /// An object containing the telemetry data.
        /// Null in the event of a timeout, or if the port is closed, or the message checksum is invalid. 
        /// </returns>
        /// <exception cref="SystemException">Thrown when there is an issue parsing the received telemetry data</exception>
        public virtual Message Receive()
        {
            
            //TODO should I check to see if the port is opened first?
            Message data = null;

            if (port.IsOpen)
            {

                try
                {
                    
                    try
                    {
                        //Read until the sync bytes are received or we time out.
                        //Throw away the 'garbage' bytes.
                        byte firstSyncByte = 0;
                        byte secondSyncByte = 0;
                        byte thirdSyncByte = 0;

                        while (!(firstSyncByte == SyncByte1 && secondSyncByte == SyncByte2 && thirdSyncByte == SyncByte3))
                        {
                            firstSyncByte = secondSyncByte;
                            secondSyncByte = thirdSyncByte;
                            thirdSyncByte = port.ReadByte();
                        }

                        //once we have found a valid message, get the message ID
                        byte messageType = port.ReadByte();

                        int messageSize = 0;

                        if (messageType == FlightComputerTelemetryMessage.MessageType)
                        {
                            messageSize = FlightComputerTelemetryMessage.NumOfBytesInMsg;
                        }else if (messageType == SyncMessage.MessageType)
                        {
                            messageSize = SyncMessage.NumOfBytesInMsg;
                        }
                        else if (messageType == SensorDataMessage.MessageType)
                        {
                            messageSize = SensorDataMessage.NumOfBytesInMsg;
                        }
                        else if (messageType == ControlMessage.MessageType)
                        {
                            messageSize = ControlMessage.NumOfBytesInMsg;
                        }
                        else if (messageType == SimpleTelemetryMessage.MessageType)
                        {
                            messageSize = SimpleTelemetryMessage.NumOfBytesInMsg;
                        }
                        else
                        {
                            Debug.WriteLine("Invalid msgType");

                            return data;
                        }


                        byte[] messagePayload = new byte[messageSize];

                        messagePayload[0] = messageType;

                        //skip the first position since thats where the message type is located.
                        for (int i = 1; i < messageSize; i++)
                        {
                            messagePayload[i] = port.ReadByte();
                        }

                        //Read the two checksum bytes
                        byte messageChecksumA = port.ReadByte();
                        byte messageChecksumB = port.ReadByte();

                        byte calculatedChecksumA = 0;
                        byte calculatedChecksumB = 0;

                        calculateChecksum(messagePayload, ref calculatedChecksumA, ref calculatedChecksumB);

                        //verify that the checksum is correct
                        if (calculatedChecksumA == messageChecksumA && calculatedChecksumB == messageChecksumB)
                        {
                            //build the message
                            if (messageType == FlightComputerTelemetryMessage.MessageType)
                            {
                                data = FlightComputerTelemetryMessage.BuildMessageSt(messagePayload);
                            }else if (messageType == SyncMessage.MessageType)
                            {
                                data = SyncMessage.BuildMessageSt(messagePayload);
                            }
                            else if (messageType == SensorDataMessage.MessageType)
                            {
                                data = SensorDataMessage.BuildMessageSt(messagePayload);
                            }
                            else if (messageType == ControlMessage.MessageType)
                            {
                                data = ControlMessage.BuildMessageSt(messagePayload);
                            }
                            else if (messageType == SimpleTelemetryMessage.MessageType)
                            {
                                data = SimpleTelemetryMessage.BuildMessageSt(messagePayload);
                            }
                        }
                        else
                        {
                                
                            Debug.WriteLine("Invalid CRC");
                        }
                    }
                    catch (TimeoutException)
                    {
                        //swallow the timeout since this is expected behavior.
                        Debug.WriteLine("Timed out");
                    }
                }
                catch (SystemException ex)
                {
                    throw new SystemException("Exception while processing telemetry data", ex);
                }
            }

            return data;
        }

        /// <summary>
        /// Transmit the telemetry message and append appropriate sync and checksum bytes.
        /// </summary>
        /// <param name="telemetry"></param>
        public virtual void Transmit(Message telemetry)
        {
            byte[] msgPayload = telemetry.GetRawBytes();

            byte[] completeMsg = new byte[msgPayload.Length + MsgHeaderFooterSize];

            //First synch byte used to determine if this data is a start of a new msg.
            completeMsg[0] = SyncByte1;
            completeMsg[1] = SyncByte2;
            completeMsg[2] = SyncByte3;

            msgPayload.CopyTo(completeMsg, 3);

            byte checksumA = 0;
            byte checksumB = 0;

            calculateChecksum(msgPayload, ref checksumA, ref checksumB);

            //insert checksum values into message to send.
            completeMsg[completeMsg.Length - 2] = checksumA;
            completeMsg[completeMsg.Length - 1] = checksumB;

            port.Write(completeMsg, 0, completeMsg.Length);
        }

        private void calculateChecksum(byte[] msgPayload, ref byte checksumA, ref byte checksumB)
        {
            //These fields can overflow, but that is intentional.
            foreach (byte b in msgPayload)
            {
                checksumA = (byte) (checksumA + b);
                checksumB = (byte) (checksumB + checksumA);
            }
        }
    }
}

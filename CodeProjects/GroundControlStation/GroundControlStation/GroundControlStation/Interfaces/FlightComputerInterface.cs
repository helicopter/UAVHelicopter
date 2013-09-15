using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Model;

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
        public virtual FlightComputerTelemetry Receive()
        {
            
            //TODO should I check to see if the port is opened first?
            FlightComputerTelemetry data = null;

            if (port.IsOpen)
            {

                try
                {
                    
                    try
                    {
                        //Read until the sync bytes are received or we time out.
                        //Throw away the 'garbage' bytes.
                        byte previousByte = 0;
                        byte currentByte = 0;

                        while (!(previousByte == SyncByte1 && currentByte == SyncByte2))
                        {
                            previousByte = currentByte;
                            currentByte = port.ReadByte();
                        }

                        //once we have found a valid message, get the message ID
                        byte messageType = port.ReadByte();

                        if (messageType == FlightComputerTelemetry.FlightControllerTelemetryMessageType)
                        {
                            byte[] messagePayload = new byte[FlightComputerTelemetry.NumOfBytesInMsg];

                            messagePayload[0] = messageType;

                            //skip the first position since thats where the message type is located.
                            for (int i = 1; i < FlightComputerTelemetry.NumOfBytesInMsg; i++)
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
                                data = FlightComputerTelemetry.BuildMessageSt(messagePayload);
                            }
                            else
                            {
                                
                                Debug.WriteLine("Invalid CRC");
                            }
                        }
                        else
                        {
                            Debug.WriteLine("Invalid Message Type Received");
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
        public virtual void Transmit(FlightComputerTelemetry telemetry)
        {
            byte[] msgPayload = telemetry.GetRawBytes();

            byte[] completeMsg = new byte[msgPayload.Length + 4];

            //First synch byte used to determine if this data is a start of a new msg.
            completeMsg[0] = SyncByte1;
            completeMsg[1] = SyncByte2;

            msgPayload.CopyTo(completeMsg, 2);

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

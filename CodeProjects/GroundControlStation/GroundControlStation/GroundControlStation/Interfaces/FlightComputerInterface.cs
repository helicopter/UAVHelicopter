﻿using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Interfaces
{
    /// <summary>
    /// This class interfaces with the flight computer for sending and receiving
    /// data.
    /// </summary>
    public class FlightComputerInterface : IDisposable
    {
        private SerialPort port;

        public FlightComputerInterface(SerialPort port)
        {
            this.port = port;
        }

        public void Open()
        {
            if (!port.IsOpen)
            {
                port.Open();
            }
        }

        public void Close()
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
        /// <returns>An object containing the telemetry data</returns>
        /// <exception cref="SystemException">Thrown when there is an issue parsing the received telemetry data</exception>
        public TelemetryData Receive()
        {
            //TODO should I check to see if the port is opened first?
            TelemetryData data = null;

            try
            {
                byte[] byteBuffer = new byte[TelemetryData.GetNumOfBytesInMsg()];

                for (int i = 0; i < TelemetryData.GetNumOfBytesInMsg(); i++)
                {
                    int temp = port.ReadByte();

                    if (temp != -1)
                    {
                        byteBuffer[i] = (byte) temp;
                    }
                    else
                    {
                        throw new SystemException("Unexpectedly reached the end of the telemetry buffer");
                    }
                }

                data = new TelemetryData();

                data.BuildMessage(byteBuffer);

            }
            catch (SystemException ex)
            {
                throw new SystemException("Exception while processing telemetry data", ex);
            }

            return data;
        }

        public void Transmit(TelemetryData telemetryData)
        {
            port.Write(telemetryData.GetRawBytes(), 0, TelemetryData.GetNumOfBytesInMsg());
        }
    }
}
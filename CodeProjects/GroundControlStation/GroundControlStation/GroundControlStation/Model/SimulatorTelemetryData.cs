using System;
using System.IO;

namespace GroundControlStation.Model
{
    /// <summary>
    /// Class which represents the data received from the xplane simulator
    ///
    /// </summary>
    public class SimulatorTelemetryData
    {
        public byte[] RawData { get; set; }

        /// <summary>
        /// Heading of the aircraft in degrees from 0 - 259.99..., where 0/360 is north.
        /// </summary>
        public float HeadingDegrees { get; set; }

        /// <summary>
        /// Creates an XPlaneData object from the raw bytes received from xplane.
        /// </summary>
        /// <remarks>
        /// In Xplane's input & output page, the following data sets should be enabled for internet broadcasting
        /// This parser assumes that these are enabled, and will not parse correctly otherwise:
        /// Joystick ail/elv/rud
        /// angular velocities
        /// pitch, roll, headings
        /// lat, long, altitude
        /// loc, vel, dist traveled
        /// prop pitch
        /// </remarks>
        /// <param name="xplaneBytes">Raw bytes received from xplane</param>
        /// <returns>An object containing the parsed xplane data</returns>
        /// <exception cref="SystemException"></exception>
        internal static SimulatorTelemetryData Create(byte[] xplaneBytes)
        {
            SimulatorTelemetryData data = new SimulatorTelemetryData();
            data.RawData = xplaneBytes;

            try
            {
                BinaryReader byteReader = new BinaryReader(new MemoryStream(xplaneBytes));

                //Skip to the relevant data
                byteReader.ReadBytes(89);

                data.HeadingDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
            }
            catch (SystemException ex)
            {
                throw new SystemException("Unable to parse xplane data", ex);
            }

            return data;
        }
    }
}

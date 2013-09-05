using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Configuration;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Interfaces
{
    /// <summary>
    /// Class which represents the data received from xplane
    ///
    /// </summary>
    public class XPlaneData
    {
        public byte[] RawData { get; set; }

        public float YawDegrees { get; set; }

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
        internal static XPlaneData Create(byte[] xplaneBytes)
        {
            XPlaneData data = new XPlaneData();
            data.RawData = xplaneBytes;

            try
            {
                BinaryReader byteReader = new BinaryReader(new MemoryStream(xplaneBytes));

                //Skip to the relevant data
                byteReader.ReadBytes(89);

                data.YawDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
            }
            catch (SystemException ex)
            {
                throw new SystemException("Unable to parse xplane data", ex);
            }

            return data;
        }
    }
}

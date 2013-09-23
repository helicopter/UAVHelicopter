using System;
using System.Collections.Generic;
using System.IO;

namespace GroundControlStation.Model
{
    /// <summary>
    /// Class which represents the data received from the xplane simulator
    ///
    /// </summary>
    public class SimulatorTelemetry
    {
        public byte[] RawData { get; set; }

        public float PitchVelocityMs { get; set; }

        public float RollVelocityMs { get; set; }

        public float YawVelocityMs { get; set; }

        public float PitchDegrees { get; set; }

        public float RollDegrees { get; set; }

        /// <summary>
        /// Heading of the aircraft in degrees from 0 - 259.99..., where 0/360 is north.
        /// </summary>
        public float MagHeadingDegrees { get; set; }

        public float XLattitudeDegrees { get; set; }

        public float YLongitudeDegrees { get; set; }

        public float ZAltitudeFtAgl { get; set; }

        public float YVelocityNEDFrameMs { get; set; }

        public float ZVelocityNEDFrameMs { get; set; }

        public float XVelocityNEDFrameMs { get; set; }

        

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
        internal static SimulatorTelemetry Create(byte[] xplaneBytes)
        {
            SimulatorTelemetry data = new SimulatorTelemetry();
            data.RawData = xplaneBytes;

            try
            {
                BinaryReader byteReader = new BinaryReader(new MemoryStream(xplaneBytes));

                //Skip to the relevant data
                byteReader.ReadBytes(45);
                data.PitchVelocityMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.RollVelocityMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.YawVelocityMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(24);
                data.PitchDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.RollDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip true heading to get to mag heading.
                byteReader.ReadBytes(4);
                data.MagHeadingDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(24);
                data.XLattitudeDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.YLongitudeDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(4);
                data.ZAltitudeFtAgl = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(32);
                data.YVelocityNEDFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.ZVelocityNEDFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.XVelocityNEDFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                /*byteReader.ReadBytes(89);

                data.MagHeadingDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);*/
            }
            catch (SystemException ex)
            {
                throw new SystemException("Unable to parse xplane data", ex);
            }

            return data;
        }

        internal List<Tuple<string, string>> ListValues()
        {
            List<Tuple<string,string>> lstValues = new List<Tuple<string, string>>();

            lstValues.Add(new Tuple<string, string>("Pitch V Ms", PitchVelocityMs.ToString()));
            lstValues.Add(new Tuple<string, string>("Roll V Ms", RollVelocityMs.ToString()));
            lstValues.Add(new Tuple<string, string>("Yaw V Ms", YawVelocityMs.ToString()));
            lstValues.Add(new Tuple<string, string>("Pitch Deg", PitchDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Roll Deg", RollDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Mag Heading Deg", MagHeadingDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("X Lat Deg", XLattitudeDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Y Long Deg", YLongitudeDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Z Alt Ft Agl", ZAltitudeFtAgl.ToString()));
            lstValues.Add(new Tuple<string, string>("Y V Ms", YVelocityNEDFrameMs.ToString()));
            lstValues.Add(new Tuple<string, string>("Z V Ms", ZVelocityNEDFrameMs.ToString()));
            lstValues.Add(new Tuple<string, string>("X V Ms", XVelocityNEDFrameMs.ToString()));

            return lstValues;
        }

    }
}

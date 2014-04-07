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

        public float PitchVelocityRadsPerS { get; set; }

        public float RollVelocityRadsPerS { get; set; }

        public float YawVelocityRadsPerS { get; set; }

        public float PitchDegrees { get; set; }

        public float RollDegrees { get; set; }

        /// <summary>
        /// Heading of the aircraft in degrees from 0 - 259.99..., where 0/360 is north.
        /// </summary>
        public float MagHeadingDegrees { get; set; }

        public float LatitudeDegrees { get; set; }

        public float LongitudeDegrees { get; set; }

        public float ZAltitudeFtAgl { get; set; }

        /*
        public float YVelocityFRDBodyFrameMs { get; set; }

        public float ZVelocityFRDBodyFrameMs { get; set; }

        public float XVelocityFRDBodyFrameMs { get; set; }
         */

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
                data.PitchVelocityRadsPerS = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.RollVelocityRadsPerS = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.YawVelocityRadsPerS = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(24);
                data.PitchDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.RollDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip true heading to get to mag heading.
                byteReader.ReadBytes(4);
                data.MagHeadingDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(20);
                data.LatitudeDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.LongitudeDegrees = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                data.ZAltitudeFtMsl = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                data.ZAltitudeFtAgl = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);

                //Skip to the relevant data
                byteReader.ReadBytes(32);

                //The data from the simulator is in the following order: X,Y,Z.
                //X is positive pointing east
                //Y is positive pointing up
                //Z is positive pointing south
                //So it needs to be converted to North East Down.
                float simXVelocityEUSFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                float simYVelocityEUSFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
                float simZVelocityEUSFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0);
               
                /**
                 * Reformat the velocity variables to NED frame.
                 * Multiply by -1 to convert to down positive. Y value from simulator *-1 is Z
                 * Multiply by -1 to convert to north positive. Z value from simualtor * -1 is X
                 */
                data.XVelocityNEDMs = simZVelocityEUSFrameMs * -1; //multiply by -1 to convert to north positive.
                data.YVelocityNEDMs = simXVelocityEUSFrameMs;
                data.ZVelocityNEDMs = simYVelocityEUSFrameMs * -1; //multiply by -1 to convert to down positive.


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

            lstValues.Add(new Tuple<string, string>("Pitch V Ms", PitchVelocityRadsPerS.ToString()));
            lstValues.Add(new Tuple<string, string>("Roll V Ms", RollVelocityRadsPerS.ToString()));
            lstValues.Add(new Tuple<string, string>("Yaw V Ms", YawVelocityRadsPerS.ToString()));
            lstValues.Add(new Tuple<string, string>("Pitch Deg", PitchDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Roll Deg", RollDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Mag Heading Deg", MagHeadingDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("X Lat Deg", LatitudeDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Y Long Deg", LongitudeDegrees.ToString()));
            lstValues.Add(new Tuple<string, string>("Z Alt Ft Agl", ZAltitudeFtAgl.ToString()));
            lstValues.Add(new Tuple<string, string>("Y V Ms", YVelocityNEDMs.ToString()));
            lstValues.Add(new Tuple<string, string>("Z V Ms", ZVelocityNEDMs.ToString()));
            lstValues.Add(new Tuple<string, string>("X V Ms", XVelocityNEDMs.ToString()));

            return lstValues;
        }

        public float ZAltitudeFtMsl { get; set; }

        public float XVelocityNEDMs { get; set; }

        public float YVelocityNEDMs { get; set; }

        public float ZVelocityNEDMs { get; set; }
    }
}

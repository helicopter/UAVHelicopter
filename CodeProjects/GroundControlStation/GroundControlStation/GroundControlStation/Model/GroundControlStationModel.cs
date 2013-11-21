using GroundControlStation.Messages;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Model
{
    public class GroundControlStationModel
    {
        public float MagYaw { get; set; }

        public float YawVelocityDegreesPerSecond { get; set; }


        public float YawIntegral { get; set; }

        public float YawProportional { get; set; }

        public float YawDerivativeError { get; set; }

        public float YawControl { get; set; }

        public int Timeouts { get; set; }

        public int UnrecognizedMsgTypes { get; set; }

        public int ChecksumErrors { get; set; }

        public int NumOfBlownFrames { get; set; }

        public SimulatorTelemetry SimTelm { get; set; }

        public GroundControlStationModel()
        {
            SimTelm = new SimulatorTelemetry();
        }


        public System.Collections.Generic.IEnumerable<Tuple<string, string>> ListValues()
        {
            List<Tuple<string, string>> lstValues = new List<Tuple<string, string>>();

            lstValues.Add(new Tuple<string, string>("FC Mag Yaw", MagYaw.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Velocity DPS", YawVelocityDegreesPerSecond.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Integral", YawIntegral.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Prop", YawProportional.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Deriv", YawDerivativeError.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Control", YawControl.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Timeouts", Timeouts.ToString()));

            lstValues.Add(new Tuple<string, string>("FC UnrecognizedMsgTypes", UnrecognizedMsgTypes.ToString()));

            lstValues.Add(new Tuple<string, string>("FC ChecksumErrors", ChecksumErrors.ToString()));

            lstValues.Add(new Tuple<string, string>("FC NumOfBlownFrames", NumOfBlownFrames.ToString()));


            return lstValues;
        }

    }
}

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


        public class Results
        {
            public double median = 0;
            public double average = 0;
            public double minimum = 0;
            public double q1 = 0;
            public double q3 = 0;
            public double maximum = 0;

            public double magVal = 0;
            public double accelVal = 0;

            public List<double> values = new List<double>();

            public void calc()
            {
                values.Sort();

                median = values[values.Count / 2];
                q1 = values[values.Count / 4];
                q3 = values[(values.Count / 4) + (values.Count / 2)];

                minimum = values.Min();
                maximum = values.Max();
                average = values.Average();


            }

            public override string ToString()
            {
                return String.Format("{0},{1},{2},{3},{4},{5},{6},{7}", minimum, q1, median, average, q3, maximum,magVal,accelVal);
            }
        }


        public float PreviousXVelocityNEDCms { get; set; }

        public float PreviousYVelocityNEDCms { get; set; }

        public float PreviousZVelocityNEDCms { get; set; }


        public float MagYaw { get; set; }

        public float YawVelocityRadsPerSecond { get; set; }


        public float YawIntegral { get; set; }

        public float YawProportional { get; set; }

        public float YawDerivativeError { get; set; }

        public float YawControl { get; set; }



        public float YawIntegralGain { get; set; }

        public float YawDerivativeGain { get; set; }

        public float YawProportionalGain { get; set; }

        public float YawAntiWindupGain { get; set; }



        public int Timeouts { get; set; }

        public int UnrecognizedMsgTypes { get; set; }

        public int ChecksumErrors { get; set; }

        public int NumOfBlownFrames { get; set; }

        public int SerialCommunicationBufferOverruns { get; set; }

        public float XNEDLocalFrame { get; set; }

        public float XVelocityMetersPerSecond { get; set; }

        public float ThetaPitchDegrees { get; set; }

        public float XIntegral { get; set; }

        public float XProportional { get; set; }

        public float XDerivativeError { get; set; }

        public float LongitudeControl { get; set; }

        public float XIntegralGain { get; set; }

        public float XDerivativeGain { get; set; }

        public float XProportionalGain { get; set; }

        public float XAntiWindupGain { get; set; }

        public float LongitudeInnerLoopGain { get; set; }

        public float YNEDLocalFrame { get; set; }

        public float YVelocityMetersPerSecond { get; set; }

        public float PhiRollDegrees { get; set; }

        public float YIntegral { get; set; }

        public float YProportional { get; set; }

        public float YDerivativeError { get; set; }

        public float LateralControl { get; set; }

        public float YIntegralGain { get; set; }

        public float YDerivativeGain { get; set; }

        public float YProportionalGain { get; set; }

        public float YAntiWindupGain { get; set; }

        public float LateralInnerLoopGain { get; set; }

        public float AltitudeMetersAgl { get; set; }

        public float ZVelocityFeetPerSecond { get; set; }

        public float ZIntegral { get; set; }

        public float ZProportional { get; set; }

        public float ZDerivativeError { get; set; }

        public float MainRotorCollectiveControl { get; set; }

        public float ZIntegralGain { get; set; }

        public float ZDerivativeGain { get; set; }

        public float ZProportionalGain { get; set; }

        public float ZAntiWindupGain { get; set; }

        public float ZNEDLocalFrame { get; set; }


        public float XLongitudeOuterLoopSetpoint { get; set; }

        public float YLateralOuterLoopSetpoint { get; set; }

        public float PitchAngularVelocityRadsPerSecond { get; set; }

        public float RollAngularVelocityRadsPerSecond { get; set; }

        public float PitchAngularVelocityGain { get; set; }

        public float RollAngularVelocityGain { get; set; }



        public float XAccelFrdMss { get; set; }

        public float YAccelFrdMss { get; set; }

        public float ZAccelFrdMss { get; set; }

        public float XMagFrd { get; set; }

        public float YMagFrd { get; set; }

        public float ZMagFrd { get; set; }

        public int XVEcefCms { get; set; }

        public int YVEcefCms { get; set; }

        public int ZVEcefCms { get; set; }

        public float PressureMillibars { get; set; }

        public int XEcefCm { get; set; }

        public int YEcefCm { get; set; }

        public int ZEcefCm { get; set; }

        public float YawRads { get; set; }

        public float RollRads { get; set; }

        public float PitchRads { get; set; }
        

        public SimulatorTelemetry SimTelm { get; set; }

        public GroundControlStationModel()
        {
            SimTelm = new SimulatorTelemetry();
        }


        public System.Collections.Generic.IEnumerable<Tuple<string, string>> ListValues()
        {
            List<Tuple<string, string>> lstValues = new List<Tuple<string, string>>();

            lstValues.Add(new Tuple<string, string>("FC Mag Yaw", MagYaw.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Velocity RPS", YawVelocityRadsPerSecond.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Integral", YawIntegral.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Prop", YawProportional.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Deriv", YawDerivativeError.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Yaw Control", YawControl.ToString()));

            


            lstValues.Add(new Tuple<string, string>("FC Y Deriv", YDerivativeError.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Y Prop", YProportional.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Y Control", LateralControl.ToString()));

            lstValues.Add(new Tuple<string, string>("FC XOuterLoopSetpoint", XLongitudeOuterLoopSetpoint.ToString()));
            lstValues.Add(new Tuple<string, string>("FC YOuterLoopSetpoint", YLateralOuterLoopSetpoint.ToString()));

            lstValues.Add(new Tuple<string, string>("FC XNED", XNEDLocalFrame.ToString()));

            lstValues.Add(new Tuple<string, string>("FC YNED", YNEDLocalFrame.ToString()));

            lstValues.Add(new Tuple<string, string>("FC ZNED", AltitudeMetersAgl.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Timeouts", Timeouts.ToString()));

            lstValues.Add(new Tuple<string, string>("FC UnrecognizedMsgTypes", UnrecognizedMsgTypes.ToString()));

            lstValues.Add(new Tuple<string, string>("FC ChecksumErrors", ChecksumErrors.ToString()));

            lstValues.Add(new Tuple<string, string>("FC NumOfBlownFrames", NumOfBlownFrames.ToString()));

            lstValues.Add(new Tuple<string, string>("FC SerialCommunicationBufferOverruns", SerialCommunicationBufferOverruns.ToString()));

            


            return lstValues;
        }




        public float XRefSetpoint { get; set; }

        public float YRefSetpoint { get; set; }

        public float ZRefSetpoint { get; set; }

        public float YawRefSetpoint { get; set; }



        public bool CalcMSE { get; set; }
        public double RollMSE { get; set; }
        public double PitchMSE { get; set; }
        public double YawMSE { get; set; }
        public List<Results> RollResults { get; set; }
        public List<Results> YawResults { get; set; }
        public List<Results> PitchResults { get; set; }
        
        public int MSEIterations { get; set; }
 
    }
}

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



        public float YawIntegralGain { get; set; }

        public float YawDerivativeGain { get; set; }

        public float YawProportionalGain { get; set; }

        public float YawAntiWindupGain { get; set; }



        public int Timeouts { get; set; }

        public int UnrecognizedMsgTypes { get; set; }

        public int ChecksumErrors { get; set; }

        public int NumOfBlownFrames { get; set; }


        public float XNEDBodyFrame { get; set; }

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

        public float YNEDBodyFrame { get; set; }

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

        public float AltitudeFeetAgl { get; set; }

        public float ZVelocityFeetPerSecond { get; set; }

        public float ZIntegral { get; set; }

        public float ZProportional { get; set; }

        public float ZDerivativeError { get; set; }

        public float MainRotorCollectiveControl { get; set; }

        public float ZIntegralGain { get; set; }

        public float ZDerivativeGain { get; set; }

        public float ZProportionalGain { get; set; }

        public float ZAntiWindupGain { get; set; }

        public float ZNEDBodyFrame { get; set; }


        public float XLongitudeOuterLoopSetpoint { get; set; }

        public float YLateralOuterLoopSetpoint { get; set; }

        public float PitchAngularVelocityRadsPerSecond { get; set; }

        public float RollAngularVelocityRadsPerSecond { get; set; }


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

            


            lstValues.Add(new Tuple<string, string>("FC Y Deriv", YDerivativeError.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Y Prop", YProportional.ToString()));
            lstValues.Add(new Tuple<string, string>("FC Y Control", LateralControl.ToString()));

            lstValues.Add(new Tuple<string, string>("FC XOuterLoopSetpoint", XLongitudeOuterLoopSetpoint.ToString()));
            lstValues.Add(new Tuple<string, string>("FC YOuterLoopSetpoint", YLateralOuterLoopSetpoint.ToString()));

            lstValues.Add(new Tuple<string, string>("FC XNED", XNEDBodyFrame.ToString()));

            lstValues.Add(new Tuple<string, string>("FC YNED", YNEDBodyFrame.ToString()));

            lstValues.Add(new Tuple<string, string>("FC ZNED", AltitudeFeetAgl.ToString()));

            lstValues.Add(new Tuple<string, string>("FC Timeouts", Timeouts.ToString()));

            lstValues.Add(new Tuple<string, string>("FC UnrecognizedMsgTypes", UnrecognizedMsgTypes.ToString()));

            lstValues.Add(new Tuple<string, string>("FC ChecksumErrors", ChecksumErrors.ToString()));

            lstValues.Add(new Tuple<string, string>("FC NumOfBlownFrames", NumOfBlownFrames.ToString()));

            


            return lstValues;
        }

        public float PitchAngularVelocityGain { get; set; }

        public float RollAngularVelocityGain { get; set; }
    }
}

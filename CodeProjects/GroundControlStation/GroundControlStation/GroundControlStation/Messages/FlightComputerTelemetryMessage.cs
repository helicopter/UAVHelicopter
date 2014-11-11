using GroundControlStation.Model;
using System;
using System.Collections.Generic;
using System.Text;
using GroundControlStation.Util;

namespace GroundControlStation.Messages
{
    public class FlightComputerTelemetryMessage : Message
    {
        public const byte MessageType = 2;


        public float YawIntegral;

        public float YawProportional;

        public float YawDerivativeError;

        public float YawControl;


        public float YawIntegralGain;

        public float YawDerivativeGain;

        public float YawProportionalGain;

        public float YawAntiWindupGain;







        public float XNEDLocalFrame;
        public float XVelocityFRDCms;
        public float PitchRads;

        public float XIntegral;
        public float XProportional;
        public float XDerivativeError;
        public float LongitudeControl;
        public float XIntegralGain;
        public float XDerivativeGain;
        public float XProportionalGain;
        public float XAntiWindupGain;
        public float LongitudeInnerLoopGain;
        public float XLongitudeOuterLoopSetpoint;
        public float PitchAngularVelocityGain;

        public float YNEDLocalFrame;
        public float YVelocityFRDCms;
        public float RollRads;
        public float YIntegral;
        public float YProportional;
        public float YDerivativeError;
        public float LateralControl;
        public float YIntegralGain;
        public float YDerivativeGain;
        public float YProportionalGain;
        public float YAntiWindupGain;
        public float LateralInnerLoopGain;
        public float YLateralOuterLoopSetpoint;
        public float RollAngularVelocityGain;

        public float AltitudeMetersAgl;
        public float ZVelocityFRDCms;
        public float YawRads;
        public float ZIntegral;
        public float ZProportional;
        public float ZDerivativeError;
        public float MainRotorCollectiveControl;
        public float ZIntegralGain;
        public float ZDerivativeGain;
        public float ZProportionalGain;
        public float ZAntiWindupGain;
        public float ZNEDLocalFrame;

        public float LatitudeDegrees;
        public float LongitudeDegrees;


        public float XAccelFrdMss;
        public float YAccelFrdMss;
        public float ZAccelFrdMss;
        public float YawAngularVelocityRadsPerSecond;
        public float PitchAngularVelocityRadsPerSecond;
        public float RollAngularVelocityRadsPerSecond;
        public float XMagFrd;
        public float YMagFrd;
        public float ZMagFrd;
        public int XEcefCm;
        public int YEcefCm;
        public int ZEcefCm;
        public int XVEcefCms;
        public int YVEcefCms;
        public int ZVEcefCms;
        public float PressureMillibars;

        public float XRefSetpoint;
        public float YRefSetpoint;
        public float ZRefSetpoint;
        public float YawRefSetpoint;


        public int Timeouts;
        public int UnrecognizedMsgTypes;
        public int ChecksumErrors;
        public int NumOfBlownFrames;
        public int SerialCommunicationBufferOverruns;

        /// <summary>
        /// This represents the number of bytes in this message including the message type.
        /// </summary>
        public const int NumOfBytesInMsg =
            sizeof(byte) +
            sizeof(float) +

            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +





            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +

            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +

            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +

            sizeof(float) +
            sizeof(float) +

            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) + 
            sizeof(float) +
            sizeof(int) +
            sizeof(int) +  
            sizeof(int) + 
            sizeof(int) + 
            sizeof(int) +
            sizeof(int) + 
            sizeof(float) + 

                        sizeof(float) +
            sizeof(float) +
            sizeof(float) +
            sizeof(float) +

            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int) +
            sizeof(int);


        public FlightComputerTelemetryMessage()
            : base(MessageType, NumOfBytesInMsg)
        {
        }

        public static FlightComputerTelemetryMessage BuildMessageSt(byte[] byteBuffer)
        {
            FlightComputerTelemetryMessage fct = new FlightComputerTelemetryMessage();
            fct.BuildMessage(byteBuffer);

            return fct;
        }

        public override void BuildMessage(byte[] byteBuffer)
        {
            int positionCounter = 0;
            MsgType = decodeByte(byteBuffer, ref positionCounter);

            YawIntegral = decodeFloat(byteBuffer, ref positionCounter);
            YawProportional = decodeFloat(byteBuffer, ref positionCounter);
            YawDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            YawControl = decodeFloat(byteBuffer, ref positionCounter);
            YawIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            YawDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            YawProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            YawAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);




            XNEDLocalFrame = decodeFloat(byteBuffer, ref positionCounter);
            XVelocityFRDCms = decodeFloat(byteBuffer, ref positionCounter);
            PitchRads = decodeFloat(byteBuffer, ref positionCounter);
            XIntegral = decodeFloat(byteBuffer, ref positionCounter);
            XProportional = decodeFloat(byteBuffer, ref positionCounter);
            XDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeControl = decodeFloat(byteBuffer, ref positionCounter);
            XIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            XDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            XProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            XAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeInnerLoopGain = decodeFloat(byteBuffer, ref positionCounter);
            XLongitudeOuterLoopSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            PitchAngularVelocityGain = decodeFloat(byteBuffer, ref positionCounter);

            YNEDLocalFrame = decodeFloat(byteBuffer, ref positionCounter);
            YVelocityFRDCms = decodeFloat(byteBuffer, ref positionCounter);
            RollRads = decodeFloat(byteBuffer, ref positionCounter);
            YIntegral = decodeFloat(byteBuffer, ref positionCounter);
            YProportional = decodeFloat(byteBuffer, ref positionCounter);
            YDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            LateralControl = decodeFloat(byteBuffer, ref positionCounter);
            YIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            YDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            YProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            YAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            LateralInnerLoopGain = decodeFloat(byteBuffer, ref positionCounter);
            YLateralOuterLoopSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            RollAngularVelocityGain = decodeFloat(byteBuffer, ref positionCounter);

            AltitudeMetersAgl = decodeFloat(byteBuffer, ref positionCounter);
            ZVelocityFRDCms = decodeFloat(byteBuffer, ref positionCounter);
            YawRads = decodeFloat(byteBuffer, ref positionCounter);
            ZIntegral = decodeFloat(byteBuffer, ref positionCounter);
            ZProportional = decodeFloat(byteBuffer, ref positionCounter);
            ZDerivativeError = decodeFloat(byteBuffer, ref positionCounter);
            MainRotorCollectiveControl = decodeFloat(byteBuffer, ref positionCounter);
            ZIntegralGain = decodeFloat(byteBuffer, ref positionCounter);
            ZDerivativeGain = decodeFloat(byteBuffer, ref positionCounter);
            ZProportionalGain = decodeFloat(byteBuffer, ref positionCounter);
            ZAntiWindupGain = decodeFloat(byteBuffer, ref positionCounter);
            ZNEDLocalFrame = decodeFloat(byteBuffer, ref positionCounter);

            LatitudeDegrees = decodeFloat(byteBuffer, ref positionCounter);
            LongitudeDegrees = decodeFloat(byteBuffer, ref positionCounter);



            XAccelFrdMss = decodeFloat(byteBuffer, ref positionCounter);
            YAccelFrdMss = decodeFloat(byteBuffer, ref positionCounter);
            ZAccelFrdMss = decodeFloat(byteBuffer, ref positionCounter);
            YawAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            PitchAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            RollAngularVelocityRadsPerSecond = decodeFloat(byteBuffer, ref positionCounter);
            XMagFrd = decodeFloat(byteBuffer, ref positionCounter);
            YMagFrd = decodeFloat(byteBuffer, ref positionCounter);
            ZMagFrd = decodeFloat(byteBuffer, ref positionCounter);
            XEcefCm = decodeInt(byteBuffer, ref positionCounter);
            YEcefCm = decodeInt(byteBuffer, ref positionCounter);
            ZEcefCm = decodeInt(byteBuffer, ref positionCounter);
            XVEcefCms = decodeInt(byteBuffer, ref positionCounter);
            YVEcefCms = decodeInt(byteBuffer, ref positionCounter);
            ZVEcefCms = decodeInt(byteBuffer, ref positionCounter);
            PressureMillibars = decodeFloat(byteBuffer, ref positionCounter);



            XRefSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            YRefSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            ZRefSetpoint = decodeFloat(byteBuffer, ref positionCounter);
            YawRefSetpoint = decodeFloat(byteBuffer, ref positionCounter);



            Timeouts = decodeInt(byteBuffer, ref positionCounter);
            UnrecognizedMsgTypes = decodeInt(byteBuffer, ref positionCounter);
            ChecksumErrors = decodeInt(byteBuffer, ref positionCounter);
            NumOfBlownFrames = decodeInt(byteBuffer, ref positionCounter);
            SerialCommunicationBufferOverruns = decodeInt(byteBuffer, ref positionCounter);
        }


        public override byte[] GetRawBytes()
        {
            byte[] rawMsg = new byte[NumOfBytesInMsg];

            int positionCounter = 0;

            encode(ref rawMsg, MsgType, ref positionCounter);
            encode(ref rawMsg, YawIntegral, ref positionCounter);
            encode(ref rawMsg, YawProportional, ref positionCounter);
            encode(ref rawMsg, YawDerivativeError, ref positionCounter);
            encode(ref rawMsg, YawControl, ref positionCounter);
            encode(ref rawMsg, YawIntegralGain, ref positionCounter);
            encode(ref rawMsg, YawDerivativeGain, ref positionCounter);
            encode(ref rawMsg, YawProportionalGain, ref positionCounter);
            encode(ref rawMsg, YawAntiWindupGain, ref positionCounter);



            encode(ref rawMsg, XNEDLocalFrame, ref positionCounter);
            encode(ref rawMsg, XVelocityFRDCms, ref positionCounter);
            encode(ref rawMsg, PitchRads, ref positionCounter);
            encode(ref rawMsg, XIntegral, ref positionCounter);
            encode(ref rawMsg, XProportional, ref positionCounter);
            encode(ref rawMsg, XDerivativeError, ref positionCounter);
            encode(ref rawMsg, LongitudeControl, ref positionCounter);
            encode(ref rawMsg, XIntegralGain, ref positionCounter);
            encode(ref rawMsg, XDerivativeGain, ref positionCounter);
            encode(ref rawMsg, XProportionalGain, ref positionCounter);
            encode(ref rawMsg, XAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, LongitudeInnerLoopGain, ref positionCounter);
            encode(ref rawMsg, XLongitudeOuterLoopSetpoint, ref positionCounter);
            encode(ref rawMsg, PitchAngularVelocityGain, ref positionCounter);

            encode(ref rawMsg, YNEDLocalFrame, ref positionCounter);
            encode(ref rawMsg, YVelocityFRDCms, ref positionCounter);
            encode(ref rawMsg, RollRads, ref positionCounter);
            encode(ref rawMsg, YIntegral, ref positionCounter);
            encode(ref rawMsg, YProportional, ref positionCounter);
            encode(ref rawMsg, YDerivativeError, ref positionCounter);
            encode(ref rawMsg, LateralControl, ref positionCounter);
            encode(ref rawMsg, YIntegralGain, ref positionCounter);
            encode(ref rawMsg, YDerivativeGain, ref positionCounter);
            encode(ref rawMsg, YProportionalGain, ref positionCounter);
            encode(ref rawMsg, YAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, LateralInnerLoopGain, ref positionCounter);
            encode(ref rawMsg, YLateralOuterLoopSetpoint, ref positionCounter);
            encode(ref rawMsg, RollAngularVelocityGain, ref positionCounter);


            encode(ref rawMsg, AltitudeMetersAgl, ref positionCounter);
            encode(ref rawMsg, ZVelocityFRDCms, ref positionCounter);
            encode(ref rawMsg, YawRads, ref positionCounter);
            encode(ref rawMsg, ZIntegral, ref positionCounter);
            encode(ref rawMsg, ZProportional, ref positionCounter);
            encode(ref rawMsg, ZDerivativeError, ref positionCounter);
            encode(ref rawMsg, MainRotorCollectiveControl, ref positionCounter);
            encode(ref rawMsg, ZIntegralGain, ref positionCounter);
            encode(ref rawMsg, ZDerivativeGain, ref positionCounter);
            encode(ref rawMsg, ZProportionalGain, ref positionCounter);
            encode(ref rawMsg, ZAntiWindupGain, ref positionCounter);
            encode(ref rawMsg, ZNEDLocalFrame, ref positionCounter);

            encode(ref rawMsg, LatitudeDegrees, ref positionCounter);
            encode(ref rawMsg, LongitudeDegrees, ref positionCounter);





            encode(ref rawMsg,  XAccelFrdMss, ref positionCounter);
            encode(ref rawMsg,  YAccelFrdMss, ref positionCounter);
            encode(ref rawMsg,  ZAccelFrdMss, ref positionCounter);
            encode(ref rawMsg,  YawAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg,  PitchAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg,  RollAngularVelocityRadsPerSecond, ref positionCounter);
            encode(ref rawMsg,  XMagFrd, ref positionCounter);
            encode(ref rawMsg,  YMagFrd, ref positionCounter);
            encode(ref rawMsg,  ZMagFrd, ref positionCounter);
            encode(ref rawMsg,  XEcefCm, ref positionCounter);
            encode(ref rawMsg,  YEcefCm, ref positionCounter);
            encode(ref rawMsg,  ZEcefCm, ref positionCounter);
            encode(ref rawMsg,  XVEcefCms, ref positionCounter);
            encode(ref rawMsg,  YVEcefCms, ref positionCounter);
            encode(ref rawMsg,  ZVEcefCms, ref positionCounter);
            encode(ref rawMsg,  PressureMillibars, ref positionCounter);




            encode(ref rawMsg, XRefSetpoint, ref positionCounter);
            encode(ref rawMsg, YRefSetpoint, ref positionCounter);
            encode(ref rawMsg, ZRefSetpoint, ref positionCounter);
            encode(ref rawMsg, YawRefSetpoint, ref positionCounter);




            encode(ref rawMsg, Timeouts, ref positionCounter);
            encode(ref rawMsg, UnrecognizedMsgTypes, ref positionCounter);
            encode(ref rawMsg, ChecksumErrors, ref positionCounter);
            encode(ref rawMsg, NumOfBlownFrames, ref positionCounter);
            encode(ref rawMsg, SerialCommunicationBufferOverruns, ref positionCounter);

            return rawMsg;
        }

        public void UpdateModel(GroundControlStationModel model)
        {
            model.YawIntegral = YawIntegral;
            model.YawProportional = YawProportional;
            model.YawDerivativeError = YawDerivativeError;



            model.XNEDLocalFrame = XNEDLocalFrame;
            model.XVelocityMetersPerSecond = XVelocityFRDCms;
            model.ThetaPitchDegrees = PitchRads;
            model.XIntegral = XIntegral;
            model.XProportional = XProportional;
            model.XDerivativeError = XDerivativeError;
            model.LongitudeControl = LongitudeControl * -1;
            model.XLongitudeOuterLoopSetpoint = XLongitudeOuterLoopSetpoint;
            model.PitchAngularVelocityRadsPerSecond = PitchAngularVelocityRadsPerSecond;
            //model.XIntegralGain = XIntegralGain;
            //model.XDerivativeGain = XDerivativeGain;
            //model.XProportionalGain = XProportionalGain;
            //model.XAntiWindupGain = XAntiWindupGain;
            //model.LongitudeInnerLoopGain = LongitudeInnerLoopGain;

            model.YNEDLocalFrame = YNEDLocalFrame;
            model.YVelocityMetersPerSecond = YVelocityFRDCms;
            model.PhiRollDegrees = RollRads;
            model.YIntegral = YIntegral;
            model.YProportional = YProportional;
            model.YDerivativeError = YDerivativeError;
            model.LateralControl = LateralControl;
            model.YLateralOuterLoopSetpoint = YLateralOuterLoopSetpoint;
            model.RollAngularVelocityRadsPerSecond = RollAngularVelocityRadsPerSecond;
            //model.YIntegralGain = YIntegralGain;
            //model.YDerivativeGain = YDerivativeGain;
            //model.YProportionalGain = YProportionalGain;
            //model.YAntiWindupGain = YAntiWindupGain;
            //model.LateralInnerLoopGain = LateralInnerLoopGain;



            model.AltitudeMetersAgl = AltitudeMetersAgl;
            model.ZVelocityFeetPerSecond = ZVelocityFRDCms;
            model.YawRads = YawRads;
            model.ZIntegral = ZIntegral;
            model.ZProportional = ZProportional;
            model.ZDerivativeError = ZDerivativeError;
            model.MainRotorCollectiveControl = MainRotorCollectiveControl;
            model.ZNEDLocalFrame = ZNEDLocalFrame;
            //model.ZIntegralGain = ZIntegralGain;
            //model.ZDerivativeGain = ZDerivativeGain;
            //model.ZProportionalGain = ZProportionalGain;
            //model.ZAntiWindupGain = ZAntiWindupGain;

            model.YawControl = YawControl * -1;





            model.XAccelFrdMss = XAccelFrdMss;
            model.YAccelFrdMss = YAccelFrdMss;
            model.ZAccelFrdMss = ZAccelFrdMss;
            model.YawVelocityRadsPerSecond = YawAngularVelocityRadsPerSecond;
            model.PitchAngularVelocityRadsPerSecond = PitchAngularVelocityRadsPerSecond;
            model.RollAngularVelocityRadsPerSecond = RollAngularVelocityRadsPerSecond;
            model.XMagFrd = XMagFrd;
            model.YMagFrd = YMagFrd;
            model.ZMagFrd = ZMagFrd;
            model.XEcefCm = XEcefCm;
            model.YEcefCm = YEcefCm;
            model.ZEcefCm = ZEcefCm;
            model.XVEcefCms = XVEcefCms;
            model.YVEcefCms = YVEcefCms;
            model.ZVEcefCms = ZVEcefCms;
            model.PressureMillibars = PressureMillibars;

            model.PitchRads = PitchRads;
            model.RollRads = RollRads;

            /*
            model.XRefSetpoint = XRefSetpoint;
            model.YRefSetpoint = YRefSetpoint;
            model.ZRefSetpoint = ZRefSetpoint;
            model.YawRefSetpoint = YawRefSetpoint;
            */

            model.Timeouts = Timeouts;
            model.UnrecognizedMsgTypes = UnrecognizedMsgTypes;
            model.ChecksumErrors = ChecksumErrors;
            model.NumOfBlownFrames = NumOfBlownFrames;
            model.SerialCommunicationBufferOverruns = SerialCommunicationBufferOverruns;
        }







        static int xvel = 0;
        static int yvel = 0;
        static int zvel = 0;
        static int xecef = 0;
        static int yecef = 0;
        static int zecef = 0;
        static int counter = 0;

        static DateTime startTime = DateTime.Now;
        static int counter2 = 0;

        public static FlightComputerTelemetryMessage CreateFromModel(GroundControlStationModel model)
        {
            FlightComputerTelemetryMessage msg = new FlightComputerTelemetryMessage();

            msg.YawIntegralGain = model.YawIntegralGain;
            msg.YawDerivativeGain = model.YawDerivativeGain;
            msg.YawProportionalGain = model.YawProportionalGain;
            msg.YawAntiWindupGain = model.YawAntiWindupGain;

            msg.XIntegralGain = model.XIntegralGain;
            msg.XDerivativeGain = model.XDerivativeGain;
            msg.XProportionalGain = model.XProportionalGain;
            msg.XAntiWindupGain = model.XAntiWindupGain;
            msg.LongitudeInnerLoopGain = model.LongitudeInnerLoopGain;
            msg.PitchAngularVelocityRadsPerSecond = model.PitchAngularVelocityRadsPerSecond;
            msg.PitchAngularVelocityGain = model.PitchAngularVelocityGain;

            msg.YIntegralGain = model.YIntegralGain;
            msg.YDerivativeGain = model.YDerivativeGain;
            msg.YProportionalGain = model.YProportionalGain;
            msg.YAntiWindupGain = model.YAntiWindupGain;
            msg.LateralInnerLoopGain = model.LateralInnerLoopGain;
            msg.RollAngularVelocityRadsPerSecond = model.RollAngularVelocityRadsPerSecond;
            msg.RollAngularVelocityGain = model.RollAngularVelocityGain;

            msg.ZIntegralGain = model.ZIntegralGain;
            msg.ZDerivativeGain = model.ZDerivativeGain;
            msg.ZProportionalGain = model.ZProportionalGain;
            msg.ZAntiWindupGain = model.ZAntiWindupGain;

            msg.LatitudeDegrees = model.SimTelm.LatitudeDegrees;
            msg.LongitudeDegrees = model.SimTelm.LongitudeDegrees;
      //      msg.AltitudeMetersAgl = model.SimTelm.ZAltitudeFtAgl * .3048f; //Removed since it should be calculated from pressure


            //http://stackoverflow.com/questions/1568568/how-to-convert-euler-angles-to-directional-vector
            float yawRads = Util.Util.ConvertToRads(model.SimTelm.MagHeadingDegrees);
            float pitchRads = Util.Util.ConvertToRads(model.SimTelm.PitchDegrees);
            float rollRads = Util.Util.ConvertToRads(model.SimTelm.RollDegrees);



            //Multiply by 100 to convert to centimeters.
            float[] velocitiesNEDCms = { model.SimTelm.XVelocityNEDMs * 100, model.SimTelm.YVelocityNEDMs * 100, model.SimTelm.ZVelocityNEDMs * 100};
            float[,] nedToFRDRotationMatrix = Util.Util.CreateRotationMatrixTransposed(rollRads, pitchRads, yawRads);
            float[] velocityFRDBodyFrameCms = Util.Util.RotateMatrix(nedToFRDRotationMatrix, velocitiesNEDCms);


            /*
            data.YVelocityFRDBodyFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0); //x value from simulator is y value.
            data.ZVelocityFRDBodyFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0) * -1; //Multiply by -1 to convert to down positive. Y value from simulator *-1 is Z
            data.XVelocityFRDBodyFrameMs = BitConverter.ToSingle(byteReader.ReadBytes(4), 0) * -1; //Multiply by -1 to convert to north positive. Z value from simualtor * -1 is X
            */
            //model.SimTelm.XVelocityFRDBodyFrameMs = velocityFRDBodyFrameMs[0]; //x value from simulator is y value.
            //model.SimTelm.YVelocityFRDBodyFrameMs = velocityFRDBodyFrameMs[1]; //Multiply by -1 to convert to down positive. Y value from simulator *-1 is Z
            //model.SimTelm.ZVelocityFRDBodyFrameMs = velocityFRDBodyFrameMs[2]; //Multiply by -1 to convert to north positive. Z value from simualtor * -1 is X
         
                

            msg.XVelocityFRDCms = velocityFRDBodyFrameCms[0];
            msg.YVelocityFRDCms = velocityFRDBodyFrameCms[1];
            msg.ZVelocityFRDCms = velocityFRDBodyFrameCms[2];


            //****** Remove these since these should be calculated.            
            //msg.PitchRads = Util.Util.ConvertToRads(model.SimTelm.PitchDegrees);
            //msg.RollRads = Util.Util.ConvertToRads(model.SimTelm.RollDegrees);


            /**
              * Calculate fake sensor readings from data from simulator.
              */

            //Take the yaw, pitch, and roll, to create a gravitational vector
            //then add linear acceleration to it


            //float[,] rotationMatrix = Util.Util.CreateRotationMatrixTransposed(rollRads,pitchRads,yawRads);
            //float[] rotatedUnitVector = Util.Util.RotateMatrix(nedToFRDRotationMatrix, new float[3] { 0, 0, 1 });
            //float[] rotatedUnitVector = Util.Util.RotateMatrix(nedToFRDRotationMatrix, new float[3] { 0, 0, -1 });//mostrecent 11/10/2014

            //I changed this back to be 1 because now the gravitational vector is in FRD frame, not the acceleration vector. 
            float[] rotatedUnitVector = Util.Util.RotateMatrix(nedToFRDRotationMatrix, new float[3] { 0, 0, 1 });

            float xGrav = rotatedUnitVector[0] * Util.Util.GRAVITY;
            float yGrav = rotatedUnitVector[1] * Util.Util.GRAVITY;
            float zGrav = rotatedUnitVector[2] * Util.Util.GRAVITY;

            /**
             * Add linear acceleration to grav vectors
             */
            float linearAccelXNEDMss = (velocitiesNEDCms[0]/100f - model.PreviousXVelocityNEDCms/100f) / Util.Util.INTERVAL_BETWEEN_SIM_DATA;
            float linearAccelYNEDMss = (velocitiesNEDCms[1]/100f - model.PreviousYVelocityNEDCms/100f) / Util.Util.INTERVAL_BETWEEN_SIM_DATA;
            float linearAccelZNEDMss = (velocitiesNEDCms[2]/100f - model.PreviousZVelocityNEDCms/100f) / Util.Util.INTERVAL_BETWEEN_SIM_DATA;


            


            //Rotate linear acceleration from NED into body frame in meters per second per second (because gravity is ms^2
            float[] linearAccelerationFRDBodyFrameMss = Util.Util.RotateMatrix(nedToFRDRotationMatrix, new float[] { linearAccelXNEDMss, linearAccelYNEDMss, linearAccelZNEDMss });

            float[] bodyFrameVelocities = Util.Util.RotateMatrix(nedToFRDRotationMatrix, velocitiesNEDCms);
/*
System.Diagnostics.Debug.WriteLine("XLinearAccel: " + linearAccelerationFRDBodyFrameMss[0]);
System.Diagnostics.Debug.WriteLine("YLinearAccel: " + linearAccelerationFRDBodyFrameMss[1]);
System.Diagnostics.Debug.WriteLine("ZLinearAccel: " + linearAccelerationFRDBodyFrameMss[2]);
*/
//System.Diagnostics.Debug.WriteLine("ygrav before: " + yGrav + ", adjustment " + linearAccelerationFRDBodyFrameMss[1]);  
          
            //Subtract out linear acceleration because in FRD the gravitational vector is what is FRD, not acceleration due to vehicle motion.
            xGrav -= linearAccelerationFRDBodyFrameMss[0];
            yGrav -= linearAccelerationFRDBodyFrameMss[1];
            zGrav -= linearAccelerationFRDBodyFrameMss[2];
            /*xGrav += linearAccelerationFRDBodyFrameMss[0];
            //yGrav += linearAccelerationFRDBodyFrameMss[1];
            yGrav += linearAccelerationFRDBodyFrameMss[1];
            zGrav += linearAccelerationFRDBodyFrameMss[2];*/
            
            
            //System.Diagnostics.Debug.WriteLine("XLinearAccel: " + xGrav + ", YLinearAccel: " + yGrav + ", ZLinearAccel: " + zGrav + ", Xvel " + bodyFrameVelocities[0] + ", yvel " + bodyFrameVelocities[1] + ", zvel " + bodyFrameVelocities[2]);
            //System.Diagnostics.Debug.WriteLine("YLinearAccel: " + yGrav);
            //System.Diagnostics.Debug.WriteLine("ZLinearAccel: " + zGrav);
           
            /*System.Diagnostics.Debug.WriteLine("XLinearV: " + msg.XVelocityFRDCms);
            System.Diagnostics.Debug.WriteLine("YLinearV: " + msg.YVelocityFRDCms);
            System.Diagnostics.Debug.WriteLine("ZLinearV: " + msg.ZVelocityFRDCms);*/
            /*
            xGrav += (velocitiesNEDCms[0] - model.PreviousXVelocityNEDCms) * (1 / Util.Util.INTERVAL_BETWEEN_SIM_DATA);
            yGrav += (velocitiesNEDCms[1] - model.PreviousYVelocityNEDCms) * (1 / Util.Util.INTERVAL_BETWEEN_SIM_DATA);
            zGrav += (velocitiesNEDCms[2] - model.PreviousZVelocityNEDCms) * (1 / Util.Util.INTERVAL_BETWEEN_SIM_DATA);
             * */


            model.PreviousXVelocityNEDCms = velocitiesNEDCms[0];
            model.PreviousYVelocityNEDCms = velocitiesNEDCms[1];
            model.PreviousZVelocityNEDCms = velocitiesNEDCms[2];




            /**
             * Calculate magnetic vector including the angle of inclination
             */
            float[,] magFrameRotationMatrix = Util.Util.CreateRotationMatrix(0, Util.Util.ConvertToRads(70), 0);
            float[] magFrameRotatedUnitVector = Util.Util.RotateMatrix(magFrameRotationMatrix, new float[3] { 1, 0, 0 });

            //multiply the Z axis by -1 because the rotation rotates it 'down' correctly, but 'down' is technically positive in NED frame. 
            magFrameRotatedUnitVector[2] *= -1;

            float[,] magRotationMatrix = Util.Util.CreateRotationMatrixTransposed(rollRads, pitchRads, yawRads);
            float[] magRotatedUnitVector = Util.Util.RotateMatrix(magRotationMatrix, magFrameRotatedUnitVector);

            
            /**
             * Calculate position in ECEF based on lat
             */
            float ecefX = 0;
            float ecefY = 0;
            float ecefZ = 0;

            //Convert lat/long to earth centered earth fixed coordinates. Convert altitude to meters.
            Util.Util.ConvertFromGeodeticToECEF(model.SimTelm.LatitudeDegrees, model.SimTelm.LongitudeDegrees, model.SimTelm.ZAltitudeFtAgl * 0.3048f, out ecefX, out ecefY, out ecefZ);

            /**
             * Convert velocity from Body frame to NED to ECEF
             */
            //float[,] rotationMatrixVelocity = Util.Util.CreateRotationMatrix(rollRads, pitchRads, yawRads);
            //float[] velocityRotatedVectorNED = Util.Util.RotateMatrix(rotationMatrixVelocity, new float[] { model.SimTelm.XVelocityFRDBodyFrameMs, model.SimTelm.YVelocityFRDBodyFrameMs, model.SimTelm.ZVelocityFRDBodyFrameMs});

            float[,] rotationMatrixVelocityECEF = Util.Util.CalculateECEFToLocalNEDRotationMatrixTransposed(model.SimTelm.LatitudeDegrees, model.SimTelm.LongitudeDegrees);
            float[] velocityRotatedVectorECEFCms = Util.Util.RotateMatrix(rotationMatrixVelocityECEF, velocitiesNEDCms);

/*
float[] velocityRotatedVectorECEF2 = velocityRotatedVectorECEF;
float[,] rotationMatrixVelocityNED2 = Util.Util.CalculateECEFToLocalNEDRotationMatrix(model.SimTelm.LatitudeDegrees, model.SimTelm.LongitudeDegrees);
float[] velocityRotatedVectorNED2 = Util.Util.RotateMatrix(rotationMatrixVelocityNED2, velocityRotatedVectorECEF2);
float[,] rotationMatrixVelocity2 = Util.Util.CreateRotationMatrixTransposed(rollRads, pitchRads, yawRads);
float[] velocityRotatedVectorBody = Util.Util.RotateMatrix(rotationMatrixVelocity2, new float[] { velocityRotatedVectorNED2[0], velocityRotatedVectorNED2[1], velocityRotatedVectorNED2[2]});


System.Diagnostics.Debug.WriteLine("FRD x from sim: " + model.SimTelm.XVelocityFRDBodyFrameMs);
System.Diagnostics.Debug.WriteLine("FRD x from sim recalced: " + velocityRotatedVectorBody[0]);
*/


            /**
             * Convert altitude to pressure
             */
            //equation from http://www.engineeringtoolbox.com/air-altitude-pressure-d_462.html
            //Divide by 100 to convert from Pa to Mb
            float pressureMb = (101325 * (float) Math.Pow(1 - .0000225577 * (model.SimTelm.ZAltitudeFtMsl * .3048), 5.25588)) / 100;

            //equation from http://www.regentsprep.org/Regents/math/algtrig/ATP8b/exponentialResource.htm
        //    float altitudeKmMsl = (float) model.SimTelm.ZAltitudeFtMsl / 3280.84f;
        //    float pressureMb = (float)(1.0 * Math.Exp((float)-1 * ((altitudeKmMsl) / 7))) * 1000;

            //root (((altitude / (288.15 / (6.5 / 1000.0))) - 1), (6.5 / 1000.0) * (287.052 / 9.78)))

            msg.XAccelFrdMss = xGrav;
            msg.YAccelFrdMss = yGrav;
            msg.ZAccelFrdMss = zGrav;
            msg.YawAngularVelocityRadsPerSecond = model.SimTelm.YawVelocityRadsPerS;
            msg.PitchAngularVelocityRadsPerSecond = model.SimTelm.PitchVelocityRadsPerS;
            msg.RollAngularVelocityRadsPerSecond = model.SimTelm.RollVelocityRadsPerS;
            msg.XMagFrd = magRotatedUnitVector[0];
            msg.YMagFrd = magRotatedUnitVector[1];
            msg.ZMagFrd = magRotatedUnitVector[2];
                                    
                /*
                msg.XEcefCm = (int)(ecefX * 100.0f);
                msg.YEcefCm = (int)(ecefY * 100.0f);
                msg.ZEcefCm = (int)(ecefZ * 100.0f);
                msg.XVEcefCms = (int)(velocityRotatedVectorECEFCms[0]);
                msg.YVEcefCms = (int)(velocityRotatedVectorECEFCms[1]);
                msg.ZVEcefCms = (int)(velocityRotatedVectorECEFCms[2]);
                 */
            msg.PressureMillibars = pressureMb;

            //four times a second, fully update the position and velocity values.
            //techincally this is based on how frequently the FC reads data, so if that changes, intervalbetweensimdata would need to change to something other than that constant. but for now, they are the same.
            if (counter == 0)
            {
                xvel = (int)(velocityRotatedVectorECEFCms[0]);
                yvel = (int)(velocityRotatedVectorECEFCms[1]);
                zvel = (int)(velocityRotatedVectorECEFCms[2]);
                xecef = (int)(ecefX * 100.0f);
                yecef = (int)(ecefY * 100.0f);
                zecef = (int)(ecefZ * 100.0f);

                counter++;
                counter2++;
                //System.Diagnostics.Debug.WriteLine("here :");
            }
            //else if (counter > ((1 / Util.Util.INTERVAL_BETWEEN_SIM_DATA) / 4))
            else if (counter > 5)
            {
                counter = 0;
            }
            else
            {
                counter++;
            }

            TimeSpan seconds = DateTime.Now.Subtract(startTime);

            if (seconds.Seconds >= 1)
            {
           //     System.Diagnostics.Debug.WriteLine("counter2 : " + counter2 + " Seconds, " + seconds.TotalMilliseconds);
                counter2 = 0;
                startTime = DateTime.Now;
            }

            msg.XEcefCm = xecef;
            msg.YEcefCm = yecef;
            msg.ZEcefCm = zecef;
            msg.XVEcefCms = xvel;
            msg.YVEcefCms = yvel;
            msg.ZVEcefCms = zvel;




            msg.XRefSetpoint = model.XRefSetpoint;
            msg.YRefSetpoint = model.YRefSetpoint;
            msg.ZRefSetpoint = model.ZRefSetpoint;
            msg.YawRefSetpoint = model.YawRefSetpoint * ((float)Math.PI / (float)180);

            return msg;
        }

    }
}

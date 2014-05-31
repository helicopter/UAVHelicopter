using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;
using GroundControlStation.Views;
using GroundControlStation.Messages;
using System.IO;
using GroundControlStation.Util;
using System.Globalization;

namespace GroundControlStation.Controller
{
    public class GroundControlStationController
    {
        private const String LogFilePath = @"C:\Heli\GitRepository\UAVHelicopter\CodeProjects\GroundControlStation\logs";

        private bool isSimThreadRunning = false;

        private bool isFlightComputerThreadRunning = false;

        private SimulatorInterface xplaneInterface;

        private FlightComputerInterface fcInterface;

        private Thread simThread;

        private Thread flightComputerThread;

        private StreamWriter flightComputerFileLogger;

        private StreamWriter simulatorFileLogger;

        public GroundControlStationModel Model { get; set; }

        public GroundControlStationForm View { get; set; }

        public enum DATATOSEND { ALL, SENSORDATA, GAINSDATA };

        public DATATOSEND dataToSend;

        public GroundControlStationController(SimulatorInterface xplaneInterface, FlightComputerInterface fcInterface, DATATOSEND dataToSend, int realFlight)
        {
            this.xplaneInterface = xplaneInterface;
            this.fcInterface = fcInterface;

            simThread = new Thread(new ThreadStart(BeginSimPolling));
            flightComputerThread = new Thread(new ThreadStart(BeginFlightComputerPolling));
            flightComputerThread.Priority = ThreadPriority.Highest;

            flightComputerFileLogger = new StreamWriter(Path.Combine(LogFilePath, "FlightComputerLog" + DateTime.Now.ToString("_MM_dd_yyyy_hhmmss") + ".csv"));

            if (realFlight == 1)
            {

                flightComputerFileLogger.WriteLine("DateTime," + LoggingUtil.ToCsvHeader(",", new SimpleTelemetryMessage()));
            }
            else
            {
                flightComputerFileLogger.WriteLine("DateTime," + LoggingUtil.ToCsvHeader(",", new FlightComputerTelemetryMessage()));
            }


            simulatorFileLogger = new StreamWriter(Path.Combine(LogFilePath, "SimulatorLog" + DateTime.Now.ToString("_MM_dd_yyyy_hhmmss") + ".csv"));
            simulatorFileLogger.WriteLine("DateTime," + LoggingUtil.ToCsvHeader(",", new SimulatorTelemetry()));

            this.dataToSend = dataToSend;
        }


        /// <summary>
        /// Updates the views (graphs) based on the received data.
        /// </summary>
        public void UpdateViews()
        {
            UpdateView(View.SimHeadingGraph, Model.SimTelm.MagHeadingDegrees);
            //UpdateView(View.FcMagYaw, Model.MagYaw);
            UpdateView(View.YawVelocityDegreesPerSecond, Model.YawVelocityRadsPerSecond);
            UpdateView(View.YawProportional, Model.YawProportional);
            UpdateView(View.YawIntegral, Model.YawIntegral);
            UpdateView(View.YawDerivative, Model.YawDerivativeError);
            UpdateView(View.YawControl, Model.YawControl);

            UpdateView(View.XProportional, Model.XProportional);
            UpdateView(View.XIntegral, Model.XIntegral);
            UpdateView(View.XDerivative, Model.XDerivativeError);
            UpdateView(View.XControl, Model.LongitudeControl);

            UpdateView(View.YProportional, Model.YProportional);
            UpdateView(View.YIntegral, Model.YIntegral);
            UpdateView(View.YDerivative, Model.YDerivativeError);
            UpdateView(View.YControl, Model.LateralControl);



            UpdateView(View.ZNED, Model.ZNEDLocalFrame);
            UpdateView(View.ZProportional, Model.ZProportional);
            UpdateView(View.ZDerivative, Model.ZDerivativeError);
            UpdateView(View.ZIntegral, Model.ZIntegral);

            UpdateView(View.Pitch, Model.PitchRads * (180/Math.PI));
            UpdateView(View.Roll, Model.RollRads * (180 / Math.PI));



            //UpdateView(View.FcMagYaw, Model.
            UpdateView(View.FcMagYaw, (float)Model.YawRads * (180.0f / Math.PI));
            UpdateView(View.XNED, Model.XNEDLocalFrame);
            UpdateView(View.YNED, Model.YNEDLocalFrame);
            UpdateView(View.XVEL, Model.XVelocityMetersPerSecond);
            UpdateView(View.YVEL, Model.YVelocityMetersPerSecond);
            UpdateView(View.ZVEL, Model.ZVelocityFeetPerSecond);

            UpdateLatestValues();
        }

        private void UpdateLatestValues()
        {
            List<Tuple<String, String>> simValues = Model.SimTelm.ListValues();
            simValues.AddRange(Model.ListValues());

     //       Debug.WriteLine(Model.NumOfBlownFrames);
            View.UpdateLatestValues(simValues);
        }

        private void UpdateView(IGraphingView view, double valueToGraph)
        {
            if (view.IsActive)
            {
                view.AddValueToGraph(valueToGraph);
            }
        }

        public void Start()
        {
            isSimThreadRunning = true;
            isFlightComputerThreadRunning = true;

            xplaneInterface.Open();

            simThread.Start();

            flightComputerThread.Start();
        }

        public void Stop()
        {
            isSimThreadRunning = false;
            isFlightComputerThreadRunning = false;

            simThread.Abort();
            flightComputerThread.Abort();

            simThread.Join();
            flightComputerThread.Join();

            xplaneInterface.Close();
        }

        //Polls for new simulator data and updates the model.
        private void BeginSimPolling()
        {
            while (isSimThreadRunning)
            {
                Model.SimTelm = xplaneInterface.Receive();
                simulatorFileLogger.WriteLine(DateTime.Now.ToString("hh.mm.ss.ffffff") + ", " + LoggingUtil.ToCsv(",", Model.SimTelm));
            }
        }

        DateTime startTime = DateTime.Now;
        //int counter = 0;


        //Receives data from the flight computer.
        private void BeginFlightComputerPolling()
        {
            while (isFlightComputerThreadRunning)
            {

                Message msg = fcInterface.Receive();

                if (msg != null)
                {
                    if (msg.MsgType == FlightComputerTelemetryMessage.MessageType)
                    {
                        //Receive model data from FC
                        FlightComputerTelemetryMessage telem = (FlightComputerTelemetryMessage) msg;


//                        System.Diagnostics.Debug.WriteLine("collective control: " + telem.MainRotorCollectiveControl + ", lat control: " + telem.LateralControl + ", long control: " + telem.LongitudeControl + ", Rudder control: " + telem.YawControl);


                        telem.UpdateModel(Model);

                        UpdateViews();

                        //Scale the Yaw Control value to a value usable by xplane.
                        /**
                         * Rescale yaw control to appropriate values. 
                         * from -1/1 to -10/10
                         * new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
                         */
                        Model.YawControl = (10 - -10) / (1 - -1) * (Model.YawControl - -1) + -10;
                        Model.MainRotorCollectiveControl = (10 - -10) / (1 - -1) * (Model.MainRotorCollectiveControl - -1) + -10;
                        if (Model.LateralControl > .8) Model.LateralControl = .8f;
                        if (Model.LateralControl < -.8) Model.LateralControl = -.8f;
                        if (Model.LongitudeControl > .8) Model.LongitudeControl = .8f;
                        if (Model.LongitudeControl < -.8) Model.LongitudeControl = -.8f;

//System.Diagnostics.Debug.WriteLine("pitch error" + (telem.PitchRads * (180 / Math.PI) - Model.SimTelm.PitchDegrees));
//System.Diagnostics.Debug.WriteLine("roll error" + (telem.RollRads * (180 / Math.PI) - Model.SimTelm.RollDegrees));

//System.Diagnostics.Debug.WriteLine("FC pitch " + (telem.PitchRads * (180 / Math.PI)) + ", sim pitch " + Model.SimTelm.PitchDegrees + ", Error " + (telem.PitchRads * (180 / Math.PI) - Model.SimTelm.PitchDegrees));
//System.Diagnostics.Debug.WriteLine("FC roll " + (telem.RollRads * (180 / Math.PI)) + ", sim roll " + Model.SimTelm.RollDegrees + ", Error " + (telem.RollRads * (180 / Math.PI) - Model.SimTelm.RollDegrees));

                        //Transmit data to simulator
                        xplaneInterface.Transmit(Model);

                        flightComputerFileLogger.WriteLine(DateTime.Now.ToString("hh.mm.ss.ffffff") + ", " + LoggingUtil.ToCsv(",", telem));
                    }
                    else if (msg.MsgType == ControlMessage.MessageType)
                    {
                        //Receive model data from FC
                        ControlMessage telem = (ControlMessage)msg;
                        Model.MainRotorCollectiveControl = telem.MainRotorCollectiveControl;
                        //Model.YawControl = telem.YawControl;
                        //Model.LongitudeControl = telem.LongitudeControl;
                        //Multiply by -1 because the control direction between the helicopter and the sim is reversed for yaw and longitude.
                        Model.YawControl = telem.YawControl * -1;
                        Model.LongitudeControl = telem.LongitudeControl * -1;
                        Model.LateralControl = telem.LateralControl;

                        UpdateViews();

                        //Scale the Yaw Control value to a value usable by xplane.
                        /**
                         * Rescale yaw control to appropriate values. 
                         * from -1/1 to -10/10
                         * new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
                         */
                        Model.YawControl = (10 - -10) / (1 - -1) * (Model.YawControl - -1) + -10;
                        Model.MainRotorCollectiveControl = (10 - -10) / (1 - -1) * (Model.MainRotorCollectiveControl - -1) + -10;
                        if (Model.LateralControl > .8) Model.LateralControl = .8f;
                        if (Model.LateralControl < -.8) Model.LateralControl = -.8f;
                        if (Model.LongitudeControl > .8) Model.LongitudeControl = .8f;
                        if (Model.LongitudeControl < -.8) Model.LongitudeControl = -.8f;

                        xplaneInterface.Transmit(Model);

                        
                    }
                    else if (msg.MsgType == SimpleTelemetryMessage.MessageType)
                    {

                        SimpleTelemetryMessage telem = (SimpleTelemetryMessage)msg;

                        Model.XNEDLocalFrame = telem.XNEDLocalFrame;
                        Model.XVelocityMetersPerSecond = telem.XVelocityFRDCms;


                        Model.YNEDLocalFrame = telem.YNEDLocalFrame;
                        Model.YVelocityMetersPerSecond = telem.YVelocityFRDCms;

                        //System.Diagnostics.Debug.WriteLine(telem.YVelocityFRDCms);
//System.Diagnostics.Debug.WriteLine("yaw: " + (telem.YawRads * (180/Math.PI)) + " xmagfrd: " + telem.XMagFrd + ", ymag: " + telem.YMagFrd + ", zmag: " + telem.ZMagFrd);
//System.Diagnostics.Debug.WriteLine( Math.Sqrt(Math.Pow(telem.XMagFrd,2)  + Math.Pow(telem.YMagFrd,2) + Math.Pow(telem.ZMagFrd,2)));


                        Model.ZVelocityFeetPerSecond = telem.ZVelocityFRDCms;
                        
                        Model.YawRads = telem.YawRads;
                        Model.ZNEDLocalFrame = telem.ZNEDLocalFrame;



                        Model.XAccelFrdMss = telem.XAccelFrdMss;
                        Model.YAccelFrdMss = telem.YAccelFrdMss;
                        Model.ZAccelFrdMss = telem.ZAccelFrdMss;
                        Model.YawVelocityRadsPerSecond = telem.YawAngularVelocityRadsPerSecond;
                        Model.PitchAngularVelocityRadsPerSecond = telem.PitchAngularVelocityRadsPerSecond;
                        Model.RollAngularVelocityRadsPerSecond = telem.RollAngularVelocityRadsPerSecond;
                        Model.XMagFrd = telem.XMagFrd;
                        Model.YMagFrd = telem.YMagFrd;
                        Model.ZMagFrd = telem.ZMagFrd;
                        Model.XEcefCm = telem.XEcefCm;
                        Model.YEcefCm = telem.YEcefCm;
                        Model.ZEcefCm = telem.ZEcefCm;
                        Model.XVEcefCms = telem.XVEcefCms;
                        Model.YVEcefCms = telem.YVEcefCms;
                        Model.ZVEcefCms = telem.ZVEcefCms;
                        Model.PressureMillibars = telem.PressureMillibars;
System.Diagnostics.Debug.WriteLine("X: " + telem.XMagFrd + " Y: " + telem.YMagFrd + " Z: " + telem.ZMagFrd);
                        Model.PitchRads = telem.PitchRads;
                        Model.RollRads = telem.RollRads;


                        Model.Timeouts = telem.Timeouts;
                        Model.UnrecognizedMsgTypes = telem.UnrecognizedMsgTypes;
                        Model.ChecksumErrors = telem.ChecksumErrors;
                        Model.NumOfBlownFrames = telem.NumOfBlownFrames;
                        Model.SerialCommunicationBufferOverruns = telem.SerialCommunicationBufferOverruns;
                        UpdateViews();


                        flightComputerFileLogger.WriteLine(DateTime.Now.ToString("hh.mm.ss.ffffff") + ", " + LoggingUtil.ToCsv(",", telem));
                    }
                    else if (msg.MsgType == SyncMessage.MessageType)
                    {
                        SyncMessage syncMsg = (SyncMessage)msg;

                        FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(Model);

                        if (syncMsg.RequestedMessage == (byte)DATATOSEND.ALL)
                        {

                            //Send sim model data to FC. 
                            fcInterface.Transmit(data);
                        }
                        else if (syncMsg.RequestedMessage == (byte)DATATOSEND.SENSORDATA)
                        {
                            SensorDataMessage sensorData = new SensorDataMessage();
                            sensorData.PitchAngularVelocityRadsPerSecond = data.PitchAngularVelocityRadsPerSecond;
                            sensorData.PressureMillibars = data.PressureMillibars;
                            sensorData.RollAngularVelocityRadsPerSecond = data.RollAngularVelocityRadsPerSecond;
                            sensorData.XAccelFrdMss = data.XAccelFrdMss;
                            sensorData.XEcefCm = data.XEcefCm;
                            sensorData.XMagFrd = data.XMagFrd;
                            sensorData.XVEcefCms = data.XVEcefCms;
                            sensorData.YAccelFrdMss = data.YAccelFrdMss;
                            sensorData.YawAngularVelocityRadsPerSecond = data.YawAngularVelocityRadsPerSecond;
                            sensorData.YEcefCm = data.YEcefCm;
                            sensorData.YMagFrd = data.YMagFrd;
                            sensorData.YVEcefCms = data.YVEcefCms;
                            sensorData.ZAccelFrdMss = data.ZAccelFrdMss;
                            sensorData.ZEcefCm = data.ZEcefCm;
                            sensorData.ZMagFrd = data.ZMagFrd;
                            sensorData.ZVEcefCms = data.ZVEcefCms;


                            fcInterface.Transmit(sensorData);
                        }
                        else if (syncMsg.RequestedMessage == (byte)DATATOSEND.GAINSDATA)
                        {
                            GainsMessage gains = new GainsMessage();
                            gains.YawIntegralGain = data.YawIntegralGain;
                            gains.YawDerivativeGain = data.YawDerivativeGain;
                            gains.YawProportionalGain = data.YawProportionalGain;
                            gains.YawAntiWindupGain = data.YawAntiWindupGain;

                            gains.XIntegralGain = data.XIntegralGain;
                            gains.XDerivativeGain = data.XDerivativeGain;
                            gains.XProportionalGain = data.XProportionalGain;
                            gains.XAntiWindupGain = data.XAntiWindupGain;
                            gains.LongitudeInnerLoopGain = data.LongitudeInnerLoopGain;
                            gains.PitchAngularVelocityGain = data.PitchAngularVelocityGain;

                            gains.YIntegralGain = data.YIntegralGain;
                            gains.YDerivativeGain = data.YDerivativeGain;
                            gains.YProportionalGain = data.YProportionalGain;
                            gains.YAntiWindupGain = data.YAntiWindupGain;
                            gains.LateralInnerLoopGain = data.LateralInnerLoopGain;
                            gains.RollAngularVelocityGain = data.RollAngularVelocityGain;

                            gains.ZIntegralGain = data.ZIntegralGain;
                            gains.ZDerivativeGain = data.ZDerivativeGain;
                            gains.ZProportionalGain = data.ZProportionalGain;
                            gains.ZAntiWindupGain = data.ZAntiWindupGain;


                            fcInterface.Transmit(gains);
                        }
                    }
                }
            }
        }

    }
}

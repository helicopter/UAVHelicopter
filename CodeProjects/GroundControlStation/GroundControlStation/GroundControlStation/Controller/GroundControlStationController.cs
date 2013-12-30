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

namespace GroundControlStation.Controller
{
    public class GroundControlStationController
    {
        private bool isSimThreadRunning = false;

        private bool isFlightComputerThreadRunning = false;

        private SimulatorInterface xplaneInterface;

        private FlightComputerInterface fcInterface;

        private Thread simThread;

        private Thread flightComputerThread;

        public GroundControlStationModel Model { get; set; }

        public GroundControlStationForm View { get; set; }


        public GroundControlStationController(SimulatorInterface xplaneInterface, FlightComputerInterface fcInterface)
        {
            this.xplaneInterface = xplaneInterface;
            this.fcInterface = fcInterface;

            simThread = new Thread(new ThreadStart(BeginSimPolling));
            flightComputerThread = new Thread(new ThreadStart(BeginFlightComputerPolling));
        }


        /// <summary>
        /// Updates the views (graphs) based on the received data.
        /// </summary>
        public void UpdateViews()
        {
            UpdateView(View.SimHeadingGraph, Model.SimTelm.MagHeadingDegrees);
            UpdateView(View.FcMagYaw, Model.MagYaw);
            UpdateView(View.YawVelocityDegreesPerSecond, Model.YawVelocityDegreesPerSecond);
            UpdateView(View.YawProportional, Model.YawProportional);
            UpdateView(View.YawIntegral, Model.YawIntegral);
            UpdateView(View.YawDerivative, Model.YawDerivativeError);
            UpdateView(View.YawControl, Model.YawControl);

            UpdateView(View.XProportional, Model.XProportional);
            UpdateView(View.XIntegral, Model.XIntegral);
            UpdateView(View.XDerivative, Model.XDerivativeError);
            UpdateView(View.XControl, Model.LongitudeControl);

            UpdateLatestValues();
        }

        private void UpdateLatestValues()
        {
            List<Tuple<String, String>> simValues = Model.SimTelm.ListValues();
            simValues.AddRange(Model.ListValues());

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
            }
        }

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

                        telem.UpdateModel(Model);
                        
                        UpdateViews();

                        //Scale the Yaw Control value to a value usable by xplane.
                        /**
                         * Rescale yaw control to appropriate values. 
                         * from -1/1 to -10/10
                         * new_v = (new_max - new_min) / (old_max - old_min) * (v - old_min) + new_min
                         */
                        float scaledYaw = (10 - -10) / (1 - -1) * (Model.YawControl - -1) + -10;

                        Model.YawControl = scaledYaw;

                        //Transmit data to simulator
                        xplaneInterface.Transmit(Model);
                    }
                    else if (msg.MsgType == SyncMessage.MessageType)
                    {
                        //Send sim model data to FC. 
                        FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(Model);

                        fcInterface.Transmit(data);
                    }
                }
            }
        }

    }
}

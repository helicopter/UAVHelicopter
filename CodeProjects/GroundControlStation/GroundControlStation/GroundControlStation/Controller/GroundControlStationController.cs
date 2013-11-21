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

        private GroundControlStationDashboardView _dashboardView;

        public GroundControlStationDashboardView DashboardView {
            get
            {
                return _dashboardView;
            }

            set
            {
                value.DashboardForm.Controller = this;
                _dashboardView = value;
            }
        }

        public GroundControlStationModel Model { get; set; }

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
            UpdateView(DashboardView.SimHeadingGraph, Model.SimTelm.MagHeadingDegrees);
            UpdateView(DashboardView.FcMagYaw, Model.MagYaw);
            UpdateView(DashboardView.YawVelocityDegreesPerSecond, Model.YawVelocityDegreesPerSecond);
            UpdateView(DashboardView.YawProportional, Model.YawProportional);
            UpdateView(DashboardView.YawIntegral, Model.YawIntegral);
            UpdateView(DashboardView.YawDerivative, Model.YawDerivativeError);
            UpdateView(DashboardView.YawControl, Model.YawControl);

            UpdateLatestValues();
        }

        private void UpdateLatestValues()
        {
            List<Tuple<String, String>> simValues = Model.SimTelm.ListValues();
            simValues.AddRange(Model.ListValues());

            DashboardView.DashboardForm.UpdateLatestValues(simValues);
        }

        private void UpdateView(IGraphingView view, double valueToGraph)
        {
            if (view.IsActive)
            {
                view.AddValueToGraph(valueToGraph);
            }
        }

        public void ActivateSimHeadingGraph()
        {
            DashboardView.SimHeadingGraph.ActivateView();
        }

        public void DeactivateSimHeadingGraph()
        {
            DashboardView.SimHeadingGraph.DeactivateView();
        }

        internal void ToggleSimHeadingGraph()
        {
            ToggleGraph(DashboardView.SimHeadingGraph);
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

        internal void ToggleFcMagYawGraph()
        {
            ToggleGraph(DashboardView.FcMagYaw);
        }

        private void ToggleGraph(IGraphingView graph)
        {
            if (graph.IsActive)
            {
                graph.DeactivateView();
            }
            else
            {
                graph.ActivateView();
            }
        }

        internal void ToggleFcYawProportionalErrorGraph()
        {
            ToggleGraph(DashboardView.YawProportional);
        }

        internal void ToggleYawIntegralGraph()
        {
            ToggleGraph(DashboardView.YawIntegral);
        }

        internal void ToggleYawDerivativeGraph()
        {
            ToggleGraph(DashboardView.YawDerivative);
        }

        internal void ToggleYawControlGraph()
        {
            ToggleGraph(DashboardView.YawControl);
        }
    }
}

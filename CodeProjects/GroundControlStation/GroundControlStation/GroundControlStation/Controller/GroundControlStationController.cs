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
                value.DashboardView.Controller = this;
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

        /*
        public void GetSimulatorTelemetry()
        {
            Model.SimTelm = xplaneInterface.Receive();
        }

        public void GetFlightComputerTelemetry()
        {
            Model.FcTelm = fcInterface.Receive();
        }
         */

        /// <summary>
        /// Updates the views (graphs) based on the received data.
        /// </summary>
        public void UpdateViews()
        {
            UpdateView(DashboardView.SimHeadingGraph, Model.SimTelm.MagHeadingDegrees);
            UpdateView(DashboardView.FcMagYaw, Model.FcTelm.MagYaw);
            UpdateView(DashboardView.YawVelocityDegreesPerSecond, Model.FcTelm.YawVelocityDegreesPerSecond);
            UpdateView(DashboardView.FcMagY, Model.FcTelm.MagY);
            UpdateView(DashboardView.FcMagZ, Model.FcTelm.MagZ);
            UpdateView(DashboardView.YawProportional, Model.FcTelm.YawProportional);

            UpdateLatestValues();
        }

        private void UpdateLatestValues()
        {
            List<Tuple<String, String>> simValues = Model.SimTelm.ListValues();
            simValues.AddRange(Model.FcTelm.ListValues());

            DashboardView.DashboardView.UpdateLatestValues(simValues);
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
                    if (msg.MsgType == FlightComputerTelemetry.MessageType)
                    {
                        //Receive model data from FC
                        FlightComputerTelemetry telem = (FlightComputerTelemetry) msg;

                        Model.FcTelm = telem;

                        UpdateViews();

                        //Transmit data to simulator
                        xplaneInterface.Transmit(telem);
                    }
                    else if (msg.MsgType == SyncMessage.MessageType)
                    {
                        //Send sim model data to FC. 
                        FlightComputerTelemetry data = new FlightComputerTelemetry();
                        data.MagYaw = (ushort) (Model.SimTelm.MagHeadingDegrees * 100);
                        data.YawVelocityDegreesPerSecond = (short) (Model.SimTelm.YawVelocityMs * 100);

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
    }
}

using System;
using System.Collections.Generic;
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

        public void GetSimulatorTelemetry()
        {
            Model.SimTelm = xplaneInterface.Receive();
        }

        public void GetFlightComputerTelemetry()
        {
            Model.FcTelm = fcInterface.Receive();
        }

        /// <summary>
        /// Updates the views based on the received data.
        /// </summary>
        public void UpdateViews()
        {
            UpdateView(DashboardView.SimHeadingGraph, Model.SimTelm.TrueHeadingDegrees);
            UpdateView(DashboardView.FcMagX, Model.FcTelm.MagX);
            UpdateView(DashboardView.FcMagY, Model.FcTelm.MagY);
            UpdateView(DashboardView.FcMagZ, Model.FcTelm.MagZ);

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
            if (DashboardView.SimHeadingGraph.IsActive)
            {
                DashboardView.SimHeadingGraph.DeactivateView();
            }
            else
            {
                DashboardView.SimHeadingGraph.ActivateView();
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

        private void BeginSimPolling()
        {
            while (isSimThreadRunning)
            {
                Model.SimTelm = xplaneInterface.Receive();

                UpdateViews();

                FlightComputerTelemetry data = new FlightComputerTelemetry();
                data.MagX = 22;
                data.MagY = 23;
                data.MagZ = 24;

                fcInterface.Transmit(data);
            }
        }

        private void BeginFlightComputerPolling()
        {
            while (isFlightComputerThreadRunning)
            {
                /*FlightComputerTelemetry data = new FlightComputerTelemetry();

                //TODO don't hard code this.
                data.MagX = 93;
                data.MagY = 93;
                data.MagZ = 93;*/

      //          SimulatorTelemetry data = new SimulatorTelemetry();
      //          data.
//                SimulatorTelemetryDataToFlightComputer data = Model.SimTelm.CreateToFlightComputerMessage();

  //              fcInterface.Transmit(data);

                Model.FcTelm = fcInterface.Receive();

                UpdateViews();
            }
        }

        internal void ToggleFcMagXGraph()
        {
            if (DashboardView.FcMagX.IsActive)
            {
                DashboardView.FcMagX.DeactivateView();
            }
            else
            {
                DashboardView.FcMagX.ActivateView();
            }
        }
    }
}

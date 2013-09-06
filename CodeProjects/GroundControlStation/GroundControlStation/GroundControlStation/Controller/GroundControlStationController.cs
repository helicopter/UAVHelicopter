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
        private SimulatorInterface xplaneInterface;
        private FlightComputerInterface fcInterface;

        private Thread controllerThread;

        private GroundControlStationView view;

        public GroundControlStationView View {
            get
            {
                return view;
            }

            set
            {
                value.DashboardView.Controller = this;
                view = value;
            }
        }

        public GroundControlStationModel Model { get; set; }

        public GroundControlStationController(SimulatorInterface xplaneInterface, FlightComputerInterface fcInterface)
        {
            this.xplaneInterface = xplaneInterface;
            this.fcInterface = fcInterface;

            controllerThread = new Thread(new ThreadStart(BeginPolling));
        }

        public void GetSimulatorTelemetry()
        {
            Model.SimTelmData = xplaneInterface.Receive();
        }

        public void GetFlightComputerTelemetry()
        {
            Model.FcTelmData = fcInterface.Receive();
        }

        /// <summary>
        /// Updates the views based on the received data.
        /// </summary>
        public void UpdateViews()
        {
            UpdateView(View.SimHeadingGraph, Model.SimTelmData.HeadingDegrees);

            UpdateLatestValues();
        }

        private void UpdateLatestValues()
        {
            Tuple<String,String> value = new Tuple<string, string>("Sim Heading", Model.SimTelmData.HeadingDegrees.ToString());

            List<Tuple<String,String>> listValues = new List<Tuple<string, string>>();
            listValues.Add(value);

            View.DashboardView.UpdateLatestValues(listValues);
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
            View.SimHeadingGraph.ActivateView();
        }

        public void DeactivateSimHeadingGraph()
        {
            View.SimHeadingGraph.InactivateView();
        }

        internal void ToggleSimHeadingGraph()
        {
            if (View.SimHeadingGraph.IsActive)
            {
                DeactivateSimHeadingGraph();
            }
            else
            {
                ActivateSimHeadingGraph();
            }
        }

        public void Start()
        {
            xplaneInterface.Open();

            controllerThread.Start();
        }

        private void BeginPolling()
        {
            while (true)
            {
                Model.SimTelmData = xplaneInterface.Receive();

                UpdateViews();
            }
        }
    }
}

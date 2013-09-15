using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Forms;
using GroundControlStation.Controller;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;
using GroundControlStation.Views;

namespace GroundControlStation
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            //TODO focus on thread ui update.
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            port.ReadTimeout = 500;

            SimulatorInterface xInterface = new SimulatorInterface(8089, 49000, IPAddress.Parse("127.0.0.255"));

            SerialPortInterface portInterface = new SerialPortInterface(port);

            FlightComputerInterface fcInterface = new FlightComputerInterface(portInterface);
            fcInterface.Open();

            IGraphingView simHeadingGraph = new GraphForm();
            IGraphingView fcMagX = new GraphForm();
            IGraphingView fcMagY = new GraphForm();
            IGraphingView fcMagZ = new GraphForm();

            IDashboardView dashboard = new GroundControlStationForm();

            GroundControlStationDashboardView gcsDashboardView = new GroundControlStationDashboardView();
            gcsDashboardView.DashboardView = dashboard;
            gcsDashboardView.SimHeadingGraph = simHeadingGraph;
            gcsDashboardView.FcMagX = fcMagX;
            gcsDashboardView.FcMagY = fcMagY;
            gcsDashboardView.FcMagZ = fcMagZ;
            


            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = new SimulatorTelemetry();
            model.FcTelm = new FlightComputerTelemetry();

            GroundControlStationController gcsController =
                new GroundControlStationController(xInterface, fcInterface);

            gcsController.DashboardView = gcsDashboardView;
            gcsController.Model = model;

            gcsController.Start();

            Application.Run((GroundControlStationForm)dashboard);

            gcsController.Stop();
        }
    }
}

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
    static class GroundControlStationMain
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

            SerialPort port = new SerialPort("COM7", 76800, Parity.None, 8, StopBits.One);
           // SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
            
 //           port.ReadTimeout = 500;

            SimulatorInterface xInterface = new SimulatorInterface(8089, 49000, IPAddress.Parse("127.0.0.255"));

            SerialPortInterface portInterface = new SerialPortInterface(port);

            FlightComputerInterface fcInterface = new FlightComputerInterface(portInterface);
            fcInterface.Open();

            

            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = new SimulatorTelemetry();

            
            /*model.YawIntegralGain = .008f;
            model.YawDerivativeGain = .85f;
            model.YawProportionalGain = 1.0f;
            model.YawAntiWindupGain = 0.1f;
            */

            //original from other program
            /*
             * model.YawIntegralGain = .5f;
            model.YawProportionalGain = 1.8471f;
            model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            model.YawAntiWindupGain = 0.1f;
             */
            model.YawIntegralGain = .05f;
            model.YawProportionalGain = .018471f;
            model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            model.YawAntiWindupGain = 0.01f;

            model.XIntegralGain = 0.0f;
            model.XProportionalGain = .289f;
            model.XDerivativeGain = 1.859f;
            model.XAntiWindupGain = 0.0f; 
            model.LongitudeInnerLoopGain = .031f;
            model.PitchAngularVelocityGain = .03f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 1.437f;
            model.YDerivativeGain = 2.8363f;
            model.YAntiWindupGain = 0.0f; 
            model.LateralInnerLoopGain = .092f;
            model.RollAngularVelocityGain = .048f;

            /*
            model.ZIntegralGain = .02f;
            model.ZProportionalGain = 1.901f;
            model.ZDerivativeGain = 1.141f;
            model.ZAntiWindupGain = .1f;
             */

            model.ZIntegralGain = .02f;
            model.ZProportionalGain = 0.05901f;
            model.ZDerivativeGain = .01141f;
            model.ZAntiWindupGain = .1f;
             

            GroundControlStationController gcsController =
                new GroundControlStationController(xInterface, fcInterface);

            gcsController.Model = model;

            GroundControlStationForm gcsView = new GroundControlStationForm(gcsController);

            gcsController.View = gcsView;

            gcsController.Start();

            Application.Run(gcsView);

            gcsController.Stop();
        }
    }
}

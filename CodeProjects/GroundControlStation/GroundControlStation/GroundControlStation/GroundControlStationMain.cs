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
        enum FlightModes { SimulatedFlight, RealFlight };

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            //TODO focus on thread ui update.
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            //FlightModes flightMode = FlightModes.RealFlight;
            FlightModes flightMode = FlightModes.SimulatedFlight;

            //SerialPort port = new SerialPort("COM7", 76800, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One); //MOST RECENT
            //SerialPort port = new SerialPort("COM7", 76800, Parity.None, 8, StopBits.One);
           // SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);

            SerialPort port = null;
            if (flightMode == FlightModes.RealFlight)
            {
                //port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
                port = new SerialPort("COM12", 115200, Parity.None, 8, StopBits.One);
                //port = new SerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            }
            else
            {
                port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One);
            }
     //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            
 //           port.ReadTimeout = 500;

            SimulatorInterface xInterface = new SimulatorInterface(8089, 49000, IPAddress.Parse("127.0.0.255"));

            SerialPortInterface portInterface = new SerialPortInterface(port);

            FlightComputerInterface fcInterface = new FlightComputerInterface(portInterface);
            fcInterface.Open();

            

            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = new SimulatorTelemetry();


            /* Crap gains.
            model.YawIntegralGain = .806f;
            model.YawProportionalGain = 1.116f;
            model.YawDerivativeGain = .391f;
            model.YawAntiWindupGain = 0.135f;

            model.XIntegralGain = 0.0f;
            //model.XProportionalGain = 4.101f;
            //model.XDerivativeGain = 8.449f;
            model.XProportionalGain = .00289f;
            model.XDerivativeGain = .01859f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 0.000541f;
            model.PitchAngularVelocityGain = .0005235f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 5.638f;
            model.YDerivativeGain = 1.116f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 1.696f;
            model.RollAngularVelocityGain = .241f;


            model.ZIntegralGain = .0005f;
            model.ZProportionalGain = 0.003f;
            model.ZDerivativeGain = .0061f;
            model.ZAntiWindupGain = .6685f;
            */


            model.YawIntegralGain = .806f;
            model.YawProportionalGain = 1.116f;
            //model.YawDerivativeGain = .391f;
            model.YawDerivativeGain = .391f;
            model.YawAntiWindupGain = 0.135f;

            model.XIntegralGain = 0.0f;
            //model.XProportionalGain = 4.101f;
            //model.XDerivativeGain = 8.449f;
            model.XProportionalGain = .00019f;
            model.XDerivativeGain = .00153f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 1.7081f;
            model.PitchAngularVelocityGain = 1.77509f;

            model.YIntegralGain = 0.0f;
           // model.YProportionalGain = 5.638f;
            //model.YProportionalGain = .01439f;
            model.YProportionalGain = .04577f;
            //model.YDerivativeGain = 1.116f;
            //model.YDerivativeGain = .02869f;
            //model.YDerivativeGain = .00268f;//mostrecent 4/12/2014
            model.YDerivativeGain = .13043f;
            model.YAntiWindupGain = 0.0f;
            //model.LateralInnerLoopGain = 1.696f;
            //model.LateralInnerLoopGain = .53623f;
            //model.LateralInnerLoopGain = .38271f;//mostrecent 4/12
            model.LateralInnerLoopGain = .36102f;
            //model.RollAngularVelocityGain = .241f;
            model.RollAngularVelocityGain = .04348f;

            /*
            model.ZIntegralGain = .0005f;
            model.ZProportionalGain = 0.003f;
            model.ZDerivativeGain = .0061f;
            model.ZAntiWindupGain = .6685f;
            */

            model.ZIntegralGain = .000874f;
            model.ZProportionalGain = 0.00445f;
            model.ZDerivativeGain = .430435f;
            //model.ZAntiWindupGain = .000874f;
            model.ZAntiWindupGain = .300874f;




            /*
            model.YawIntegralGain = .806f;
            model.YawProportionalGain = 1.116f;
            model.YawDerivativeGain = .391f;
            model.YawAntiWindupGain = 0.135f;

            model.XIntegralGain = 0.0f;
            //model.XProportionalGain = 4.101f;
            //model.XDerivativeGain = 8.449f;
            model.XProportionalGain = 0.0f;
            model.XDerivativeGain = 0f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 1.667f;
            model.PitchAngularVelocityGain = 1.638f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 5.638f;
            model.YDerivativeGain = 1.116f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 1.696f;
            model.RollAngularVelocityGain = .241f;


            model.ZIntegralGain = .0005f;
            model.ZProportionalGain = 0.003f;
            model.ZDerivativeGain = .0061f;
            model.ZAntiWindupGain = .6685f;
             */





            /*model.ZIntegralGain = .019f;
            model.ZProportionalGain = 0.05901f;
            model.ZDerivativeGain = .011f;
            model.ZAntiWindupGain = .1f;*/







            /* orig 3/30/ */
            /*
            model.YawIntegralGain = .014f;
            model.YawProportionalGain = .073f;
            model.YawDerivativeGain = .01f;
            model.YawAntiWindupGain = 0.135f;
             
            model.XIntegralGain = 0.0f;
            model.XProportionalGain = .217f;
            model.XDerivativeGain = 1.859f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = .015f;
            model.PitchAngularVelocityGain = .159f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 7.246f;
            model.YDerivativeGain = .014f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = .036f;
            model.RollAngularVelocityGain = .652f;


            model.ZIntegralGain = .019f;
            model.ZProportionalGain = 0.05901f;
            model.ZDerivativeGain = .011f;
            model.ZAntiWindupGain = .1f;

            */

            /*
            model.ZIntegralGain = .02f;
            model.ZProportionalGain = 1.901f;
            model.ZDerivativeGain = 1.141f;
            model.ZAntiWindupGain = .1f;
             */


            ///*model.YawIntegralGain = .008f;
            //model.YawDerivativeGain = .85f;
            //model.YawProportionalGain = 1.0f;
            //model.YawAntiWindupGain = 0.1f;
            //*/

            ////original from other program
            ///*
            // * model.YawIntegralGain = .5f;
            //model.YawProportionalGain = 1.8471f;
            //model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            //model.YawAntiWindupGain = 0.1f;
            // */
            ///*model.YawIntegralGain = .05f;
            //model.YawProportionalGain = .018471f;
            //model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            //model.YawAntiWindupGain = 0.01f;*/

            //model.YawIntegralGain = .00f;
            //model.YawProportionalGain = 0.0058471f;
            //model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            //model.YawAntiWindupGain = 0.1f;

            //model.XIntegralGain = 0.0f;
            //model.XProportionalGain = .289f;
            //model.XDerivativeGain = 1.859f;
            //model.XAntiWindupGain = 0.0f; 
            //model.LongitudeInnerLoopGain = .031f;
            //model.PitchAngularVelocityGain = .03f;

            //model.YIntegralGain = 0.0f;
            //model.YProportionalGain = 1.437f;
            //model.YDerivativeGain = 2.8363f;
            //model.YAntiWindupGain = 0.0f; 
            //model.LateralInnerLoopGain = .092f;
            //model.RollAngularVelocityGain = .048f;

            ///*
            //model.ZIntegralGain = .02f;
            //model.ZProportionalGain = 1.901f;
            //model.ZDerivativeGain = 1.141f;
            //model.ZAntiWindupGain = .1f;
            // */

            //model.ZIntegralGain = .02f;
            //model.ZProportionalGain = 0.05901f;
            //model.ZDerivativeGain = .01141f;
            //model.ZAntiWindupGain = .1f;
             

            GroundControlStationController gcsController =
                new GroundControlStationController(xInterface, fcInterface, GroundControlStationController.DATATOSEND.SENSORDATA);

            gcsController.Model = model;

            GroundControlStationForm gcsView = new GroundControlStationForm(gcsController);

            gcsController.View = gcsView;

            gcsController.Start();

            Application.Run(gcsView);

            gcsController.Stop();
        }
    }
}

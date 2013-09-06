using System;
using System.IO.Ports;
using GroundControlStation.Model;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using GroundControlStation.Interfaces;

namespace HelicopterIntegrationTests
{
    [TestClass]
    public class FlightComputerInterfaceTest
    {
        /// <summary>
        /// Test the integration between the flight computer, and the GCS.
        /// Tests the flight computer interface. 
        /// </summary>
        [TestMethod]
        public void TestFlightComputerInterface()
        {
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(port))
            {
                fcInt.Open();

                FlightControllerTelemetryData telemetryData = fcInt.Receive();

                Assert.IsTrue(telemetryData.MsgType == 122);

                Assert.IsTrue(telemetryData.HeadingDegrees == 321);

                fcInt.Transmit(telemetryData);

                for (int i = 0; i < 127; i++)
                {
                    telemetryData = fcInt.Receive();

                    Assert.IsTrue(telemetryData.HeadingDegrees == i);

                    fcInt.Transmit(telemetryData);
                }

                telemetryData = fcInt.Receive();

                Assert.IsTrue(telemetryData.HeadingDegrees == 12);
            }
        }


        [TestMethod]
        public void TestRadioIntegration()
        {
//            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
            port.Open();

            byte[] byteBuffer = new byte[3];
            port.Read(byteBuffer, 0, 1);
            port.Read(byteBuffer, 1, 1);
            port.Read(byteBuffer, 2, 1);


            byte msgType = byteBuffer[0];

            Assert.IsTrue(msgType == 122);

            short yawVal = BitConverter.ToInt16(byteBuffer, 1);

            Assert.IsTrue(yawVal == 321);

            port.Write(byteBuffer, 0, 3);
        //    port.Write(byteBuffer, 0, 1);
        //    port.Write(byteBuffer,1,2);


            /*

            byteBuffer = new byte[3];
            port.Read(byteBuffer, 0, 1);
            port.Read(byteBuffer, 1, 1);
            port.Read(byteBuffer, 2, 1);
            port.Read(byteBuffer, 0, 1);

            */

            for (int i = 0; i < 127; i++)
            {
                byteBuffer = new byte[3];

                port.Read(byteBuffer, 0, 1);
                port.Read(byteBuffer, 1, 1);
                port.Read(byteBuffer, 2, 1);
                yawVal = BitConverter.ToInt16(byteBuffer, 1);

                Assert.IsTrue(yawVal == i);

                //port.Write(BitConverter.GetBytes((short)122), 0, 2);
                port.Write(byteBuffer, 0, 3);
            }

            byteBuffer = new byte[3];

            port.Read(byteBuffer, 0, 1);
            port.Read(byteBuffer, 1, 1);
            port.Read(byteBuffer, 2, 1);
            yawVal = BitConverter.ToInt16(byteBuffer, 1);

            Assert.IsTrue(yawVal == 12);

            port.Close();
        }
    }
}

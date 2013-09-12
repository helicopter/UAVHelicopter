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

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();

                FlightComputerTelemetry telemetry = fcInt.Receive();

                Assert.IsTrue(telemetry.MsgType == 122);

                Assert.IsTrue(telemetry.MagX == 321);

                fcInt.Transmit(telemetry);

                for (int i = 0; i < 127; i++)
                {
                    telemetry = fcInt.Receive();

                    Assert.IsTrue(telemetry.MagX == i);

                    fcInt.Transmit(telemetry);
                }

                telemetry = fcInt.Receive();

                Assert.IsTrue(telemetry.MagX == 12);
            }
        }

        [TestMethod]
        public void TestSystemTelemetryTransmitAndReceive()
        {
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();

                FlightComputerTelemetry telemetry = fcInt.Receive();

                Assert.IsTrue(telemetry.MsgType == 2);

                Assert.IsTrue(telemetry.MagX == 33);
                Assert.IsTrue(telemetry.MagY == 32);
                Assert.IsTrue(telemetry.MagZ == 31);

                fcInt.Transmit(telemetry);

                telemetry = fcInt.Receive();

                Assert.IsTrue(telemetry.MagX == 12);
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

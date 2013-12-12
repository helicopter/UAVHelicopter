using System;
using System.IO.Ports;
using GroundControlStation.Model;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using GroundControlStation.Interfaces;
using GroundControlStation.Messages;

namespace HelicopterIntegrationTests
{
    [TestClass]
    public class FlightComputerInterfaceTest
    {

        /// <summary>
        /// Test the integration between the flight computer, and the GCS.
        /// Tests the flight computer interface. 
        /// </summary>
        //[TestMethod]
        //public void TestFlightComputerInterface()
        //{
        //    SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
        //    //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);

        //    SerialPortInterface portInterface = new SerialPortInterface(port);

        //    using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
        //    {
        //        fcInt.Open();

        //        FlightComputerTelemetry telemetry = fcInt.Receive();

        //        Assert.IsTrue(telemetry.MsgType == 122);

        //        Assert.IsTrue(telemetry.MagX == 321);

        //        fcInt.Transmit(telemetry);

        //        for (int i = 0; i < 127; i++)
        //        {
        //            telemetry = fcInt.Receive();

        //            Assert.IsTrue(telemetry.MagX == i);

        //            fcInt.Transmit(telemetry);
        //        }

        //        telemetry = fcInt.Receive();

        //        Assert.IsTrue(telemetry.MagX == 12);
        //    }
        //}

        [TestMethod]
        public void TestSystemTelemetryTransmitAndReceive()
        {
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();

                GroundControlStationModel model = new GroundControlStationModel();

                FlightComputerTelemetryMessage telemetry = (FlightComputerTelemetryMessage)fcInt.Receive();

                telemetry.UpdateModel(model);

                Assert.IsTrue(telemetry.MsgType == 2);
                Assert.IsTrue(model.ChecksumErrors == 2);
                Assert.IsTrue(model.MagYaw == 2.22f);
                Assert.IsTrue(model.Timeouts == 2);
                Assert.IsTrue(model.UnrecognizedMsgTypes == 2);
             //   Assert.IsTrue(model.YawControl == 2.22f);
                Assert.IsTrue(model.YawDerivativeError == 2.22f);
                Assert.IsTrue(model.YawIntegral == 2.22f);
                Assert.IsTrue(model.YawProportional == 2.22f);
                Assert.IsTrue(model.YawVelocityDegreesPerSecond == 2.22f);


                Assert.IsTrue(model.AltitudeFeet == 2.22f);
                Assert.IsTrue(model.LateralControl == 2.22f);
                Assert.IsTrue(model.MainRotorCollectiveControl == 2.22f);



                fcInt.Transmit(telemetry);

                telemetry = (FlightComputerTelemetryMessage) fcInt.Receive();

                Assert.IsTrue(telemetry.MagYaw == 12);
            }
        }




        //[TestMethod]
        //public void TestReliablyTransmit()
        //{
        //    SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
        //    //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);


        //    SerialPortInterface portInterface = new SerialPortInterface(port);

        //    using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
        //    {
        //        fcInt.Open();


        //        FlightComputerTelemetryMessage telemetry = null;

        //        //wait for the starting message.
        //        telemetry = (FlightComputerTelemetryMessage) fcInt.Receive();
        //        Assert.IsTrue(telemetry.MagX == 12);

        //        for (short i = 0; i < 127; i++)
        //        {
        //            telemetry = new FlightComputerTelemetryMessage();
        //            telemetry.MagX = i;
        //            telemetry.MagY = i;
        //            telemetry.MagZ = i;
        //            telemetry.MagYaw = (ushort)i;
        //            telemetry.YawDerivativeError = i;
        //            telemetry.YawIntegral = i;
        //            telemetry.YawProportional = i;
        //            telemetry.YawVelocityDegreesPerSecond = i;
        //            telemetry.Timeouts = i;
        //            telemetry.UnrecognizedMsgTypes = i;
        //            telemetry.ChecksumErrors = i;

        //            fcInt.Transmit(telemetry);
        //        }


        //        telemetry = (FlightComputerTelemetryMessage) fcInt.Receive();

        //        Assert.IsTrue(telemetry.MagX == 0);
        //        Assert.IsTrue(telemetry.MagY == 0);
        //        Assert.IsTrue(telemetry.MagZ == 0);
        //        Assert.IsTrue(telemetry.MagYaw == 0);
        //        Assert.IsTrue(telemetry.YawDerivativeError == 0);
        //        Assert.IsTrue(telemetry.YawIntegral == 0);
        //        Assert.IsTrue(telemetry.YawProportional == 0);
        //        Assert.IsTrue(telemetry.YawVelocityDegreesPerSecond == 0);

        //        Assert.IsTrue(telemetry.Timeouts == 0);
        //        Assert.IsTrue(telemetry.UnrecognizedMsgTypes == 0);
        //        Assert.IsTrue(telemetry.ChecksumErrors == 0);
        //    } 
        //}




//        [TestMethod]
//        public void TestRadioIntegration()
//        {
////            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
//            SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
//            port.Open();

//            byte[] byteBuffer = new byte[3];
//            port.Read(byteBuffer, 0, 1);
//            port.Read(byteBuffer, 1, 1);
//            port.Read(byteBuffer, 2, 1);


//            byte msgType = byteBuffer[0];

//            Assert.IsTrue(msgType == 122);

//            short yawVal = BitConverter.ToInt16(byteBuffer, 1);

//            Assert.IsTrue(yawVal == 321);

//            port.Write(byteBuffer, 0, 3);
//        //    port.Write(byteBuffer, 0, 1);
//        //    port.Write(byteBuffer,1,2);


//            /*

//            byteBuffer = new byte[3];
//            port.Read(byteBuffer, 0, 1);
//            port.Read(byteBuffer, 1, 1);
//            port.Read(byteBuffer, 2, 1);
//            port.Read(byteBuffer, 0, 1);

//            */

//            for (int i = 0; i < 127; i++)
//            {
//                byteBuffer = new byte[3];

//                port.Read(byteBuffer, 0, 1);
//                port.Read(byteBuffer, 1, 1);
//                port.Read(byteBuffer, 2, 1);
//                yawVal = BitConverter.ToInt16(byteBuffer, 1);

//                Assert.IsTrue(yawVal == i);

//                //port.Write(BitConverter.GetBytes((short)122), 0, 2);
//                port.Write(byteBuffer, 0, 3);
//            }

//            byteBuffer = new byte[3];

//            port.Read(byteBuffer, 0, 1);
//            port.Read(byteBuffer, 1, 1);
//            port.Read(byteBuffer, 2, 1);
//            yawVal = BitConverter.ToInt16(byteBuffer, 1);

//            Assert.IsTrue(yawVal == 12);

//            port.Close();
//        }
    }
}

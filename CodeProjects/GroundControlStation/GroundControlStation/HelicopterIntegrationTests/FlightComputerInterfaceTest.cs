using System;
using System.IO.Ports;
using GroundControlStation.Model;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using GroundControlStation.Interfaces;
using GroundControlStation.Messages;
using System.Diagnostics;
using System.Collections;

namespace HelicopterIntegrationTests
{
    [TestClass]
    public class FlightComputerInterfaceTest
    {

        short readShort(SerialPortInterface portInterface)
        {
            return (short) (portInterface.ReadByte() << 8 | portInterface.ReadByte());
        }

        float readFloat(SerialPortInterface portInterface)
        {
            return (float)(portInterface.ReadByte() << 8 | portInterface.ReadByte());
        }

        [TestMethod]
        public void TestSensorData()
        {
            SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
            SerialPortInterface portInterface = new SerialPortInterface(port);
            portInterface.Open();

            System.Diagnostics.Debug.WriteLine(readShort(portInterface));
            System.Diagnostics.Debug.WriteLine(readShort(portInterface));
            System.Diagnostics.Debug.WriteLine(readShort(portInterface));
            System.Diagnostics.Debug.WriteLine(readShort(portInterface));
            System.Diagnostics.Debug.WriteLine(readShort(portInterface));
            System.Diagnostics.Debug.WriteLine(readShort(portInterface));
            System.Diagnostics.Debug.WriteLine(readFloat(portInterface));
            System.Diagnostics.Debug.WriteLine(readFloat(portInterface));
            System.Diagnostics.Debug.WriteLine(readFloat(portInterface));
            System.Diagnostics.Debug.WriteLine(readFloat(portInterface));
            System.Diagnostics.Debug.WriteLine(readFloat(portInterface));
            System.Diagnostics.Debug.WriteLine(readFloat(portInterface));


            portInterface.Close();
        }




        [TestMethod]
        public void FwdGpsData()
        {
            SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);
            portInterface.Open();

            ArrayList list = new ArrayList();

            


            while (true)
            {

                list.Add(portInterface.ReadByte());
            }
            /*float b = BitConverter.ToSingle(bytes,0);
            double c = -122.21683502197266d;
            float d = -122.21683502197266f;*/
            //Assert.IsTrue(b == 6.4f);
        }


        [TestMethod]
        public void TestSendReceiveFloat()
        {
            SerialPort port = new SerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);
            portInterface.Open();

            float f = -180.21683502197266f; //13.0101013183596
            byte[] bytes = BitConverter.GetBytes(f);

            portInterface.Write(bytes, 0, 4);

            byte[] bytes2 = new byte[4];

            bytes2[0] = portInterface.ReadByte();
            bytes2[1] = portInterface.ReadByte();
            bytes2[2] = portInterface.ReadByte();
            bytes2[3] = portInterface.ReadByte();
            
            portInterface.Close();
            /*float b = BitConverter.ToSingle(bytes,0);
            double c = -122.21683502197266d;
            float d = -122.21683502197266f;*/
            //Assert.IsTrue(b == 6.4f);
        }

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
        public void gainsmessageasync_test()
        {
            SerialPort port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();
                port.DiscardInBuffer();
                port.DiscardOutBuffer();

                FlightComputerTelemetryMessage gains = null;

                for (int i = 0; i < 1000; i++)
                {
                    gains = (FlightComputerTelemetryMessage) fcInt.Receive();

                    Assert.IsTrue(gains.LateralInnerLoopGain == 1.1f);
                    Assert.IsTrue(gains.LongitudeInnerLoopGain == 1.2f);
                    Assert.IsTrue(gains.PitchAngularVelocityGain == 1.3f);
                    Assert.IsTrue(gains.RollAngularVelocityGain == 1.4f);
                    Assert.IsTrue(gains.XAntiWindupGain == 1.5f);
                    Assert.IsTrue(gains.XDerivativeGain == 1.6f);
                    Assert.IsTrue(gains.XIntegralGain == 1.7f);
                    Assert.IsTrue(gains.XProportionalGain == 1.8f);
                    Assert.IsTrue(gains.YAntiWindupGain == 1.9f);
                    Assert.IsTrue(gains.YawAntiWindupGain == 1.0f);
                    Assert.IsTrue(gains.YawDerivativeGain == 1.11f);
                    Assert.IsTrue(gains.YawIntegralGain == 1.12f);
                    Assert.IsTrue(gains.YawProportionalGain == 1.13f);
                    Assert.IsTrue(gains.YDerivativeGain == 1.14f);
                    Assert.IsTrue(gains.YIntegralGain == 1.15f);
                    Assert.IsTrue(gains.YProportionalGain == 1.16f);
                    Assert.IsTrue(gains.ZAntiWindupGain == 1.17f);
                    Assert.IsTrue(gains.ZDerivativeGain == 1.18f);
                    Assert.IsTrue(gains.ZIntegralGain == 1.19f);
                    Assert.IsTrue(gains.ZProportionalGain == i);

                    Debug.WriteLine(i);

                    fcInt.Transmit(gains);
                }

                gains = (FlightComputerTelemetryMessage) fcInt.Receive();

                Assert.IsTrue(gains.ZProportionalGain == 12);

            }
        }














        //[TestMethod]
        //public void gainsmessageasync_test()
        //{
        //    SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);

        //    SerialPortInterface portInterface = new SerialPortInterface(port);

        //    using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
        //    {
        //        fcInt.Open();
        //        port.DiscardInBuffer();
        //        port.DiscardOutBuffer();

        //        GainsMessage gains = null;

        //        for (int i = 0; i < 100; i++)
        //        {
        //            gains = (GainsMessage)fcInt.Receive();

        //            Assert.IsTrue(gains.LateralInnerLoopGain == 1.1f);
        //            Assert.IsTrue(gains.LongitudeInnerLoopGain == 1.2f);
        //            Assert.IsTrue(gains.PitchAngularVelocityGain == 1.3f);
        //            Assert.IsTrue(gains.RollAngularVelocityGain == 1.4f);
        //            Assert.IsTrue(gains.XAntiWindupGain == 1.5f);
        //            Assert.IsTrue(gains.XDerivativeGain == 1.6f);
        //            Assert.IsTrue(gains.XIntegralGain == 1.7f);
        //            Assert.IsTrue(gains.XProportionalGain == 1.8f);
        //            Assert.IsTrue(gains.YAntiWindupGain == 1.9f);
        //            Assert.IsTrue(gains.YawAntiWindupGain == 1.0f);
        //            Assert.IsTrue(gains.YawDerivativeGain == 1.11f);
        //            Assert.IsTrue(gains.YawIntegralGain == 1.12f);
        //            Assert.IsTrue(gains.YawProportionalGain == 1.13f);
        //            Assert.IsTrue(gains.YDerivativeGain == 1.14f);
        //            Assert.IsTrue(gains.YIntegralGain == 1.15f);
        //            Assert.IsTrue(gains.YProportionalGain == 1.16f);
        //            Assert.IsTrue(gains.ZAntiWindupGain == 1.17f);
        //            Assert.IsTrue(gains.ZDerivativeGain == 1.18f);
        //            Assert.IsTrue(gains.ZIntegralGain == 1.19f);
        //            Assert.IsTrue(gains.ZProportionalGain == i);

        //            Debug.WriteLine(i);

        //            fcInt.Transmit(gains);
        //        }

        //        gains = (GainsMessage)fcInt.Receive();

        //        Assert.IsTrue(gains.ZProportionalGain == 12);

        //    }
        //}











        [TestMethod]
        public void gainsmessage_test()
        {
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();


                GainsMessage gains = (GainsMessage)fcInt.Receive();

                Assert.IsTrue(gains.LateralInnerLoopGain == 1.1f);
                Assert.IsTrue(gains.LongitudeInnerLoopGain == 1.2f);
                Assert.IsTrue(gains.PitchAngularVelocityGain == 1.3f);
                Assert.IsTrue(gains.RollAngularVelocityGain == 1.4f);
                Assert.IsTrue(gains.XAntiWindupGain == 1.5f);
                Assert.IsTrue(gains.XDerivativeGain == 1.6f);
                Assert.IsTrue(gains.XIntegralGain == 1.7f);
                Assert.IsTrue(gains.XProportionalGain == 1.8f);
                Assert.IsTrue(gains.YAntiWindupGain == 1.9f);
                Assert.IsTrue(gains.YawAntiWindupGain == 1.0f);
                Assert.IsTrue(gains.YawDerivativeGain == 1.11f);
                Assert.IsTrue(gains.YawIntegralGain == 1.12f);
                Assert.IsTrue(gains.YawProportionalGain == 1.13f);
                Assert.IsTrue(gains.YDerivativeGain == 1.14f);
                Assert.IsTrue(gains.YIntegralGain == 1.15f);
                Assert.IsTrue(gains.YProportionalGain == 1.16f);
                Assert.IsTrue(gains.ZAntiWindupGain == 1.17f);
                Assert.IsTrue(gains.ZDerivativeGain == 1.18f);
                Assert.IsTrue(gains.ZIntegralGain == 1.19f);
                Assert.IsTrue(gains.ZProportionalGain == 1.20f);



                fcInt.Transmit(gains);

                gains = (GainsMessage)fcInt.Receive();

                Assert.IsTrue(gains.ZProportionalGain == 12);

            }
        }





        [TestMethod]
        public void newmsgformat2_test()
        {
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();


                SimpleTelemetryMessage sensorData = (SimpleTelemetryMessage)fcInt.Receive();

                Assert.IsTrue(sensorData.MsgType == 6);
                Assert.IsTrue(sensorData.PitchAngularVelocityRadsPerSecond == 3.14f);
                Assert.IsTrue(sensorData.PressureMillibars == 44.313f);
                Assert.IsTrue(sensorData.RollAngularVelocityRadsPerSecond == 4422.1f);
                Assert.IsTrue(sensorData.XAccelFrdMss == 33.33f);
                Assert.IsTrue(sensorData.XEcefCm == 331);
                Assert.IsTrue(sensorData.XMagFrd == 22.2f);
                Assert.IsTrue(sensorData.YawRads == 231.23f);
                Assert.IsTrue(sensorData.ZVelocityFRDCms == .0123f);



                fcInt.Transmit(sensorData);

                sensorData = (SimpleTelemetryMessage)fcInt.Receive();

                Assert.IsTrue(sensorData.PressureMillibars == 12);

            }
        }



        [TestMethod]
        public void newmsgformat_test()
        {
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);

            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();

                
                SensorDataMessage sensorData = (SensorDataMessage)fcInt.Receive();

                Assert.IsTrue(sensorData.MsgType == 4);
                Assert.IsTrue(sensorData.PitchAngularVelocityRadsPerSecond == 3.14f);
                Assert.IsTrue(sensorData.PressureMillibars == 44.313f);
                Assert.IsTrue(sensorData.RollAngularVelocityRadsPerSecond == 4422.1f);
                Assert.IsTrue(sensorData.XAccelFrdMss == 33.33f);
                Assert.IsTrue(sensorData.XEcefCm == 331);
                Assert.IsTrue(sensorData.XMagFrd == 22.2f);



                fcInt.Transmit(sensorData);

                sensorData = (SensorDataMessage)fcInt.Receive();

                Assert.IsTrue(sensorData.PressureMillibars == 12);

            }
        }









        [TestMethod]
        public void TestSystemTelemetryTransmitAndReceive()
        {
            //SerialPort port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One);
            SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
            
            SerialPortInterface portInterface = new SerialPortInterface(port);

            using (FlightComputerInterface fcInt = new FlightComputerInterface(portInterface))
            {
                fcInt.Open();
                port.DiscardInBuffer();

                GroundControlStationModel model = new GroundControlStationModel();



                FlightComputerTelemetryMessage telemetry = (FlightComputerTelemetryMessage)fcInt.Receive();

                telemetry.UpdateModel(model);

                Assert.IsTrue(telemetry.MsgType == 2);
                Assert.IsTrue(model.ChecksumErrors == 2);
                //Assert.IsTrue(model.MagYaw == 2.22f);
                Assert.IsTrue(model.Timeouts == 2);
                Assert.IsTrue(model.UnrecognizedMsgTypes == 2);
             //   Assert.IsTrue(model.YawControl == 2.22f);
                Assert.IsTrue(model.YawDerivativeError == 2.22f);
                Assert.IsTrue(model.YawIntegral == 2.22f);
                Assert.IsTrue(model.YawProportional == 2.22f);
                Assert.IsTrue(model.YawVelocityRadsPerSecond == 2.22f);


                //Assert.IsTrue(model.AltitudeMetersAgl == 4.46f);
                Assert.IsTrue(model.LateralControl == 2.22f);
                Assert.IsTrue(model.MainRotorCollectiveControl == 2.22f);

                //Assert.IsTrue(model.YawControl == -3.22f);//negative because in update model we make it negative. 
                Assert.IsTrue(model.XProportional == 4.22f);
                Assert.IsTrue(model.YProportional == 1.22f);
                Assert.IsTrue(model.ZProportional == 6.22f);
                Assert.IsTrue(model.ZIntegral == 8.22f);
                Assert.IsTrue(model.ZNEDLocalFrame == 9.22f);
                Assert.IsTrue(model.XNEDLocalFrame == 19.22f);
                Assert.IsTrue(model.YNEDLocalFrame == 21.33f);

                Assert.IsTrue(model.XVEcefCms == 42);
                Assert.IsTrue(model.YVEcefCms == 12);
                Assert.IsTrue(model.ZVEcefCms == 32);

                fcInt.Transmit(telemetry);

                DateTime startTime = DateTime.Now;

                telemetry = (FlightComputerTelemetryMessage) fcInt.Receive();
                telemetry.UpdateModel(model);

                Assert.IsTrue(model.PressureMillibars == 12);

                DateTime endTime = DateTime.Now;

                Trace.WriteLine("Time ms: " + endTime.Subtract(startTime).Milliseconds);
              //  Assert.IsTrue(false, "Time ms: " + endTime.Subtract(startTime).Milliseconds);
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
        //            telemetry.magY = i;
        //            telemetry.magZ = i;
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
        //        Assert.IsTrue(telemetry.magY == 0);
        //        Assert.IsTrue(telemetry.magZ == 0);
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

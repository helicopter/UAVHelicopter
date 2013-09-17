using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using UnitTests.mocks;

namespace UnitTests
{
    [TestClass]
    public class TelemetryMessageTests
    {
        [TestMethod]
        public void TestTelemetryMessage()
        {
            FlightComputerTelemetry message = new FlightComputerTelemetry();

            message.MagX = (short) 32767;
            message.MagY = (short) 125;
            message.MagZ = (short) 32;

            Assert.IsTrue(message.MsgType == FlightComputerTelemetry.MessageType);
            Assert.IsTrue(FlightComputerTelemetry.NumOfBytesInMsg == 7);

            byte[] bytes = message.GetRawBytes();

            //verify bytes in the message.
            Assert.IsTrue(bytes[0] == FlightComputerTelemetry.MessageType);
            Assert.IsTrue(bytes[1] == (32767 & 0xFF));
            Assert.IsTrue(bytes[2] == ((32767 >> 8) & 0xFF));
            Assert.IsTrue(bytes[3] == 125);
            Assert.IsTrue(bytes[4] == 0);
            Assert.IsTrue(bytes[5] == 32);
            Assert.IsTrue(bytes[6] == 0);
        }

        /// <summary>
        /// Test that building a telemetry message from a 
        /// bunch of bytes works properly.
        /// </summary>
        [TestMethod]
        public void TestBuildTelemetryMessage()
        {
            byte[] msgBytes = new byte[FlightComputerTelemetry.NumOfBytesInMsg];

            msgBytes[0] = FlightComputerTelemetry.MessageType;
            msgBytes[1] = (32767 & 0xFF);
            msgBytes[2] = ((32767 >> 8) & 0xFF);
            msgBytes[3] = (32766 & 0xFF);
            msgBytes[4] = ((32766 >> 8) & 0xFF);
            msgBytes[5] = (32765 & 0xFF);
            msgBytes[6] = ((32765 >> 8) & 0xFF);

            FlightComputerTelemetry telMsg = FlightComputerTelemetry.BuildMessageSt(msgBytes);

            Assert.IsTrue(telMsg.MagX == (short)32767);
            Assert.IsTrue(telMsg.MagY == (short)32766);
            Assert.IsTrue(telMsg.MagZ == (short)32765);

            //have a bunch of bytes

            //feed it to building a message.


        }


        [TestMethod]
        public void TestFlightComputerInterfaceMsgBuilding()
        {
            SerialPortInterface serialPortInterface = new MockSerialPortInterface();
            MockSerialPortInterface mockSPI = (MockSerialPortInterface) serialPortInterface;
            

            FlightComputerInterface fci = new FlightComputerInterface(serialPortInterface);

            FlightComputerTelemetry message = new FlightComputerTelemetry();

            message.MagX = (short)32767;
            message.MagY = (short)125;
            message.MagZ = (short)32;

            fci.Transmit(message);

            List<byte> bytes = mockSPI.Bytes;
            List<byte> bytesCopy = bytes;

            Assert.IsTrue(bytes[0] == 0xB5);
            Assert.IsTrue(bytes[1] == 0x62);
            Assert.IsTrue(bytes[2] == 0xD7);
            Assert.IsTrue(bytes[3] == FlightComputerTelemetry.MessageType);
            Assert.IsTrue(bytes[4] == (32767 & 0xFF));
            Assert.IsTrue(bytes[5] == ((32767 >> 8) & 0xFF));
            Assert.IsTrue(bytes[6] == 125);
            Assert.IsTrue(bytes[7] == 0);
            Assert.IsTrue(bytes[8] == 32);
            Assert.IsTrue(bytes[9] == 0);
            Assert.IsTrue(bytes[10] == 29); 
            Assert.IsTrue(bytes[11] == 183);


            FlightComputerTelemetry message2 = (FlightComputerTelemetry)fci.Receive();

            Assert.IsTrue(message2.MagX == message.MagX);
            Assert.IsTrue(message2.MagY == message.MagY);
            Assert.IsTrue(message2.MagZ == message.MagZ);


            mockSPI.Reset();

            /**
             * Test that the interface properly rejects garbage characters
             */
            List<byte> lbytes = new List<byte>();

            //junk data
            lbytes.Add(0xD7);
            lbytes.Add(0x22);
            lbytes.Add(0x42);
            lbytes.Add(0x62);
            lbytes.Add(0xB5);
            lbytes.Add(0xB5);
            lbytes.Add(0xB5);
            lbytes.Add(0x22);

            //Real data
            lbytes.AddRange(bytes);

            ((MockSerialPortInterface) serialPortInterface).Bytes = lbytes;

            message2 = (FlightComputerTelemetry)fci.Receive();

            Assert.IsTrue(message2.MagX == message.MagX);
            Assert.IsTrue(message2.MagY == message.MagY);
            Assert.IsTrue(message2.MagZ == message.MagZ);

            mockSPI.Reset();

            /**
             * Verify that the message is rejected if the checksum is incorrect.
             */
            byte checksumA = lbytes[lbytes.Count - 2];
            byte checksumB = lbytes[lbytes.Count - 1];

            lbytes[lbytes.Count - 1] = 0xFF;

            mockSPI.Bytes = lbytes;

            message2 = (FlightComputerTelemetry)fci.Receive();

            Assert.IsTrue(message2 == null);

            lbytes[lbytes.Count - 1] = checksumB;
            lbytes[lbytes.Count - 2] = 0xFF;

            mockSPI.Reset();
            mockSPI.Bytes = lbytes;

            message2 = (FlightComputerTelemetry)fci.Receive();

            Assert.IsTrue(message2 == null);

            lbytes[lbytes.Count - 2] = checksumA;

            mockSPI.Reset();
            mockSPI.Bytes = lbytes;

            message2 = (FlightComputerTelemetry)fci.Receive();

            Assert.IsTrue(message2 != null);

            /*
             * Verify that a message with an incorrect ID is rejected
             */
            mockSPI.Reset();
            mockSPI.Bytes = bytesCopy;
            message2 = (FlightComputerTelemetry)fci.Receive();
            Assert.IsTrue(message2 != null);

            //corrupt the ID field.
            mockSPI.Reset();
            mockSPI.Bytes = bytesCopy;
            mockSPI.Bytes[3] = 22;
            message2 = (FlightComputerTelemetry)fci.Receive();
            Assert.IsTrue(message2 == null);

            mockSPI.Reset();
            mockSPI.Bytes = bytesCopy;
            mockSPI.Bytes[3] = FlightComputerTelemetry.MessageType;
            message2 = (FlightComputerTelemetry)fci.Receive();
            Assert.IsTrue(message2 != null);

            //Remove the last byte simulating a timeout exception since it tried to receive more bytes than were transmited.
            mockSPI.Reset();
            mockSPI.Bytes = bytesCopy;
            mockSPI.Bytes.RemoveAt(11);
            message2 = (FlightComputerTelemetry)fci.Receive();
            Assert.IsTrue(message2 == null);
        }





        /*
        [TestMethod]
        public void TestTelemetryMessage()
        {
            msgBytes[0] = 0xB5;
            msgBytes[1] = 0x62;
         
         
            TelemetryMessage message = new TelemetryMessage();

            message.MagX = (ushort)65535;
            message.MagY = (ushort)125;
            message.MagZ = (ushort)32;

            Assert.IsTrue(message.ID == TelemetryMessage.ID);
            Assert.IsTrue(TelemetryMessage.BytesInMsg == 11);
            Assert.IsTrue(TelemetryMessage.SyncByte1 = (byte)0xB5);
            Assert.IsTrue(TelemetryMessage.SyncByte2 = (byte)0x62);

            Assert.IsTrue(message.CalculateChecksum() == (ushort)122);

            byte[] bytes = message.GetBytes();

            //verify bytes in the message.
            Assert.IsTrue(bytes[0] == TelemetryMessage.SyncByte1);
            Assert.IsTrue(bytes[1] == TelemetryMessage.SyncByte2);
            Assert.IsTrue(bytes[2] == TelemetryMessage.ID);
            Assert.IsTrue(bytes[3] == (65535 & 0xFF));
            Assert.IsTrue(bytes[4] == ((65535 >> 8) & 0xFF));
            Assert.IsTrue(bytes[5] == 125);
            Assert.IsTrue(bytes[6] == 0);
            Assert.IsTrue(bytes[7] == 32);
            Assert.IsTrue(bytes[8] == 0);
            Assert.IsTrue(bytes[9] == (message.GetCRC() >> 8) & 0xFF);
            Assert.IsTrue(bytes[10] == (message.GetCRC() & 0xFF));
        }
        */
        //Test FlightControllerInterface
        //need to test receiving bad data.
        //bad crc and reject
        //reject up to sync bytes
        //test timeout.
    }

    
}

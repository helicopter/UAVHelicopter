using System;
using System.Net;
using GroundControlStation.Model;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using GroundControlStation.Interfaces;

namespace GCSIntegrationTests
{
    /// <summary>
    /// Tests the interface between the xplane application and the
    /// ground control station.
    /// </summary>
    [TestClass]
    public class XPlaneInterfaceTests
    {
        [TestMethod]
        public void TestReceiveData()
        {
            
            using (SimulatorInterface xInterface = new SimulatorInterface(8089, 49000, IPAddress.Parse("127.0.0.255")))
            {
                xInterface.Open();

                SimulatorTelemetryData data = null;

                data = xInterface.Receive();

                xInterface.Close();

                Assert.IsTrue(data.HeadingDegrees != 0);
                //Assert.IsTrue(data.HeadingDegrees > 8 && data.HeadingDegrees < 11);
            }
        }
    }
}

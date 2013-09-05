using System;
using System.Net;
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
            
            using (XPlaneInterface xInterface = new XPlaneInterface(8089, 49000, IPAddress.Parse("127.0.0.255")))
            {
                xInterface.Open();

                XPlaneData data = null;

                data = xInterface.Receive();

                xInterface.Close();

                Assert.IsTrue(data.YawDegrees != 0);
                //Assert.IsTrue(data.YawDegrees > 8 && data.YawDegrees < 11);
            }
        }
    }
}

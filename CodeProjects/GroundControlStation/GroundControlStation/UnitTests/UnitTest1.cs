using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace UnitTests
{
    [TestClass]
    public class XPlaneInterfaceTest
    {
        //TODO I need a unit test for testing parsing yaw values. I should have a better way of parsing than just bytes.Skip. And I should verify that it's in the proper format for what the mission computer will use. like -180 to 180.
       
        [TestMethod]
        public void TestInterface()
        {
           

        }
    }
}

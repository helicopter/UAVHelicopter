using System;
using System.Collections;
using System.Text;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using GroundControlStation.Views;
using UnitTests.mocks;

namespace UnitTests
{

    /// <summary>
    /// Summary description for GraphingViewTests
    /// </summary>
    [TestClass]
    public class GraphingViewTests
    {
       
        [TestMethod]
        public void TestAddingValues()
        {
            IGraphingView view = new MockGraphingView();

            Assert.IsFalse(view.IsActive);

            view.ActivateView();

            Assert.IsTrue(view.IsActive);

            view.AddValueToGraph(3.14159);

            Assert.IsTrue(((MockGraphingView) view).GetValue() == 3.14159d);

            view.InactivateView();

            Assert.IsFalse(view.IsActive);
        }
    }
}

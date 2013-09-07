using System;
using GroundControlStation.Controller;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;
using GroundControlStation.Views;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using UnitTests.mocks;

namespace UnitTests
{
    [TestClass]
    public class ControllerUnitTests
    {
        //TODO Start making main method and make it so that I can verify that things are graphing correctly.
        //TODO add test for the logic of periodically going out and retrieving data from the sim and HIL, and updating the views with that data

        /// <summary>
        /// Test which validates that the controller is properly updating the views
        /// </summary>
        [TestMethod]
        public void TestUpdatingGraphs()
        {
            SimulatorInterface xplaneInterface = new MockSimulatorInterface(22);
            FlightComputerInterface fcInterface = new MockFlightComputerInterface(22);

            IGraphingView simHeadingGraph = new MockGraphingView();

            IDashboardView dashboard = new MockDashboardView();

            GroundControlStationView gcsView = new GroundControlStationView();
            gcsView.DashboardView = dashboard;
            gcsView.SimHeadingGraph = simHeadingGraph;


            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelmData = new SimulatorTelemetryData();
            model.FcTelmData = new FlightControllerTelemetryData();

            GroundControlStationController gcsController =
                new GroundControlStationController(xplaneInterface, fcInterface);

            gcsController.View = gcsView;
            gcsController.Model = model;

            //Verify that the controller was set on the dashboard.
            Assert.IsTrue(dashboard.Controller == gcsController);

            //Verify that the initial model values are zeros.
            Assert.IsTrue(gcsController.Model.FcTelmData.HeadingDegrees == 0);
            Assert.IsTrue(gcsController.Model.SimTelmData.TrueHeadingDegrees == 0);

            /*
             * Verify that after retrieving mock telemetry data, the model
             * values are no longer zero. 
             */
            gcsController.GetSimulatorTelemetry();
            gcsController.GetFlightComputerTelemetry();

            Assert.IsTrue(gcsController.Model.FcTelmData.HeadingDegrees == 22);
            Assert.IsTrue(gcsController.Model.SimTelmData.TrueHeadingDegrees == 22);

            Assert.IsTrue(((MockGraphingView)simHeadingGraph).Values.Count == 0);

            /*
             * Ensure that since the views haven't been enabled yet, even if
             * new data is received, the views were not updated.
             */
            gcsController.UpdateViews();
            Assert.IsTrue(((MockGraphingView)simHeadingGraph).Values.Count == 0);

            /*
             * Ensure that after the views are enabled, the views get updated.
             */
            gcsController.ActivateSimHeadingGraph();
            gcsController.UpdateViews();
            Assert.IsTrue(((MockGraphingView)simHeadingGraph).Values.Count == 1);
            Assert.IsTrue(((MockGraphingView)simHeadingGraph).Values[0] == 22);

            /*
             * Ensure that after it's disabled, it's not updated.
             */
            gcsController.DeactivateSimHeadingGraph();
            gcsController.UpdateViews();
            Assert.IsTrue(((MockGraphingView)simHeadingGraph).Values.Count == 1);

            //validate that xplane data is returned
            //validate that xplane data is periodically retrieved
            //validate enabling certain graphs based on buttons presses
            //validate that if the graph is enabled, data is added
            //validate that if the graph is not enabled, data is not added.

            //I'll need to pass controller to views.

            //what about threading? I want to avoid manually calling application.doevents.

            //TODO I'll want to put a check in here for getting data from flight computer and checking that.
        }
    }
}

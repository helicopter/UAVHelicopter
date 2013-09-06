using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Model;
using GroundControlStation.Views;

namespace UnitTests.mocks
{
    public class MockDashboardView : IDashboardView
    {
        public GroundControlStation.Controller.GroundControlStationController Controller
        {
            get;
            set;
        }

        public void UpdateLatestValues(List<Tuple<String, String>> items)
        {
            throw new NotImplementedException();
        }
    }
}

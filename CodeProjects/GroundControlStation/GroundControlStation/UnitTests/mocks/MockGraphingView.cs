using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Views;

namespace UnitTests.mocks
{
    public class MockGraphingView : IGraphingView
    {
        private List<double> values = new List<double>();

        private bool isActive = false;

        public List<double> Values
        {
            get { return values; }
        }

        public void AddValueToGraph(double v)
        {
            values.Add(v);
        }

        public double GetValue()
        {
            return values[0];
        }

        public bool IsActive
        {
            get { return isActive; }
        }

        public void ActivateView()
        {
            isActive = true;
        }

        public void DeactivateView()
        {
            isActive = false;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Views
{
    public class GroundControlStationView
    {
        public IDashboardView DashboardView { get; set; }

        public IGraphingView SimHeadingGraph { get; set; }
    }
}

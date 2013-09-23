using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Views
{
    public class GroundControlStationDashboardView
    {
        public IDashboardView DashboardView { get; set; }

        public IGraphingView SimHeadingGraph { get; set; }

        public IGraphingView FcMagYaw { get; set; }

        public IGraphingView FcMagY { get; set; }

        public IGraphingView FcMagZ { get; set; }
    }
}

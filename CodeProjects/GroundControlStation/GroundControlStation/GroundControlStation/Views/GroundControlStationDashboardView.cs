using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Views
{
    public class GroundControlStationDashboardView
    {
        public IDashboardView DashboardForm { get; set; }

        public IGraphingView SimHeadingGraph { get; set; }

        public IGraphingView FcMagYaw { get; set; }

        public IGraphingView YawVelocityDegreesPerSecond { get; set; }

        public IGraphingView YawProportional { get; set; }

        public IGraphingView YawIntegral { get; set; }

        public IGraphingView YawDerivative { get; set; }

        public IGraphingView YawControl { get; set; }

        public GroundControlStationDashboardView()
        {
            DashboardForm = new GroundControlStationForm();

            IGraphingView simHeadingGraph = new GraphForm();

            SimHeadingGraph = new GraphForm();
            FcMagYaw = new GraphForm();
            YawProportional = new GraphForm();
            YawVelocityDegreesPerSecond = new GraphForm();
            YawIntegral = new GraphForm();
            YawDerivative = new GraphForm();
            YawControl = new GraphForm();
        }

    }
}

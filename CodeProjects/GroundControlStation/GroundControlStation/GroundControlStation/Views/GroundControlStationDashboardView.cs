using GroundControlStation.Controller;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Views
{
    public class GroundControlStationDashboardView
    {
        private GroundControlStationController controller;

        public IDashboardView DashboardForm { get; set; }

        public IGraphingView SimHeadingGraph { get; set; }

        public IGraphingView FcMagYaw { get; set; }

        public IGraphingView YawVelocityDegreesPerSecond { get; set; }

        public IGraphingView YawProportional { get; set; }

        public IGraphingView YawIntegral { get; set; }

        public IGraphingView YawDerivative { get; set; }

        public IGraphingView YawControl { get; set; }

        public IGraphingView GainAdjustments { get; set; }

        public GroundControlStationController Controller 
        {
            get
            {
                return controller;
            }
            set{
                controller = value;
                ((GainAdjustmentsForm) GainAdjustments).Controller = Controller;
            }
        } 

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
            GainAdjustments = new GainAdjustmentsForm();
           
           
        }
    }
}

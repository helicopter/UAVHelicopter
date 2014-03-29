using System;
using System.Collections.Generic;
using System.Windows.Forms;
using GroundControlStation.Controller;
using GroundControlStation.Model;

namespace GroundControlStation.Views
{
    public partial class GroundControlStationForm : Form, IDashboardView
    {
        public GroundControlStationController Controller { get; set; }

        public List<IGraphingView> Forms { get; set; }

        public IGraphingView SimHeadingGraph { get; set; }

        public IGraphingView FcMagYaw { get; set; }

        public IGraphingView YawVelocityDegreesPerSecond { get; set; }

        public IGraphingView YawProportional { get; set; }

        public IGraphingView YawIntegral { get; set; }

        public IGraphingView YawDerivative { get; set; }

        public IGraphingView YawControl { get; set; }

        public IGraphingView XProportional { get; set; }

        public IGraphingView XIntegral { get; set; }

        public IGraphingView XDerivative { get; set; }

        public IGraphingView XControl { get; set; }

        public IGraphingView YProportional { get; set; }

        public IGraphingView YIntegral { get; set; }

        public IGraphingView YDerivative { get; set; }

        public IGraphingView YControl { get; set; }

        public IGraphingView GainAdjustments { get; set; }


        public IGraphingView ZNED { get; set; }

        public GroundControlStationForm(GroundControlStationController controller)
        {
            InitializeComponent();

            SimHeadingGraph = new GraphForm();

            FcMagYaw = new GraphForm();

            YawVelocityDegreesPerSecond = new GraphForm();

            YawProportional = new GraphForm();

            YawIntegral = new GraphForm();

            YawDerivative = new GraphForm();

            YawControl = new GraphForm();

            XProportional = new GraphForm();

            XIntegral = new GraphForm();

            XDerivative = new GraphForm();

            XControl = new GraphForm();


            YProportional = new GraphForm();

            YIntegral = new GraphForm();

            YDerivative = new GraphForm();

            YControl = new GraphForm();

            ZNED = new GraphForm();

            GainAdjustmentsForm gainAdjustments = new GainAdjustmentsForm();
            gainAdjustments.Controller = controller;
            GainAdjustments = gainAdjustments;

            Controller = controller;

        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            Controller.Stop();
            base.OnFormClosing(e);
        }

        private void btnHeadingGraph_Click(object sender, EventArgs e)
        {
            ToggleGraph(SimHeadingGraph);
        }

        private void btnFcMagYaw_Click(object sender, EventArgs e)
        {
            ToggleGraph(FcMagYaw);
        }

        private void yawProportional_Click(object sender, EventArgs e)
        {
            ToggleGraph(YawProportional);
        }

        private void btnYawIntegral_Click(object sender, EventArgs e)
        {
            ToggleGraph(YawIntegral);
        }

        private void btnYawDeriv_Click(object sender, EventArgs e)
        {
            ToggleGraph(YawDerivative);
        }

        private void btnYawControl_Click(object sender, EventArgs e)
        {
            ToggleGraph(YawControl);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ToggleGraph(GainAdjustments);
        }


        private void btnXProp_Click(object sender, EventArgs e)
        {
            ToggleGraph(XProportional);
        }

        private void btnXInt_Click(object sender, EventArgs e)
        {
            ToggleGraph(XIntegral);
        }

        private void btnXDer_Click(object sender, EventArgs e)
        {
            ToggleGraph(XDerivative);
        }

        private void btnXCon_Click(object sender, EventArgs e)
        {
            ToggleGraph(XControl);
        }


        private void btnYProp_Click(object sender, EventArgs e)
        {
            ToggleGraph(YProportional);
        }

        private void btnYInt_Click(object sender, EventArgs e)
        {
            ToggleGraph(YIntegral);
        }

        private void btnYDer_Click(object sender, EventArgs e)
        {
            ToggleGraph(YDerivative);
        }

        private void btnYCon_Click(object sender, EventArgs e)
        {
            ToggleGraph(YControl);
        }



        public void UpdateLatestValues(List<Tuple<String, String>> items)
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke((MethodInvoker)delegate() { this.UpdateLatestValues(items); });
            }
            else
            {
                listLatestValues.Items.Clear();
                foreach (Tuple<String, String> item in items)
                {
                    ListViewItem lItem = new ListViewItem();
                    lItem.Text = item.Item1;
                    lItem.SubItems.Add(item.Item2);

                    listLatestValues.Items.Add(lItem);
                }
            }
        }

        private void ToggleGraph(IGraphingView graph)
        {
            if (graph.IsActive)
            {
                graph.DeactivateView();
            }
            else
            {
                graph.ActivateView();
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            ToggleGraph(ZNED);
        }

    }
}

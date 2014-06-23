using GroundControlStation.Controller;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GroundControlStation.Views
{
    public partial class SetpointsForm : Form, IGraphingView
    {
        public SetpointsForm()
        {
            InitializeComponent();
        }


        public void ActivateView()
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke((MethodInvoker)delegate() { this.ActivateView(); });
            }
            else
            {
                this.Show();
            }
        }

        public bool IsActive
        {
            get { return this.Visible; }
        }

        public void DeactivateView()
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke((MethodInvoker)delegate() { this.DeactivateView(); });
            }
            else
            {
                this.Hide();
            }
        }

        public void AddValueToGraph(double value)
        {

        }
        private GroundControlStationController controller;

        public GroundControlStationController Controller
        {
            set
            {
                controller = value;
            }
            get
            {
                return controller;
            }
        }

        private void tbX_ValueChanged(object sender, EventArgs e)
        {
            Controller.Model.XRefSetpoint = (float)tbX.Value * 10;
            Controller.Model.YRefSetpoint = (float)tbY.Value * 10;
            controller.Model.ZRefSetpoint = (float)tbZ.Value * 10;
            Controller.Model.YawRefSetpoint = (float)tbYaw.Value;

            txtX.Text = (tbX.Value * 10).ToString();
            txtY.Text = (tbY.Value * 10).ToString();
            txtZ.Text = (tbZ.Value * 10).ToString();
            txtYaw.Text = (tbYaw.Value).ToString();
        }
    }
}

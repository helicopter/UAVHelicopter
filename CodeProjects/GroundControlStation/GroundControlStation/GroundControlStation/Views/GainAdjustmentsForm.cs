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
    public partial class GainAdjustmentsForm : Form, IGraphingView
    {
        private GroundControlStationController controller;

        public GroundControlStationController Controller
        {
            set
            {
                controller = value;

                //Reset the listeners so when we set the initial values on the bar, it doesn't trigger the value changed event.
                this.tbAntiWindup.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
                this.tbDerivativeGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
                this.tbIntegralGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
                this.tbProportionalGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);

                this.tbAntiWindup.Value = (int)(controller.Model.YawAntiWindupGain * 1000);
                this.tbDerivativeGain.Value = (int)(controller.Model.YawDerivativeGain * 1000);
                this.tbIntegralGain.Value = (int)(controller.Model.YawIntegralGain * 1000);
                this.tbProportionalGain.Value = (int) (controller.Model.YawProportionalGain * 1000);


                txtAntiWindupValue.Text = (tbAntiWindup.Value / 1000).ToString();
                txtDerivativeValue.Text = (tbDerivativeGain.Value / 1000).ToString();
                txtIntegralValue.Text = (tbIntegralGain.Value / 1000).ToString();
                txtProportionalValue.Text = (tbProportionalGain.Value / 1000).ToString();

                this.tbAntiWindup.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
                this.tbDerivativeGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
                this.tbIntegralGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
                this.tbProportionalGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            }
            get
            {
                return controller;
            }
        }

        public GainAdjustmentsForm()
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

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            Controller.Model.YawDerivativeGain = (float) tbDerivativeGain.Value / 1000;
            Controller.Model.YawIntegralGain = (float)tbIntegralGain.Value / 1000;
            Controller.Model.YawProportionalGain = (float)tbProportionalGain.Value / 1000;
            Controller.Model.YawAntiWindupGain = (float)tbAntiWindup.Value / 1000;

            txtAntiWindupValue.Text = (tbAntiWindup.Value / 1000.0).ToString();
            txtDerivativeValue.Text = (tbDerivativeGain.Value / 1000.0).ToString();
            txtIntegralValue.Text = (tbIntegralGain.Value / 1000.0).ToString();
            txtProportionalValue.Text = (tbProportionalGain.Value / 1000.0).ToString();

        }
    }
}

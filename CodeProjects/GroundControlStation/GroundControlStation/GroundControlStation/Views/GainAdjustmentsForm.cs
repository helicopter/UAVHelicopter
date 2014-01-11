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
                removeListeners();

                setInitialValues();

                addListeners();

                Refresh();
            }
            get
            {
                return controller;
            }
        }

        private void setInitialValues()
        {
            this.tbYawAntiWindupGain.Value = (int)(controller.Model.YawAntiWindupGain * 1000);
            this.tbYawDerivativeGain.Value = (int)(controller.Model.YawDerivativeGain * 1000);
            this.tbYawIntegralGain.Value = (int)(controller.Model.YawIntegralGain * 1000);
            this.tbYawProportionalGain.Value = (int)(controller.Model.YawProportionalGain * 1000);

            this.tbXAntiWindupGain.Value = (int)(controller.Model.XAntiWindupGain * 1000);
            this.tbXDerivativeGain.Value = (int)(controller.Model.XDerivativeGain * 1000);
            this.tbXIntegralGain.Value = (int)(controller.Model.XIntegralGain * 1000);
            this.tbXProportionalGain.Value = (int)(controller.Model.XProportionalGain * 1000);

            this.tbYAntiWindupGain.Value = (int)(controller.Model.YAntiWindupGain * 1000);
            this.tbYDerivativeGain.Value = (int)(controller.Model.YDerivativeGain * 1000);
            this.tbYIntegralGain.Value = (int)(controller.Model.YIntegralGain * 1000);
            this.tbYProportionalGain.Value = (int)(controller.Model.YProportionalGain * 1000);

            this.tbZAntiWindupGain.Value = (int)(controller.Model.ZAntiWindupGain * 1000);
            this.tbZDerivativeGain.Value = (int)(controller.Model.ZDerivativeGain * 1000);
            this.tbZIntegralGain.Value = (int)(controller.Model.ZIntegralGain * 1000);
            this.tbZProportionalGain.Value = (int)(controller.Model.ZProportionalGain * 1000);

            this.tbLongitudeControlGain.Value = (int)(controller.Model.LongitudeInnerLoopGain * 1000);
            this.tbLateralControlGain.Value = (int)(controller.Model.LateralInnerLoopGain * 1000);

            this.tbPitchAngularVelocityGain.Value = (int)(controller.Model.PitchAngularVelocityGain * 1000);
            this.tbRollAngularVelocityGain.Value = (int)(controller.Model.RollAngularVelocityGain * 1000);

            txtYawAntiWindupValue.Text = (tbYawAntiWindupGain.Value / 1000).ToString();
            txtYawDerivativeValue.Text = (tbYawDerivativeGain.Value / 1000).ToString();
            txtYawIntegralValue.Text = (tbYawIntegralGain.Value / 1000).ToString();
            txtYawProportionalValue.Text = (tbYawProportionalGain.Value / 1000).ToString();

            txtXAntiWindupValue.Text = (tbXAntiWindupGain.Value / 1000).ToString();
            txtXDerivativeValue.Text = (tbXDerivativeGain.Value / 1000).ToString();
            txtXIntegralValue.Text = (tbXIntegralGain.Value / 1000).ToString();
            txtXProportionalValue.Text = (tbXProportionalGain.Value / 1000).ToString();

            txtYAntiWindupValue.Text = (tbYAntiWindupGain.Value / 1000).ToString();
            txtYDerivativeValue.Text = (tbYDerivativeGain.Value / 1000).ToString();
            txtYIntegralValue.Text = (tbYIntegralGain.Value / 1000).ToString();
            txtYProportionalValue.Text = (tbYProportionalGain.Value / 1000).ToString();

            txtZAntiWindupValue.Text = (tbZAntiWindupGain.Value / 1000).ToString();
            txtZDerivativeValue.Text = (tbZDerivativeGain.Value / 1000).ToString();
            txtZIntegralValue.Text = (tbZIntegralGain.Value / 1000).ToString();
            txtZProportionalValue.Text = (tbZProportionalGain.Value / 1000).ToString();

            txtLongitudeControlGainValue.Text = (tbLongitudeControlGain.Value / 1000).ToString();
            txtLateralControlGainValue.Text = (tbLateralControlGain.Value / 1000).ToString();

            txtPitchAngularVelocityValue.Text = (tbPitchAngularVelocityGain.Value / 1000).ToString();
            txtRollAngularVelocityValue.Text = (tbRollAngularVelocityGain.Value / 1000).ToString(); 
        }

        private void addListeners()
        {
            this.tbYawAntiWindupGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYawDerivativeGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYawIntegralGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYawProportionalGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbXAntiWindupGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbXDerivativeGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbXIntegralGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbXProportionalGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbYAntiWindupGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYDerivativeGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYIntegralGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYProportionalGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbZAntiWindupGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbZDerivativeGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbZIntegralGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbZProportionalGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbLongitudeControlGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbLateralControlGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbPitchAngularVelocityGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbRollAngularVelocityGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
        }

        private void removeListeners()
        {
            this.tbYawAntiWindupGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYawDerivativeGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYawIntegralGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYawProportionalGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbXAntiWindupGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbXDerivativeGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbXIntegralGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbXProportionalGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbYAntiWindupGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYDerivativeGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYIntegralGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbYProportionalGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);


            this.tbZAntiWindupGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbZDerivativeGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbZIntegralGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbZProportionalGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbLongitudeControlGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbLateralControlGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);

            this.tbPitchAngularVelocityGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
            this.tbRollAngularVelocityGain.ValueChanged -= new System.EventHandler(this.trackBar1_ValueChanged);
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
            Controller.Model.YawDerivativeGain = (float) tbYawDerivativeGain.Value / 1000;
            Controller.Model.YawIntegralGain = (float)tbYawIntegralGain.Value / 1000;
            Controller.Model.YawProportionalGain = (float)tbYawProportionalGain.Value / 1000;
            Controller.Model.YawAntiWindupGain = (float)tbYawAntiWindupGain.Value / 1000;

            txtYawAntiWindupValue.Text = (tbYawAntiWindupGain.Value / 1000.0).ToString();
            txtYawDerivativeValue.Text = (tbYawDerivativeGain.Value / 1000.0).ToString();
            txtYawIntegralValue.Text = (tbYawIntegralGain.Value / 1000.0).ToString();
            txtYawProportionalValue.Text = (tbYawProportionalGain.Value / 1000.0).ToString();

            Controller.Model.XDerivativeGain = (float)tbXDerivativeGain.Value / 1000;
            Controller.Model.XIntegralGain = (float)tbXIntegralGain.Value / 1000;
            Controller.Model.XProportionalGain = (float)tbXProportionalGain.Value / 1000;
            Controller.Model.XAntiWindupGain = (float)tbXAntiWindupGain.Value / 1000;

            txtXAntiWindupValue.Text = (tbXAntiWindupGain.Value / 1000.0).ToString();
            txtXDerivativeValue.Text = (tbXDerivativeGain.Value / 1000.0).ToString();
            txtXIntegralValue.Text = (tbXIntegralGain.Value / 1000.0).ToString();
            txtXProportionalValue.Text = (tbXProportionalGain.Value / 1000.0).ToString();


            Controller.Model.YDerivativeGain = (float)tbYDerivativeGain.Value / 1000;
            Controller.Model.YIntegralGain = (float)tbYIntegralGain.Value / 1000;
            Controller.Model.YProportionalGain = (float)tbYProportionalGain.Value / 1000;
            Controller.Model.YAntiWindupGain = (float)tbYAntiWindupGain.Value / 1000;

            txtYAntiWindupValue.Text = (tbYAntiWindupGain.Value / 1000.0).ToString();
            txtYDerivativeValue.Text = (tbYDerivativeGain.Value / 1000.0).ToString();
            txtYIntegralValue.Text = (tbYIntegralGain.Value / 1000.0).ToString();
            txtYProportionalValue.Text = (tbYProportionalGain.Value / 1000.0).ToString();

            Controller.Model.ZDerivativeGain = (float)tbZDerivativeGain.Value / 1000;
            Controller.Model.ZIntegralGain = (float)tbZIntegralGain.Value / 1000;
            Controller.Model.ZProportionalGain = (float)tbZProportionalGain.Value / 1000;
            Controller.Model.ZAntiWindupGain = (float)tbZAntiWindupGain.Value / 1000;

            txtZAntiWindupValue.Text = (tbZAntiWindupGain.Value / 1000.0).ToString();
            txtZDerivativeValue.Text = (tbZDerivativeGain.Value / 1000.0).ToString();
            txtZIntegralValue.Text = (tbZIntegralGain.Value / 1000.0).ToString();
            txtZProportionalValue.Text = (tbZProportionalGain.Value / 1000.0).ToString();

            txtLongitudeControlGainValue.Text = (tbLongitudeControlGain.Value / 1000.0).ToString();
            txtLateralControlGainValue.Text = (tbLateralControlGain.Value / 1000.0).ToString();

            txtPitchAngularVelocityValue.Text = (tbPitchAngularVelocityGain.Value / 1000.0).ToString();
            txtRollAngularVelocityValue.Text = (tbRollAngularVelocityGain.Value / 1000.0).ToString();
        }
    }
}

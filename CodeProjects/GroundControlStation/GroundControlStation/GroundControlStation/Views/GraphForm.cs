using GroundControlStation.Controller;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace GroundControlStation.Views
{
    public partial class GraphForm : Form, IGraphingView
    {
        private int xAxisCounter = 0;

        private const int MaxNumGraphingPoints = 30;

        public GraphForm()
        {
            InitializeComponent();
        }

        public GraphForm(string title)
        {
            InitializeComponent();
            this.Text = title;
        }


        public void PlotLineGraph(double dataPointValue)
        {
            if (this.TelemetryGraph.Series["TelemData"].Points.Count >= 30)
            {
                this.TelemetryGraph.Series["TelemData"].Points.RemoveAt(0);
            }

            DataPoint datapoint = new DataPoint(this.TelemetryGraph.Series["TelemData"].Points.Count, dataPointValue);
            
            this.TelemetryGraph.Series["TelemData"].Points.Add(datapoint);
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
            if (this.InvokeRequired)
            {
                this.BeginInvoke((MethodInvoker) delegate() { this.AddValueToGraph(value); });
            }
            else
            {
                if (TelemetryGraph.Series[0].Points.Count >= MaxNumGraphingPoints)
                {
                    TelemetryGraph.Series[0].Points.RemoveAt(0);
                }

                DataPoint datapoint = new DataPoint(xAxisCounter++, value);

                TelemetryGraph.Series[0].Points.Add(datapoint);

                TelemetryGraph.ChartAreas[0].RecalculateAxesScale();
            }
        }
    }
}

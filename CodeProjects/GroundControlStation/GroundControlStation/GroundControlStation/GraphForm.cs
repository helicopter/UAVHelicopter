using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace GroundControlStation
{
    public partial class GraphForm : Form
    {
        public GraphForm()
        {
            InitializeComponent();
        }

        public void PlotLineGraph(double dataPointValue)
        {
            if (this.TelemetryGraph.Series["TelemData"].Points.Count >= 30)
            {
                this.TelemetryGraph.Series["TelemData"].Points.RemoveAt(0);
            }

            DataPoint datapoint = new DataPoint(this.TelemetryGraph.Series["TelemData"].Points.Count, dataPointValue);

            this.TelemetryGraph.Series["TelemData"].Points.Add(datapoint);
            /*
            this.TelemetryGraph.Series["TelemData"].Points.Clear();

            for (int i = 0; i < values.Count; i++)
            {
                DataPoint datapoint = new DataPoint(i, values[i]);

                this.TelemetryGraph.Series["TelemData"].Points.Add(datapoint);

            }
             * */
        }
    }
}

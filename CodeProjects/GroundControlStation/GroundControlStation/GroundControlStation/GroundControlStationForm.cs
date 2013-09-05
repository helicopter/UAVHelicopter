using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GroundControlStation
{
    public partial class GroundControlStationForm : Form
    {
        private GraphForm headingGraph = new GraphForm();

        public GroundControlStationForm()
        {
            InitializeComponent();
        }

        private void btnHeadingGraph_Click(object sender, EventArgs e)
        {
            if (!headingGraph.Visible)
            {
                headingGraph.Show();

                headingGraph.PlotLineGraph(3.12);
                headingGraph.PlotLineGraph(5.13);
                headingGraph.PlotLineGraph(3.12);
                headingGraph.PlotLineGraph(12.44);
                headingGraph.PlotLineGraph(33.88);
                headingGraph.PlotLineGraph(17.22);
                headingGraph.PlotLineGraph(3.12);
                headingGraph.PlotLineGraph(5.13);
            }
            else
            {
                headingGraph.Hide();
            }
        }
    }
}

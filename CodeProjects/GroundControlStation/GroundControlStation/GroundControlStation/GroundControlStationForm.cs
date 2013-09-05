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
            }
            else
            {
                headingGraph.Hide();
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Windows.Forms;
using GroundControlStation.Controller;
using GroundControlStation.Model;

namespace GroundControlStation.Views
{
    public partial class GroundControlStationForm : Form, IDashboardView
    {
        private GraphForm headingGraph = new GraphForm();

        public GroundControlStationController Controller { get; set; }


        public void UpdateLatestValues(List<Tuple<String,String>> items)
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

        public GroundControlStationForm()
        {
            InitializeComponent();
        }

        private void btnHeadingGraph_Click(object sender, EventArgs e)
        {
            Controller.ToggleSimHeadingGraph();
        }
    }
}

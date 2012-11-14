using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using Helicopter1;


namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        Receiver2 receiver = new Receiver2();

        bool done = false;

        private void button1_Click(object sender, EventArgs e)
        {
            if (done)
            {
                receiver.done = true;
                done = false;
            }
            else
            {

                receiver.done = false;
                receiver = new Receiver2();
                receiver.Start();
                done = true;
            }

            /*
            List<double> values = new List<double>();
            values.Add(2.2);
            values.Add(3.3);
            values.Add(2.2);
            values.Add(3.3);
            values.Add(22.2);
            values.Add(7.3);
            values.Add(1.2);
            values.Add(9.3);
            plotLineGraph(values, chart1);*/

        }


        public static void plotLineGraphChart1(List<double> values)
        {
            plotLineGraph(values, Form1.chart1);
        }

        public static void plotLineGraph(List<double> values, Chart cart)
        {
            cart.Series["Series1"].Points.Clear();
            for (int i = 0; i < values.Count; i++)
            {
                System.Windows.Forms.DataVisualization.Charting.DataPoint dataPoint1 = new System.Windows.Forms.DataVisualization.Charting.DataPoint(i, values[i]);
                cart.Series["Series1"].Points.Add(dataPoint1);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {

            //ControlLaw.referenceMeasurementNED.Orientation.Sai_yawDegrees = ControlLaw.referenceMeasurementNED.Orientation.Sai_yawDegrees + 3;
            //Util.K_ETA_SAI = 0;
            //Util.K_R = 0;


            
         //   Util.K_V = 0;
        //    Util.K_ETA_Y = 0;
         //   ControlLaw.referenceMeasurementNED.Position.YLongitudePosition = ControlLaw.referenceMeasurementNED.Position.YLongitudePosition + 3;
      //      ControlLaw.referenceMeasurementNED.Position.XLatitudePosition = ControlLaw.referenceMeasurementNED.Position.XLatitudePosition - 3;
           // Util.K_Y = 0.0;
       //     Util.K_PHI = .008;
         //   ControlLaw.referenceMeasurementNED.Orientation.Sai_yawDegrees = ControlLaw.referenceMeasurementNED.Orientation.Sai_yawDegrees - 1;


            //Util.K_ETA_X = 0;
            //Util.K_U = 0;

            /*
            ControlLaw.referenceMeasurementNED.Position.ZAltitudePosition -= 5;
            ControlLaw.referenceMeasurementNED.Position.XLatitudePosition += 5;
            ControlLaw.referenceMeasurementNED.Position.YLongitudePosition += 5;*/

            ControlLaw.referenceMeasurementNED.Velocity.XVelocityBodyFrame += 5;

          //  Util.K_U = 0;
          //  Util.K_ETA_X = 0;
          //  Form1.trackBar2.Value = 0;

            //Util.K_W = 0;
        //    Util.K_ETA_Y = 0;
        //    Util.K_V = 0;
        }

    }
}

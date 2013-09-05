namespace GroundControlStation
{
    partial class GraphForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.TelemetryGraph = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.TelemetryGraph)).BeginInit();
            this.SuspendLayout();
            // 
            // TelemetryGraph
            // 
            this.TelemetryGraph.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.AxisX.Title = "Time Interval";
            chartArea1.AxisY.Title = "Telem Value";
            chartArea1.Name = "ChartArea1";
            this.TelemetryGraph.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.TelemetryGraph.Legends.Add(legend1);
            this.TelemetryGraph.Location = new System.Drawing.Point(2, 2);
            this.TelemetryGraph.Name = "TelemetryGraph";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.IsVisibleInLegend = false;
            series1.Legend = "Legend1";
            series1.Name = "TelemData";
            this.TelemetryGraph.Series.Add(series1);
            this.TelemetryGraph.Size = new System.Drawing.Size(458, 317);
            this.TelemetryGraph.TabIndex = 0;
            this.TelemetryGraph.Text = "Telemetry Data";
            // 
            // GraphForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(459, 318);
            this.Controls.Add(this.TelemetryGraph);
            this.Name = "GraphForm";
            this.Text = "Telemetry Graphs";
            ((System.ComponentModel.ISupportInitialize)(this.TelemetryGraph)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart TelemetryGraph;
    }
}
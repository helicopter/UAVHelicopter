namespace WindowsFormsApplication1
{
    partial class Form1
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
            System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title2 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title3 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend4 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title4 = new System.Windows.Forms.DataVisualization.Charting.Title();
            chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            chart2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            chart3 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            chart4 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.button1 = new System.Windows.Forms.Button();
            trackBar1 = new System.Windows.Forms.TrackBar();
            textBox1 = new System.Windows.Forms.TextBox();
            this.button2 = new System.Windows.Forms.Button();
            trackBar2 = new System.Windows.Forms.TrackBar();
            textBox2 = new System.Windows.Forms.TextBox();
            textBox3 = new System.Windows.Forms.TextBox();
            trackBar3 = new System.Windows.Forms.TrackBar();
            textBox4 = new System.Windows.Forms.TextBox();
            trackBar4 = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(chart1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(chart2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(chart3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(chart4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar4)).BeginInit();
            this.SuspendLayout();
            // 
            // chart1
            // 
            chartArea1.Name = "ChartArea1";
            chart1.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            chart1.Legends.Add(legend1);
            chart1.Location = new System.Drawing.Point(0, 2);
            chart1.Name = "chart1";
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            chart1.Series.Add(series1);
            chart1.Size = new System.Drawing.Size(469, 187);
            chart1.TabIndex = 0;
            chart1.Text = "chart1";
            title1.Name = "Title1";
            title1.Text = "Yaw Errors";
            chart1.Titles.Add(title1);
            chart1.Click += new System.EventHandler(chart1_Click);
            // 
            // chart2
            // 
            chartArea2.Name = "ChartArea1";
            chart2.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            chart2.Legends.Add(legend2);
            chart2.Location = new System.Drawing.Point(0, 195);
            chart2.Name = "chart2";
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Legend = "Legend1";
            series2.Name = "Series1";
            chart2.Series.Add(series2);
            chart2.Size = new System.Drawing.Size(469, 167);
            chart2.TabIndex = 1;
            chart2.Text = "chart2";
            title2.Name = "Title1";
            title2.Text = "Derivative Yaw";
            chart2.Titles.Add(title2);
            // 
            // chart3
            // 
            chartArea3.Name = "ChartArea1";
            chart3.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            chart3.Legends.Add(legend3);
            chart3.Location = new System.Drawing.Point(475, 195);
            chart3.Name = "chart3";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series3.Legend = "Legend1";
            series3.Name = "Series1";
            chart3.Series.Add(series3);
            chart3.Size = new System.Drawing.Size(493, 167);
            chart3.TabIndex = 2;
            chart3.Text = "chart3";
            title3.Name = "Title1";
            title3.Text = "Integral Yaw";
            chart3.Titles.Add(title3);
            // 
            // chart4
            // 
            chartArea4.Name = "ChartArea1";
            chart4.ChartAreas.Add(chartArea4);
            legend4.Name = "Legend1";
            chart4.Legends.Add(legend4);
            chart4.Location = new System.Drawing.Point(475, 2);
            chart4.Name = "chart4";
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series4.Legend = "Legend1";
            series4.Name = "Series1";
            chart4.Series.Add(series4);
            chart4.Size = new System.Drawing.Size(493, 187);
            chart4.TabIndex = 3;
            chart4.Text = "chart4";
            title4.Name = "Title1";
            title4.Text = "Ped Control";
            chart4.Titles.Add(title4);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(974, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // trackBar1
            // 
            trackBar1.LargeChange = 2;
            trackBar1.Location = new System.Drawing.Point(12, 378);
            trackBar1.Maximum = 9000;
            trackBar1.Minimum = -9000;
            trackBar1.Name = "trackBar1";
            trackBar1.Size = new System.Drawing.Size(879, 45);
            trackBar1.TabIndex = 6;
            // 
            // textBox1
            // 
            textBox1.Location = new System.Drawing.Point(272, 429);
            textBox1.Name = "textBox1";
            textBox1.Size = new System.Drawing.Size(122, 20);
            textBox1.TabIndex = 7;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(974, 67);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 8;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // trackBar2
            // 
            trackBar2.LargeChange = 2;
            trackBar2.Location = new System.Drawing.Point(12, 477);
            trackBar2.Maximum = 9000;
            trackBar2.Minimum = -9000;
            trackBar2.Name = "trackBar2";
            trackBar2.Size = new System.Drawing.Size(879, 45);
            trackBar2.TabIndex = 9;
            // 
            // textBox2
            // 
            textBox2.Location = new System.Drawing.Point(272, 528);
            textBox2.Name = "textBox2";
            textBox2.Size = new System.Drawing.Size(122, 20);
            textBox2.TabIndex = 10;
            // 
            // textBox3
            // 
            textBox3.Location = new System.Drawing.Point(272, 704);
            textBox3.Name = "textBox3";
            textBox3.Size = new System.Drawing.Size(122, 20);
            textBox3.TabIndex = 14;
            // 
            // trackBar3
            // 
            trackBar3.LargeChange = 2;
            trackBar3.Location = new System.Drawing.Point(12, 653);
            trackBar3.Maximum = 180000;
            trackBar3.Minimum = -180000;
            trackBar3.Name = "trackBar3";
            trackBar3.Size = new System.Drawing.Size(879, 45);
            trackBar3.TabIndex = 13;
            // 
            // textBox4
            // 
            textBox4.Location = new System.Drawing.Point(272, 605);
            textBox4.Name = "textBox4";
            textBox4.Size = new System.Drawing.Size(122, 20);
            textBox4.TabIndex = 12;
            // 
            // trackBar4
            // 
            trackBar4.LargeChange = 2;
            trackBar4.Location = new System.Drawing.Point(12, 554);
            trackBar4.Maximum = 9000;
            trackBar4.Minimum = -9000;
            trackBar4.Name = "trackBar4";
            trackBar4.Size = new System.Drawing.Size(879, 45);
            trackBar4.TabIndex = 11;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1068, 742);
            this.Controls.Add(textBox3);
            this.Controls.Add(trackBar3);
            this.Controls.Add(textBox4);
            this.Controls.Add(trackBar4);
            this.Controls.Add(textBox2);
            this.Controls.Add(trackBar2);
            this.Controls.Add(this.button2);
            this.Controls.Add(textBox1);
            this.Controls.Add(trackBar1);
            this.Controls.Add(this.button1);
            this.Controls.Add(chart4);
            this.Controls.Add(chart3);
            this.Controls.Add(chart2);
            this.Controls.Add(chart1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(chart1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(chart2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(chart3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(chart4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(trackBar4)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        public static System.Windows.Forms.TextBox textBox3;
        public static System.Windows.Forms.TrackBar trackBar3;
        public static System.Windows.Forms.TextBox textBox4;
        public static System.Windows.Forms.TrackBar trackBar4;
        public static System.Windows.Forms.TrackBar trackBar2;
        public static System.Windows.Forms.TextBox textBox2;
        public static System.Windows.Forms.TrackBar trackBar1;
        public static System.Windows.Forms.TextBox textBox1;
        public static System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        public static System.Windows.Forms.DataVisualization.Charting.Chart chart2;
        public static System.Windows.Forms.DataVisualization.Charting.Chart chart3;
        public static System.Windows.Forms.DataVisualization.Charting.Chart chart4;
    }
}


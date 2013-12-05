namespace GroundControlStation.Views
{
    partial class GroundControlStationForm
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
            System.Windows.Forms.ListViewItem listViewItem2 = new System.Windows.Forms.ListViewItem(new string[] {
            "Test1",
            "test2"}, -1);
            this.listLatestValues = new System.Windows.Forms.ListView();
            this.TelemName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TelemValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnYawControl = new System.Windows.Forms.Button();
            this.btnYawDeriv = new System.Windows.Forms.Button();
            this.btnYawIntegral = new System.Windows.Forms.Button();
            this.yawProportional = new System.Windows.Forms.Button();
            this.btnFcMagX = new System.Windows.Forms.Button();
            this.btnHeadingGraph = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listLatestValues
            // 
            this.listLatestValues.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.listLatestValues.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.TelemName,
            this.TelemValue});
            this.listLatestValues.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem2});
            this.listLatestValues.Location = new System.Drawing.Point(1, -1);
            this.listLatestValues.Name = "listLatestValues";
            this.listLatestValues.Size = new System.Drawing.Size(173, 632);
            this.listLatestValues.TabIndex = 2;
            this.listLatestValues.UseCompatibleStateImageBehavior = false;
            this.listLatestValues.View = System.Windows.Forms.View.Details;
            // 
            // TelemName
            // 
            this.TelemName.Text = "TelemName";
            this.TelemName.Width = 100;
            // 
            // TelemValue
            // 
            this.TelemValue.Text = "TelemValue";
            this.TelemValue.Width = 69;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.panel1.AutoScroll = true;
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.btnYawControl);
            this.panel1.Controls.Add(this.btnYawDeriv);
            this.panel1.Controls.Add(this.btnYawIntegral);
            this.panel1.Controls.Add(this.yawProportional);
            this.panel1.Controls.Add(this.btnFcMagX);
            this.panel1.Controls.Add(this.btnHeadingGraph);
            this.panel1.Location = new System.Drawing.Point(177, -1);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(137, 632);
            this.panel1.TabIndex = 3;
            // 
            // btnYawControl
            // 
            this.btnYawControl.Location = new System.Drawing.Point(0, 149);
            this.btnYawControl.Name = "btnYawControl";
            this.btnYawControl.Size = new System.Drawing.Size(129, 23);
            this.btnYawControl.TabIndex = 5;
            this.btnYawControl.Text = "Yaw Control";
            this.btnYawControl.UseVisualStyleBackColor = true;
            this.btnYawControl.Click += new System.EventHandler(this.btnYawControl_Click);
            // 
            // btnYawDeriv
            // 
            this.btnYawDeriv.Location = new System.Drawing.Point(0, 120);
            this.btnYawDeriv.Name = "btnYawDeriv";
            this.btnYawDeriv.Size = new System.Drawing.Size(129, 23);
            this.btnYawDeriv.TabIndex = 4;
            this.btnYawDeriv.Text = "Yaw Derivative";
            this.btnYawDeriv.UseVisualStyleBackColor = true;
            this.btnYawDeriv.Click += new System.EventHandler(this.btnYawDeriv_Click);
            // 
            // btnYawIntegral
            // 
            this.btnYawIntegral.Location = new System.Drawing.Point(0, 91);
            this.btnYawIntegral.Name = "btnYawIntegral";
            this.btnYawIntegral.Size = new System.Drawing.Size(129, 23);
            this.btnYawIntegral.TabIndex = 3;
            this.btnYawIntegral.Text = "Yaw Integral";
            this.btnYawIntegral.UseVisualStyleBackColor = true;
            this.btnYawIntegral.Click += new System.EventHandler(this.btnYawIntegral_Click);
            // 
            // yawProportional
            // 
            this.yawProportional.Location = new System.Drawing.Point(3, 62);
            this.yawProportional.Name = "yawProportional";
            this.yawProportional.Size = new System.Drawing.Size(129, 23);
            this.yawProportional.TabIndex = 2;
            this.yawProportional.Text = "Yaw Proportional";
            this.yawProportional.UseVisualStyleBackColor = true;
            this.yawProportional.Click += new System.EventHandler(this.yawProportional_Click);
            // 
            // btnFcMagX
            // 
            this.btnFcMagX.Location = new System.Drawing.Point(3, 33);
            this.btnFcMagX.Name = "btnFcMagX";
            this.btnFcMagX.Size = new System.Drawing.Size(129, 23);
            this.btnFcMagX.TabIndex = 1;
            this.btnFcMagX.Text = "MagYaw";
            this.btnFcMagX.UseVisualStyleBackColor = true;
            this.btnFcMagX.Click += new System.EventHandler(this.btnFcMagX_Click);
            // 
            // btnHeadingGraph
            // 
            this.btnHeadingGraph.Location = new System.Drawing.Point(3, 3);
            this.btnHeadingGraph.Name = "btnHeadingGraph";
            this.btnHeadingGraph.Size = new System.Drawing.Size(129, 23);
            this.btnHeadingGraph.TabIndex = 0;
            this.btnHeadingGraph.Text = "MagHeadingDegrees Graph";
            this.btnHeadingGraph.UseVisualStyleBackColor = true;
            this.btnHeadingGraph.Click += new System.EventHandler(this.btnHeadingGraph_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(0, 490);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(129, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "Gain Adjustments";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // GroundControlStationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(369, 633);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.listLatestValues);
            this.Name = "GroundControlStationForm";
            this.Text = "GroundControlStation";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listLatestValues;
        private System.Windows.Forms.ColumnHeader TelemName;
        private System.Windows.Forms.ColumnHeader TelemValue;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnHeadingGraph;
        private System.Windows.Forms.Button btnFcMagX;
        private System.Windows.Forms.Button yawProportional;
        private System.Windows.Forms.Button btnYawIntegral;
        private System.Windows.Forms.Button btnYawDeriv;
        private System.Windows.Forms.Button btnYawControl;
        private System.Windows.Forms.Button button1;
    }
}


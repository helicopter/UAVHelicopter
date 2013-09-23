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
            System.Windows.Forms.ListViewItem listViewItem1 = new System.Windows.Forms.ListViewItem(new string[] {
            "Test1",
            "test2"}, -1);
            this.listLatestValues = new System.Windows.Forms.ListView();
            this.TelemName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TelemValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnFcMagX = new System.Windows.Forms.Button();
            this.btnHeadingGraph = new System.Windows.Forms.Button();
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
            listViewItem1});
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
            this.panel1.Controls.Add(this.btnFcMagX);
            this.panel1.Controls.Add(this.btnHeadingGraph);
            this.panel1.Location = new System.Drawing.Point(177, -1);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(137, 632);
            this.panel1.TabIndex = 3;
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
    }
}


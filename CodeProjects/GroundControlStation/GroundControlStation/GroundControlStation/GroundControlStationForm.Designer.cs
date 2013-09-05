﻿namespace GroundControlStation
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
            System.Windows.Forms.ListViewItem listViewItem3 = new System.Windows.Forms.ListViewItem(new string[] {
            "Test1",
            "test2"}, -1);
            this.listView1 = new System.Windows.Forms.ListView();
            this.TelemName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.TelemValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnHeadingGraph = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listView1
            // 
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.TelemName,
            this.TelemValue});
            this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem3});
            this.listView1.Location = new System.Drawing.Point(1, -1);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(173, 632);
            this.listView1.TabIndex = 2;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
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
            this.panel1.Controls.Add(this.btnHeadingGraph);
            this.panel1.Location = new System.Drawing.Point(177, -1);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(137, 632);
            this.panel1.TabIndex = 3;
            // 
            // btnHeadingGraph
            // 
            this.btnHeadingGraph.Location = new System.Drawing.Point(3, 3);
            this.btnHeadingGraph.Name = "btnHeadingGraph";
            this.btnHeadingGraph.Size = new System.Drawing.Size(129, 23);
            this.btnHeadingGraph.TabIndex = 0;
            this.btnHeadingGraph.Text = "Heading Graph";
            this.btnHeadingGraph.UseVisualStyleBackColor = true;
            this.btnHeadingGraph.Click += new System.EventHandler(this.btnHeadingGraph_Click);
            // 
            // GroundControlStationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(369, 633);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.listView1);
            this.Name = "GroundControlStationForm";
            this.Text = "GroundControlStation";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader TelemName;
        private System.Windows.Forms.ColumnHeader TelemValue;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnHeadingGraph;
    }
}

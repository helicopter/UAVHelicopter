namespace GroundControlStation.Views
{
    partial class SetpointsForm
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
            this.tbX = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.txtX = new System.Windows.Forms.TextBox();
            this.txtY = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbY = new System.Windows.Forms.TrackBar();
            this.txtYaw = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbYaw = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.tbX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbYaw)).BeginInit();
            this.SuspendLayout();
            // 
            // tbX
            // 
            this.tbX.Location = new System.Drawing.Point(66, 12);
            this.tbX.Maximum = 500;
            this.tbX.Minimum = -500;
            this.tbX.Name = "tbX";
            this.tbX.Size = new System.Drawing.Size(459, 45);
            this.tbX.TabIndex = 0;
            this.tbX.ValueChanged += new System.EventHandler(this.tbX_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "X:";
            // 
            // txtX
            // 
            this.txtX.Location = new System.Drawing.Point(531, 16);
            this.txtX.Name = "txtX";
            this.txtX.Size = new System.Drawing.Size(100, 20);
            this.txtX.TabIndex = 2;
            // 
            // txtY
            // 
            this.txtY.Location = new System.Drawing.Point(531, 56);
            this.txtY.Name = "txtY";
            this.txtY.Size = new System.Drawing.Size(100, 20);
            this.txtY.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(17, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Y:";
            // 
            // tbY
            // 
            this.tbY.Location = new System.Drawing.Point(66, 52);
            this.tbY.Maximum = 500;
            this.tbY.Minimum = -500;
            this.tbY.Name = "tbY";
            this.tbY.Size = new System.Drawing.Size(459, 45);
            this.tbY.TabIndex = 3;
            this.tbY.ValueChanged += new System.EventHandler(this.tbX_ValueChanged);
            // 
            // txtYaw
            // 
            this.txtYaw.Location = new System.Drawing.Point(531, 107);
            this.txtYaw.Name = "txtYaw";
            this.txtYaw.Size = new System.Drawing.Size(100, 20);
            this.txtYaw.TabIndex = 8;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 114);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Yaw";
            // 
            // tbYaw
            // 
            this.tbYaw.Location = new System.Drawing.Point(66, 103);
            this.tbYaw.Maximum = 360;
            this.tbYaw.Name = "tbYaw";
            this.tbYaw.Size = new System.Drawing.Size(459, 45);
            this.tbYaw.TabIndex = 6;
            this.tbYaw.ValueChanged += new System.EventHandler(this.tbX_ValueChanged);
            // 
            // SetpointsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(643, 188);
            this.Controls.Add(this.txtYaw);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbYaw);
            this.Controls.Add(this.txtY);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbY);
            this.Controls.Add(this.txtX);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbX);
            this.Name = "SetpointsForm";
            this.Text = "SetpointsForm";
            ((System.ComponentModel.ISupportInitialize)(this.tbX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbYaw)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar tbX;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtX;
        private System.Windows.Forms.TextBox txtY;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar tbY;
        private System.Windows.Forms.TextBox txtYaw;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar tbYaw;
    }
}
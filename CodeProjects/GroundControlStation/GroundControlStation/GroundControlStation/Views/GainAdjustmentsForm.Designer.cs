namespace GroundControlStation.Views
{
    partial class GainAdjustmentsForm
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
            this.tbIntegralGain = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbDerivativeGain = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.tbAntiWindup = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.tbProportionalGain = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this.txtIntegralValue = new System.Windows.Forms.TextBox();
            this.txtDerivativeValue = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.txtAntiWindupValue = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.txtProportionalValue = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.tbIntegralGain)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbDerivativeGain)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbAntiWindup)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbProportionalGain)).BeginInit();
            this.SuspendLayout();
            // 
            // tbIntegralGain
            // 
            this.tbIntegralGain.Location = new System.Drawing.Point(12, 38);
            this.tbIntegralGain.Maximum = 9000;
            this.tbIntegralGain.Minimum = -9000;
            this.tbIntegralGain.Name = "tbIntegralGain";
            this.tbIntegralGain.Size = new System.Drawing.Size(648, 45);
            this.tbIntegralGain.TabIndex = 0;
            this.tbIntegralGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(296, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "YawIntegralGain";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(296, 103);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(98, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "YawDerivativeGain";
            // 
            // tbDerivativeGain
            // 
            this.tbDerivativeGain.Location = new System.Drawing.Point(12, 132);
            this.tbDerivativeGain.Maximum = 9000;
            this.tbDerivativeGain.Minimum = -9000;
            this.tbDerivativeGain.Name = "tbDerivativeGain";
            this.tbDerivativeGain.Size = new System.Drawing.Size(648, 45);
            this.tbDerivativeGain.TabIndex = 2;
            this.tbDerivativeGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(296, 208);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(105, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "YawAntiWindupGain";
            // 
            // tbAntiWindup
            // 
            this.tbAntiWindup.Location = new System.Drawing.Point(12, 237);
            this.tbAntiWindup.Maximum = 9000;
            this.tbAntiWindup.Minimum = -9000;
            this.tbAntiWindup.Name = "tbAntiWindup";
            this.tbAntiWindup.Size = new System.Drawing.Size(648, 45);
            this.tbAntiWindup.TabIndex = 4;
            this.tbAntiWindup.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(296, 315);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(106, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "YawProportionalGain";
            // 
            // tbProportionalGain
            // 
            this.tbProportionalGain.Location = new System.Drawing.Point(12, 344);
            this.tbProportionalGain.Maximum = 9000;
            this.tbProportionalGain.Minimum = -9000;
            this.tbProportionalGain.Name = "tbProportionalGain";
            this.tbProportionalGain.Size = new System.Drawing.Size(648, 45);
            this.tbProportionalGain.TabIndex = 6;
            this.tbProportionalGain.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(241, 72);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(34, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Value";
            // 
            // txtIntegralValue
            // 
            this.txtIntegralValue.Location = new System.Drawing.Point(281, 72);
            this.txtIntegralValue.Name = "txtIntegralValue";
            this.txtIntegralValue.Size = new System.Drawing.Size(100, 20);
            this.txtIntegralValue.TabIndex = 9;
            // 
            // txtDerivativeValue
            // 
            this.txtDerivativeValue.Location = new System.Drawing.Point(281, 172);
            this.txtDerivativeValue.Name = "txtDerivativeValue";
            this.txtDerivativeValue.Size = new System.Drawing.Size(100, 20);
            this.txtDerivativeValue.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(241, 172);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(34, 13);
            this.label6.TabIndex = 10;
            this.label6.Text = "Value";
            // 
            // txtAntiWindupValue
            // 
            this.txtAntiWindupValue.Location = new System.Drawing.Point(281, 275);
            this.txtAntiWindupValue.Name = "txtAntiWindupValue";
            this.txtAntiWindupValue.Size = new System.Drawing.Size(100, 20);
            this.txtAntiWindupValue.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(241, 275);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(34, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Value";
            // 
            // txtProportionalValue
            // 
            this.txtProportionalValue.Location = new System.Drawing.Point(281, 381);
            this.txtProportionalValue.Name = "txtProportionalValue";
            this.txtProportionalValue.Size = new System.Drawing.Size(100, 20);
            this.txtProportionalValue.TabIndex = 15;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(241, 381);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(34, 13);
            this.label8.TabIndex = 14;
            this.label8.Text = "Value";
            // 
            // GainAdjustmentsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(672, 658);
            this.Controls.Add(this.txtProportionalValue);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.txtAntiWindupValue);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.txtDerivativeValue);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.txtIntegralValue);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tbProportionalGain);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbAntiWindup);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbDerivativeGain);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbIntegralGain);
            this.Name = "GainAdjustmentsForm";
            this.Text = "GainAdjustmentsFormcs";
            ((System.ComponentModel.ISupportInitialize)(this.tbIntegralGain)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbDerivativeGain)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbAntiWindup)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbProportionalGain)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar tbIntegralGain;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar tbDerivativeGain;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TrackBar tbAntiWindup;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TrackBar tbProportionalGain;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtIntegralValue;
        private System.Windows.Forms.TextBox txtDerivativeValue;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox txtAntiWindupValue;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtProportionalValue;
        private System.Windows.Forms.Label label8;
    }
}
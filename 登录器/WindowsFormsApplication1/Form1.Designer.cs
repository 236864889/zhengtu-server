namespace WindowsFormsApplication1
{
    partial class title
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(title));
            this.button2 = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.btnOfficial = new System.Windows.Forms.Label();
            this.btnRegister = new System.Windows.Forms.Label();
            this.btnRecharge = new System.Windows.Forms.Label();
            this.btnLottery = new System.Windows.Forms.Label();
            this.btnRecover = new System.Windows.Forms.Label();
            this.grpServers = new System.Windows.Forms.GroupBox();
            this.WebBox1 = new System.Windows.Forms.WebBrowser();
            this.WebBox2 = new System.Windows.Forms.WebBrowser();
            this.tvServers = new System.Windows.Forms.TreeView();
            this.grpServers.SuspendLayout();
            this.SuspendLayout();
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(970, 497);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 5;
            this.button2.Text = "启动游戏";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(531, 604);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "更新完成";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // btnOfficial
            // 
            this.btnOfficial.AutoSize = true;
            this.btnOfficial.Location = new System.Drawing.Point(332, 10);
            this.btnOfficial.Name = "btnOfficial";
            this.btnOfficial.Size = new System.Drawing.Size(53, 12);
            this.btnOfficial.TabIndex = 7;
            this.btnOfficial.Text = "官方网站";
            // 
            // btnRegister
            // 
            this.btnRegister.AutoSize = true;
            this.btnRegister.Location = new System.Drawing.Point(427, 9);
            this.btnRegister.Name = "btnRegister";
            this.btnRegister.Size = new System.Drawing.Size(53, 12);
            this.btnRegister.TabIndex = 8;
            this.btnRegister.Text = "账号注册";
            // 
            // btnRecharge
            // 
            this.btnRecharge.AutoSize = true;
            this.btnRecharge.Location = new System.Drawing.Point(531, 9);
            this.btnRecharge.Name = "btnRecharge";
            this.btnRecharge.Size = new System.Drawing.Size(53, 12);
            this.btnRecharge.TabIndex = 9;
            this.btnRecharge.Text = "在线充值";
            // 
            // btnLottery
            // 
            this.btnLottery.AutoSize = true;
            this.btnLottery.Cursor = System.Windows.Forms.Cursors.Default;
            this.btnLottery.Location = new System.Drawing.Point(634, 10);
            this.btnLottery.Name = "btnLottery";
            this.btnLottery.Size = new System.Drawing.Size(53, 12);
            this.btnLottery.TabIndex = 10;
            this.btnLottery.Text = "在线抽奖";
            // 
            // btnRecover
            // 
            this.btnRecover.AutoSize = true;
            this.btnRecover.Location = new System.Drawing.Point(746, 9);
            this.btnRecover.Name = "btnRecover";
            this.btnRecover.Size = new System.Drawing.Size(53, 12);
            this.btnRecover.TabIndex = 11;
            this.btnRecover.Text = "找回密码";
            // 
            // grpServers
            // 
            this.grpServers.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.grpServers.Controls.Add(this.tvServers);
            this.grpServers.ForeColor = System.Drawing.SystemColors.MenuHighlight;
            this.grpServers.Location = new System.Drawing.Point(15, 85);
            this.grpServers.Name = "grpServers";
            this.grpServers.Size = new System.Drawing.Size(184, 489);
            this.grpServers.TabIndex = 12;
            this.grpServers.TabStop = false;
            this.grpServers.Text = "区服列表";
            this.grpServers.Enter += new System.EventHandler(this.grpServers_Enter);
            // 
            // WebBox1
            // 
            this.WebBox1.IsWebBrowserContextMenuEnabled = false;
            this.WebBox1.Location = new System.Drawing.Point(205, 85);
            this.WebBox1.MinimumSize = new System.Drawing.Size(20, 20);
            this.WebBox1.Name = "WebBox1";
            this.WebBox1.ScriptErrorsSuppressed = true;
            this.WebBox1.Size = new System.Drawing.Size(435, 489);
            this.WebBox1.TabIndex = 13;
            this.WebBox1.WebBrowserShortcutsEnabled = false;
            this.WebBox1.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.WebBox1_DocumentCompleted);
            // 
            // WebBox2
            // 
            this.WebBox2.IsWebBrowserContextMenuEnabled = false;
            this.WebBox2.Location = new System.Drawing.Point(662, 265);
            this.WebBox2.MinimumSize = new System.Drawing.Size(20, 20);
            this.WebBox2.Name = "WebBox2";
            this.WebBox2.ScriptErrorsSuppressed = true;
            this.WebBox2.Size = new System.Drawing.Size(437, 192);
            this.WebBox2.TabIndex = 14;
            this.WebBox2.WebBrowserShortcutsEnabled = false;
            // 
            // tvServers
            // 
            this.tvServers.BackColor = System.Drawing.SystemColors.Info;
            this.tvServers.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvServers.ForeColor = System.Drawing.SystemColors.Highlight;
            this.tvServers.HideSelection = false;
            this.tvServers.Location = new System.Drawing.Point(3, 17);
            this.tvServers.Name = "tvServers";
            this.tvServers.ShowLines = false;
            this.tvServers.ShowPlusMinus = false;
            this.tvServers.Size = new System.Drawing.Size(178, 469);
            this.tvServers.TabIndex = 1;
            this.tvServers.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvServers_AfterSelect_1);
            // 
            // title
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1170, 650);
            this.Controls.Add(this.WebBox2);
            this.Controls.Add(this.WebBox1);
            this.Controls.Add(this.grpServers);
            this.Controls.Add(this.btnRecover);
            this.Controls.Add(this.btnLottery);
            this.Controls.Add(this.btnRecharge);
            this.Controls.Add(this.btnRegister);
            this.Controls.Add(this.btnOfficial);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button2);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "title";
            this.Text = "征途登录器";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.grpServers.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label btnOfficial;
        private System.Windows.Forms.Label btnRegister;
        private System.Windows.Forms.Label btnRecharge;
        private System.Windows.Forms.Label btnLottery;
        private System.Windows.Forms.Label btnRecover;
        private System.Windows.Forms.GroupBox grpServers;
        private System.Windows.Forms.WebBrowser WebBox1;
        private System.Windows.Forms.WebBrowser WebBox2;
        private System.Windows.Forms.TreeView tvServers;
    }
}


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
            this.tvServers = new System.Windows.Forms.TreeView();
            this.WebBox1 = new System.Windows.Forms.WebBrowser();
            this.WebBox2 = new System.Windows.Forms.WebBrowser();
            this.grpServers.SuspendLayout();
            this.SuspendLayout();
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.button2.Font = new System.Drawing.Font("方正姚体", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button2.ForeColor = System.Drawing.Color.Brown;
            this.button2.Location = new System.Drawing.Point(983, 565);
            this.button2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(174, 76);
            this.button2.TabIndex = 5;
            this.button2.Text = "启动游戏";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.label3.Location = new System.Drawing.Point(535, 629);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "更新完成";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // btnOfficial
            // 
            this.btnOfficial.AutoSize = true;
            this.btnOfficial.BackColor = System.Drawing.Color.Transparent;
            this.btnOfficial.Font = new System.Drawing.Font("华文行楷", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnOfficial.ForeColor = System.Drawing.Color.SaddleBrown;
            this.btnOfficial.Location = new System.Drawing.Point(196, 5);
            this.btnOfficial.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.btnOfficial.Name = "btnOfficial";
            this.btnOfficial.Size = new System.Drawing.Size(108, 25);
            this.btnOfficial.TabIndex = 7;
            this.btnOfficial.Text = "官方网站";
            // 
            // btnRegister
            // 
            this.btnRegister.AutoSize = true;
            this.btnRegister.BackColor = System.Drawing.Color.Transparent;
            this.btnRegister.Font = new System.Drawing.Font("华文行楷", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnRegister.ForeColor = System.Drawing.Color.SaddleBrown;
            this.btnRegister.Location = new System.Drawing.Point(366, 4);
            this.btnRegister.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.btnRegister.Name = "btnRegister";
            this.btnRegister.Size = new System.Drawing.Size(108, 25);
            this.btnRegister.TabIndex = 8;
            this.btnRegister.Text = "账号注册";
            // 
            // btnRecharge
            // 
            this.btnRecharge.AutoSize = true;
            this.btnRecharge.BackColor = System.Drawing.Color.Transparent;
            this.btnRecharge.Font = new System.Drawing.Font("华文行楷", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnRecharge.ForeColor = System.Drawing.Color.SaddleBrown;
            this.btnRecharge.Location = new System.Drawing.Point(532, 4);
            this.btnRecharge.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.btnRecharge.Name = "btnRecharge";
            this.btnRecharge.Size = new System.Drawing.Size(108, 25);
            this.btnRecharge.TabIndex = 9;
            this.btnRecharge.Text = "在线充值";
            // 
            // btnLottery
            // 
            this.btnLottery.AutoSize = true;
            this.btnLottery.BackColor = System.Drawing.Color.Transparent;
            this.btnLottery.Cursor = System.Windows.Forms.Cursors.Default;
            this.btnLottery.Font = new System.Drawing.Font("华文行楷", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnLottery.ForeColor = System.Drawing.Color.SaddleBrown;
            this.btnLottery.Location = new System.Drawing.Point(695, 5);
            this.btnLottery.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.btnLottery.Name = "btnLottery";
            this.btnLottery.Size = new System.Drawing.Size(108, 25);
            this.btnLottery.TabIndex = 10;
            this.btnLottery.Text = "在线抽奖";
            // 
            // btnRecover
            // 
            this.btnRecover.AutoSize = true;
            this.btnRecover.BackColor = System.Drawing.Color.Transparent;
            this.btnRecover.Font = new System.Drawing.Font("华文行楷", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.btnRecover.ForeColor = System.Drawing.Color.SaddleBrown;
            this.btnRecover.Location = new System.Drawing.Point(885, 4);
            this.btnRecover.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.btnRecover.Name = "btnRecover";
            this.btnRecover.Size = new System.Drawing.Size(108, 25);
            this.btnRecover.TabIndex = 11;
            this.btnRecover.Text = "找回密码";
            // 
            // grpServers
            // 
            this.grpServers.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.grpServers.Controls.Add(this.tvServers);
            this.grpServers.Font = new System.Drawing.Font("方正姚体", 15F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.grpServers.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.grpServers.Location = new System.Drawing.Point(2, 153);
            this.grpServers.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.grpServers.Name = "grpServers";
            this.grpServers.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.grpServers.Size = new System.Drawing.Size(204, 488);
            this.grpServers.TabIndex = 12;
            this.grpServers.TabStop = false;
            this.grpServers.Text = "区服列表";
            this.grpServers.Enter += new System.EventHandler(this.grpServers_Enter);
            // 
            // tvServers
            // 
            this.tvServers.BackColor = System.Drawing.SystemColors.Info;
            this.tvServers.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvServers.Font = new System.Drawing.Font("方正姚体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tvServers.ForeColor = System.Drawing.Color.White;
            this.tvServers.HideSelection = false;
            this.tvServers.Location = new System.Drawing.Point(4, 27);
            this.tvServers.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tvServers.Name = "tvServers";
            this.tvServers.ShowLines = false;
            this.tvServers.ShowPlusMinus = false;
            this.tvServers.Size = new System.Drawing.Size(196, 457);
            this.tvServers.TabIndex = 1;
            this.tvServers.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.tvServers_AfterSelect_1);
            // 
            // WebBox1
            // 
            this.WebBox1.IsWebBrowserContextMenuEnabled = false;
            this.WebBox1.Location = new System.Drawing.Point(210, 267);
            this.WebBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.WebBox1.MinimumSize = new System.Drawing.Size(20, 20);
            this.WebBox1.Name = "WebBox1";
            this.WebBox1.ScriptErrorsSuppressed = true;
            this.WebBox1.Size = new System.Drawing.Size(470, 358);
            this.WebBox1.TabIndex = 10;
            this.WebBox1.WebBrowserShortcutsEnabled = false;
            this.WebBox1.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.WebBox1_DocumentCompleted);
            // 
            // WebBox2
            // 
            this.WebBox2.IsWebBrowserContextMenuEnabled = false;
            this.WebBox2.Location = new System.Drawing.Point(682, 394);
            this.WebBox2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.WebBox2.MinimumSize = new System.Drawing.Size(20, 20);
            this.WebBox2.Name = "WebBox2";
            this.WebBox2.ScriptErrorsSuppressed = true;
            this.WebBox2.Size = new System.Drawing.Size(475, 170);
            this.WebBox2.TabIndex = 14;
            this.WebBox2.WebBrowserShortcutsEnabled = false;
            // 
            // title
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(1170, 650);
            this.Controls.Add(this.WebBox2);
            this.Controls.Add(this.btnRecover);
            this.Controls.Add(this.btnLottery);
            this.Controls.Add(this.btnRecharge);
            this.Controls.Add(this.btnRegister);
            this.Controls.Add(this.btnOfficial);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.WebBox1);
            this.Controls.Add(this.grpServers);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "title";
            this.Text = "征途登录器";
            this.TransparencyKey = System.Drawing.SystemColors.ControlDarkDark;
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


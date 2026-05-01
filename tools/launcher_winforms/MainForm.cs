using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace LauncherWinForms
{
    public class MainForm : Form
    {
        private TextBox txtClientDir;
        private Button btnBrowse;
        private ListBox lstServers;
        private Button btnCheckResources;
        private Button btnLaunch;
        private Button btnOpenLogs;
        private Label lblStatus;

        private List<ServerEntry> _servers = new List<ServerEntry>();

        public MainForm()
        {
            InitializeComponents();
            LoadServers();
        }

        private void InitializeComponents()
        {
            Text = "征途登录器";
            Width = 800;
            Height = 520;

            txtClientDir = new TextBox();
            txtClientDir.Left = 12;
            txtClientDir.Top = 12;
            txtClientDir.Width = 620;
            txtClientDir.Text = AppDomain.CurrentDomain.BaseDirectory;

            btnBrowse = new Button();
            btnBrowse.Text = "选择客户端目录";
            btnBrowse.Left = 640;
            btnBrowse.Top = 10;
            btnBrowse.Width = 130;
            btnBrowse.Click += BtnBrowse_Click;

            lstServers = new ListBox();
            lstServers.Left = 12;
            lstServers.Top = 48;
            lstServers.Width = 758;
            lstServers.Height = 320;

            btnCheckResources = new Button();
            btnCheckResources.Text = "资源检查";
            btnCheckResources.Left = 12;
            btnCheckResources.Top = 380;
            btnCheckResources.Width = 120;
            btnCheckResources.Click += BtnCheckResources_Click;

            btnLaunch = new Button();
            btnLaunch.Text = "启动游戏";
            btnLaunch.Left = 140;
            btnLaunch.Top = 380;
            btnLaunch.Width = 120;
            btnLaunch.Click += BtnLaunch_Click;

            btnOpenLogs = new Button();
            btnOpenLogs.Text = "打开日志目录";
            btnOpenLogs.Left = 268;
            btnOpenLogs.Top = 380;
            btnOpenLogs.Width = 140;
            btnOpenLogs.Click += BtnOpenLogs_Click;

            lblStatus = new Label();
            lblStatus.Left = 12;
            lblStatus.Top = 420;
            lblStatus.Width = 758;
            lblStatus.Height = 50;
            lblStatus.Text = "就绪";

            Controls.Add(txtClientDir);
            Controls.Add(btnBrowse);
            Controls.Add(lstServers);
            Controls.Add(btnCheckResources);
            Controls.Add(btnLaunch);
            Controls.Add(btnOpenLogs);
            Controls.Add(lblStatus);
        }

        private void BtnBrowse_Click(object sender, EventArgs e)
        {
            using (FolderBrowserDialog dlg = new FolderBrowserDialog())
            {
                dlg.SelectedPath = txtClientDir.Text;
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    txtClientDir.Text = dlg.SelectedPath;
                }
            }
        }

        private void LoadServers()
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "servers.json");
            _servers = ServerListProvider.Load(path);
            lstServers.Items.Clear();
            foreach (ServerEntry s in _servers) lstServers.Items.Add(s);
            if (lstServers.Items.Count > 0) lstServers.SelectedIndex = 0;
        }

        private void BtnCheckResources_Click(object sender, EventArgs e)
        {
            string clientDir = txtClientDir.Text;
            ResourceCheckResult check = ResourceIntegrityChecker.Check(clientDir);
            lblStatus.Text = check.ToUiSummary();
            MessageBox.Show(check.ToUiSummary(), "资源检查", MessageBoxButtons.OK,
                check.CanLaunch ? MessageBoxIcon.Information : MessageBoxIcon.Warning);
        }

        private void BtnLaunch_Click(object sender, EventArgs e)
        {
            try
            {
                string clientDir = txtClientDir.Text;
                if (!Directory.Exists(clientDir)) throw new LauncherException(LauncherErrorCode.E006, "客户端目录不存在");

                ServerEntry server = lstServers.SelectedItem as ServerEntry;
                if (server == null) throw new LauncherException(LauncherErrorCode.E007, "请先选择区服");

                string clientExe = ClientExecutableDetector.PickClientExecutable(clientDir);

                ResourceCheckResult check = ResourceIntegrityChecker.Check(clientDir);
                if (!check.CanLaunch)
                    throw new LauncherException(LauncherErrorCode.E004, check.ToUiSummary());

                string iniPath = Path.Combine(clientDir, "config.ini");
                IniWriteResult iniResult = IniConfigWriter.WriteServerConfig(iniPath, server);
                if (!iniResult.Success)
                    throw new LauncherException(LauncherErrorCode.E003, iniResult.Message);

                int pid = ClientProcessLauncher.StartClient(clientExe, clientDir, null);
                LauncherLogger.LogLaunch(clientDir, clientExe, server, check, iniResult, pid, null);
                lblStatus.Text = "启动成功，PID=" + pid;
            }
            catch (LauncherException le)
            {
                LauncherLogger.LogException(le.Code.ToString(), le.ToString());
                LauncherErrorPresenter.Show(le.Code, le.Message);
                lblStatus.Text = "失败：" + le.Code;
            }
            catch (UnauthorizedAccessException ex)
            {
                LauncherLogger.LogException(LauncherErrorCode.E008.ToString(), ex.ToString());
                LauncherErrorPresenter.Show(LauncherErrorCode.E008, ex.Message);
                lblStatus.Text = "失败：E008";
            }
            catch (Exception ex)
            {
                LauncherLogger.LogException(LauncherErrorCode.E005.ToString(), ex.ToString());
                LauncherErrorPresenter.Show(LauncherErrorCode.E005, ex.Message);
                lblStatus.Text = "失败：E005";
            }
        }

        private void BtnOpenLogs_Click(object sender, EventArgs e)
        {
            string path = LauncherLogger.EnsureLogDir();
            Process.Start(path);
        }
    }
}

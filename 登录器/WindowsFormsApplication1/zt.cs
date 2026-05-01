using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net.Http;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Web.Script.Serialization;
using System.Windows.Forms;
using System.IO.Compression;
using System.Reflection; // 反射创建快捷方式
using System.Drawing;

namespace WindowsFormsApplication1
{
    public partial class title : Form
    {
        // JSON 地址
        private const string JSON_URL = "http://e.haory.cn/e/e7/mohe/list.json";

        // config.ini 在游戏根目录
        private const string CONFIG_INI_NAME = "config.ini";

        // JSON 根字典
        private Dictionary<string, object> _configRoot;

        // 区服列表
        private class ServerItem
        {
            public string GroupName;   // 分组名：乱世驿站 / 岁月征途
            public string Name;        // 区服名
            public string Ip;
            public int Port;
            public int Zone;

            public override string ToString()
            {
                return "[" + GroupName + "] " + Name;
            }
        }

        private List<ServerItem> _servers = new List<ServerItem>();
        private ServerItem _selectedServer = null;
        // 每个分组名 -> TreeNode
        private Dictionary<string, TreeNode> _groupNodes = new Dictionary<string, TreeNode>();
        // 页面地址
        private string _urlOfficial;   // 官方首页
        private string _urlRegister;   // 注册地址
        private string _urlRecharge;   // 充值地址
        private string _urlLottery;    // 抽奖地址
        private string _urlPassword;   // 密码找回地址
        private string _urlNotice1;    // 公告1
        private string _urlNotice2;    // 公告2
        private string _shortcutName;  // 桌面快捷方式名称（来自 JSON）

        // 更新配置
        private class UpdateItem
        {
            public string RelPath;   // 相对路径，如 /ztgame.dat
            public string Md5;       // 目标 MD5
            public string Url;       // ZIP 下载地址
        }

        private List<UpdateItem> _updates = new List<UpdateItem>();

        // 自更新（登录器自身）
        private string _selfExeRelPath;
        private string _selfExeMd5;
        private string _selfUpdateZipUrl;
        private string _selfUpdateExeName;

        // 游戏主程序（优先 ztgame.dat，其次 ztgame.exe）
        private string _gameFilePath;

        // 防止重复启动游戏
        private bool _gameStarted = false;
        // 启动前的 config.ini 备份内容（如果原来就存在的话）
        private string _configIniBackupContent = null;

        public title()
        {
            InitializeComponent();

            // 窗体加载事件
            this.Load += Form1_Load;

            // 服务器树事件
            tvServers.AfterSelect += tvServers_AfterSelect;
            tvServers.NodeMouseDoubleClick += tvServers_NodeMouseDoubleClick;

            button2.Click += button2_Click;
            btnOfficial.Click += btnOfficial_Click;
            btnRegister.Click += btnRegister_Click;
            btnRecharge.Click += btnRecharge_Click;
            btnLottery.Click += btnLottery_Click;
            btnRecover.Click += btnRecover_Click;
        }
        // 选中节点时，更新当前区服
        private void tvServers_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var s = e.Node.Tag as ServerItem;
            if (s != null)
            {
                _selectedServer = s;
                label3.Text = "已选择区服：" + s.Name;
            }
        }

        // 双击节点 = 直接启动游戏
        private void tvServers_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            var s = e.Node.Tag as ServerItem;
            if (s != null)
            {
                _selectedServer = s;
                button2_Click(null, EventArgs.Empty);
            }
        }

        private void EnsureDesktopShortcut()
        {
            try
            {
                string desktop = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);

                // 默认名字
                string defaultName = "征途登录器";
                // JSON 里的名字（如果为空就用默认）
                string name = string.IsNullOrWhiteSpace(_shortcutName) ? defaultName : _shortcutName;

                string finalShortcut = name + ".lnk";
                string finalPath = Path.Combine(desktop, finalShortcut);

                // 如果现在使用的是 JSON 名称，就顺手清理掉旧的默认快捷方式
                if (!string.IsNullOrWhiteSpace(_shortcutName))
                {
                    string oldPath = Path.Combine(desktop, defaultName + ".lnk");
                    if (File.Exists(oldPath) && !oldPath.Equals(finalPath, StringComparison.OrdinalIgnoreCase))
                    {
                        try { File.Delete(oldPath); } catch { }
                    }
                }

                // 目标快捷方式已经存在就不再创建
                if (File.Exists(finalPath))
                    return;

                // 通过 ProgID 拿到 WScript.Shell 类型
                Type shellType = Type.GetTypeFromProgID("WScript.Shell");
                if (shellType == null) return;

                // var shell = new WshShell();
                object shell = Activator.CreateInstance(shellType);

                // shell.CreateShortcut(finalPath)
                object shortcut = shellType.InvokeMember(
                    "CreateShortcut",
                    BindingFlags.InvokeMethod,
                    null,
                    shell,
                    new object[] { finalPath }
                );

                Type scType = shortcut.GetType();
                string exePath = Application.ExecutablePath;
                string workDir = AppDomain.CurrentDomain.BaseDirectory;

                scType.InvokeMember(
                    "TargetPath",
                    BindingFlags.SetProperty,
                    null,
                    shortcut,
                    new object[] { exePath });

                scType.InvokeMember(
                    "WorkingDirectory",
                    BindingFlags.SetProperty,
                    null,
                    shortcut,
                    new object[] { workDir });

                scType.InvokeMember(
                    "WindowStyle",
                    BindingFlags.SetProperty,
                    null,
                    shortcut,
                    new object[] { 1 });

                scType.InvokeMember(
                    "Description",
                    BindingFlags.SetProperty,
                    null,
                    shortcut,
                    new object[] { "征途登录器（JSON 配置版）" });

                // 如需图标，可用 launcher.ico
                //string icoPath = Path.Combine(workDir, "launcher.ico");
                //scType.InvokeMember(
                //    "IconLocation",
                //    BindingFlags.SetProperty,
                //    null,
                //    shortcut,
                //    new object[] { icoPath });

                scType.InvokeMember(
                    "Save",
                    BindingFlags.InvokeMethod,
                    null,
                    shortcut,
                    null
                );
            }
            catch
            {
                // 创建失败直接忽略
            }
        }

        // =================== 窗体加载 ===================

        private async void Form1_Load(object sender, EventArgs e)
        {
            button2.Enabled = false;
            label3.Text = "正在加载配置：" + JSON_URL;

            // WebBrowser 基本设置
            if (WebBox1 != null)
            {
                WebBox1.IsWebBrowserContextMenuEnabled = false;
                WebBox1.WebBrowserShortcutsEnabled = false;
                WebBox1.ScriptErrorsSuppressed = true;
            }
            if (WebBox2 != null)
            {
                WebBox2.IsWebBrowserContextMenuEnabled = false;
                WebBox2.WebBrowserShortcutsEnabled = false;
                WebBox2.ScriptErrorsSuppressed = true;
            }

            try
            {
                await LoadConfigAsync();          // 拉 JSON（里面会 ParsePages，把 _shortcutName 设置好）
                await CheckSelfUpdateAsync();     // 登录器自更新（失败不终止）
                await CheckGameUpdateAsync();     // 客户端文件更新（失败不终止）

                // ★ JSON 全部解析完，再按“桌面快捷方式”创建图标
                EnsureDesktopShortcut();

                label3.Text = "配置加载完成，共 " + _servers.Count + " 个区服。请选择区服并启动游戏。";
                button2.Enabled = true;
            }
            catch (Exception ex)
            {
                label3.Text = "初始化失败：" + ex.Message;
                MessageBox.Show("初始化失败：\n" + ex.ToString(), "错误",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // =================== 读取 JSON ===================
private async Task LoadConfigAsync()
{
    string jsonText;
    using (var client = new HttpClient())
    {
        client.Timeout = TimeSpan.FromSeconds(10);
        jsonText = await client.GetStringAsync(JSON_URL);
    }

    var serializer = new JavaScriptSerializer();
    _configRoot = serializer.Deserialize<Dictionary<string, object>>(jsonText);

    ParsePages();
    ParseServers();
    ParseUpdates();      // ★ 补上这行，才能填充 _updates 列表
    ParseSelfUpdate();
    LoadHtmlBlocks();

    _gameFilePath = GuessGameExe();
}
        // 解析页面链接
        private void ParsePages()
        {
            _urlOfficial = GetString("官方首页");
            _urlRegister = GetString("注册地址");
            _urlRecharge = GetString("充值地址");
            _urlLottery = GetString("抽奖地址");
            _urlPassword = GetString("密码地址");
            _urlNotice1 = GetString("公告1");
            _urlNotice2 = GetString("公告2");
            _shortcutName = GetString("桌面快捷方式");
        }

        private IList ToIList(object obj)
        {
            if (obj == null) return null;
            return obj as IList;
        }
        // 解析 "服务器" 节点（兼容有分组 / 无分组，多种写法）
        private void ParseServers()
        {
            _servers.Clear();
            _groupNodes.Clear();

            if (tvServers != null)
            {
                tvServers.BeginUpdate();
                tvServers.Nodes.Clear();
            }

            if (_configRoot == null) return;

            object serversObjRaw;
            if (!_configRoot.TryGetValue("服务器", out serversObjRaw) || serversObjRaw == null)
                return;

            bool anyAdded = false;

            // 情况 A：{"组名": [...], "组名2": [...]}
            var dict = serversObjRaw as Dictionary<string, object>;
            if (dict != null)
            {
                foreach (KeyValuePair<string, object> kv in dict)
                {
                    string groupName = kv.Key;
                    object groupValue = kv.Value;

                    // A1: 组直接是数组： [ [name,ip,port,zone], ... ]
                    IList arr = ToIList(groupValue);
                    if (arr != null)
                    {
                        for (int i = 0; i < arr.Count; i++)
                        {
                            IList sa = ToIList(arr[i]);
                            if (sa == null || sa.Count < 3) continue;

                            AddServerFromList(groupName, sa, null);
                            anyAdded = true;
                        }
                        continue;
                    }

                    // A2: 组是一个字典： { "区名1": [ip,port,zone], "区名2": [...] }
                    var innerDict = groupValue as Dictionary<string, object>;
                    if (innerDict != null)
                    {
                        foreach (var kv2 in innerDict)
                        {
                            string srvName = kv2.Key;        // 用 key 当区服名
                            IList sa = ToIList(kv2.Value);   // value 是 [ip,port,zone] 或 [name,ip,port,zone]
                            if (sa == null || sa.Count < 3) continue;

                            AddServerFromList(groupName, sa, srvName);
                            anyAdded = true;
                        }
                    }
                }
            }
            else
            {
                // 情况 B：服务器直接是一个数组： [ [name,ip,port,zone], ... ]
                IList arr = ToIList(serversObjRaw);
                if (arr != null)
                {
                    for (int i = 0; i < arr.Count; i++)
                    {
                        IList sa = ToIList(arr[i]);
                        if (sa == null || sa.Count < 3) continue;

                        AddServerFromList("默认分组", sa, null);
                        anyAdded = true;
                    }
                }
            }

            if (tvServers != null)
            {
                // ★ 关键：展开所有分组，这样“岁月征途”等分组的子区服一打开就能看到
                tvServers.ExpandAll();
                tvServers.EndUpdate();
            }

            // 默认选中第一个区服
            if (anyAdded && tvServers != null && tvServers.Nodes.Count > 0)
            {
                TreeNode firstGroup = tvServers.Nodes[0];
                TreeNode firstServer = firstGroup;

                if (firstGroup.Nodes.Count > 0)
                    firstServer = firstGroup.Nodes[0];

                var s = firstServer.Tag as ServerItem;
                if (s != null)
                {
                    tvServers.SelectedNode = firstServer;
                    _selectedServer = s;
                    label3.Text = "已选择区服：" + s.Name;
                }
            }
        }

        // =================== TreeView 添加节点 ===================

        // 默认：区名用列表里的第 0 个元素
        private void AddServerFromList(string groupName, IList sa)
        {
            AddServerFromList(groupName, sa, null);
        }

        // 可指定区名（用于“组是字典：键就是区名”的情况）
        private void AddServerFromList(string groupName, IList sa, string srvNameOverride)
        {
            // sa 至少 3 项：[ip,port,zone] 或 [name,ip,port,zone]
            string name;
            string ip;
            int port;
            int zone;

            if (sa.Count >= 4 && srvNameOverride == null)
            {
                // [name, ip, port, zone]
                name = sa[0] as string;
                ip = sa[1] as string;
                port = ToInt(sa[2]);
                zone = ToInt(sa[3]);
            }
            else
            {
                // [ip, port, zone]，区名用 override 或第 0 项
                name = srvNameOverride ?? (sa[0] as string) ?? "大象征途";
                ip = (sa.Count > 1 ? sa[1] as string : null) ?? "127.0.0.1";
                port = (sa.Count > 2 ? ToInt(sa[2]) : 9777);
                zone = (sa.Count > 3 ? ToInt(sa[3]) : 1111);
            }

            var item = new ServerItem
            {
                GroupName = groupName,
                Name = name,
                Ip = ip,
                Port = port,
                Zone = zone
            };

            _servers.Add(item);
            if (tvServers == null) return;

            // 分组节点（蓝色）
            TreeNode groupNode;
            if (!_groupNodes.TryGetValue(groupName, out groupNode))
            {
                groupNode = new TreeNode(groupName);
                groupNode.NodeFont = new Font(tvServers.Font, FontStyle.Bold);
                groupNode.ForeColor = Color.Blue;        // ★ 组：蓝色字
                tvServers.Nodes.Add(groupNode);
                _groupNodes[groupName] = groupNode;
            }

            // 区服节点（绿色）
            TreeNode serverNode = new TreeNode(item.Name);
            serverNode.Tag = item;
            serverNode.ForeColor = Color.Lime;           // ★ 区服：绿色字

            groupNode.Nodes.Add(serverNode);
        }

        // 解析 "更新" 列表
        private void ParseUpdates()
        {
            _updates.Clear();

            if (_configRoot == null)
                return;

            object updRaw;
            if (!_configRoot.TryGetValue("更新", out updRaw) || updRaw == null)
                return;

            IList arr = ToIList(updRaw);
            if (arr == null)
                return;

            for (int i = 0; i < arr.Count; i++)
            {
                IList ua = ToIList(arr[i]);
                if (ua == null || ua.Count < 3) continue;

                UpdateItem item = new UpdateItem();
                item.RelPath = ua[0] as string;                    // 相对路径，如 /Data/datas.pak
                item.Md5 = (ua[1] as string ?? "").ToLowerInvariant(); // 目标 MD5
                item.Url = ua[2] as string;                    // ZIP 下载地址

                _updates.Add(item);
            }
        }

        // 解析 自更新[删除文字生效]
        private void ParseSelfUpdate()
        {
            _selfExeRelPath = null;
            _selfExeMd5 = null;
            _selfUpdateZipUrl = null;
            _selfUpdateExeName = null;

            object selfRaw;
            if (!_configRoot.TryGetValue("自更新[删除文字生效]", out selfRaw) || selfRaw == null)
                return;

            IList arr = ToIList(selfRaw);
            if (arr == null || arr.Count < 4) return;

            _selfExeRelPath = arr[0] as string;
            _selfExeMd5 = (arr[1] as string ?? "").ToLowerInvariant();
            _selfUpdateZipUrl = arr[2] as string;
            _selfUpdateExeName = arr[3] as string;
        }

        // 公告 HTML / 页面加载
        private void LoadHtmlBlocks()
        {
            // 公告1 → WebBox1
            if (WebBox1 != null)
            {
                if (!string.IsNullOrEmpty(_urlNotice1))
                {
                    if (_urlNotice1.TrimStart().StartsWith("<"))
                        WebBox1.DocumentText = _urlNotice1;
                    else
                    {
                        try { WebBox1.Navigate(_urlNotice1); }
                        catch { WebBox1.DocumentText = "<h3>公告1加载失败</h3>"; }
                    }
                }
                else
                {
                    WebBox1.DocumentText = "<h3>公告区</h3><p>暂无公告</p>";
                }
            }

            // 公告2 → WebBox2
            if (WebBox2 != null)
            {
                if (!string.IsNullOrEmpty(_urlNotice2))
                {
                    if (_urlNotice2.TrimStart().StartsWith("<"))
                        WebBox2.DocumentText = _urlNotice2;
                    else
                    {
                        try { WebBox2.Navigate(_urlNotice2); }
                        catch { WebBox2.DocumentText = "<h3>公告2加载失败</h3>"; }
                    }
                }
                else
                {
                    WebBox2.DocumentText = "<h3>公告2</h3><p>暂无公告</p>";
                }
            }
        }

        // 猜测游戏 EXE 路径：优先 ztgame.dat，然后 ztgame.exe
        private string GuessGameExe()
        {
            string baseDir = AppDomain.CurrentDomain.BaseDirectory;

            string datPath = Path.Combine(baseDir, "ztgame.dat");
            if (File.Exists(datPath))
                return datPath;

            string exePath = Path.Combine(baseDir, "ztgame.exe");
            if (File.Exists(exePath))
                return exePath;

            // 再从更新列表里猜
            foreach (UpdateItem u in _updates)
            {
                if (string.IsNullOrEmpty(u.RelPath)) continue;
                string rel = u.RelPath.Replace('/', '\\').TrimStart('\\');
                string full = Path.Combine(baseDir, rel);
                if (File.Exists(full))
                    return full;
            }

            return datPath; // 默认返回 datPath，找不到时后面会提示
        }
        private async void button2_Click(object sender, EventArgs e)
        {
            if (_gameStarted)
                return;

            // 优先以当前 TreeView 选中项为准
            if (tvServers != null && tvServers.SelectedNode != null)
            {
                var s = tvServers.SelectedNode.Tag as ServerItem;
                if (s != null)
                {
                    _selectedServer = s;
                }
            }

            // 没选就默认第一个
            if (_selectedServer == null && tvServers != null && tvServers.Nodes.Count > 0)
            {
                TreeNode firstGroup = tvServers.Nodes[0];
                TreeNode firstServer = firstGroup;
                if (firstGroup.Nodes.Count > 0)
                    firstServer = firstGroup.Nodes[0];

                var s = firstServer.Tag as ServerItem;
                if (s != null)
                {
                    _selectedServer = s;
                    tvServers.SelectedNode = firstServer;
                }
            }

            if (_selectedServer == null)
            {
                label3.Text = "没有可用区服配置，请检查 list.json。";
                MessageBox.Show("没有可用区服配置，请检查 list.json 中的“服务器”节点。", "提示");
                return;
            }

            // 确保游戏主程序存在
            if (string.IsNullOrEmpty(_gameFilePath) || !File.Exists(_gameFilePath))
            {
                _gameFilePath = GuessGameExe();
            }
            if (string.IsNullOrEmpty(_gameFilePath) || !File.Exists(_gameFilePath))
            {
                label3.Text = "找不到游戏主程序（ztgame.dat / ztgame.exe）。";
                MessageBox.Show("找不到游戏主程序（ztgame.dat / ztgame.exe）。", "错误");
                return;
            }

            try
            {
                // 1）先根据所选区服更新 config.ini 的 [server] 段（全部来自 JSON）
                UpdateConfigIni(_selectedServer);

                // 2）然后启动游戏（不带参数，完全等价于手动双击 ztgame.dat）
                string exePath = _gameFilePath;
                string workDir = Path.GetDirectoryName(exePath);

                ProcessStartInfo psi = new ProcessStartInfo();
                psi.FileName = exePath;
                psi.Arguments = string.Empty;
                psi.WorkingDirectory = workDir;
                psi.UseShellExecute = false;

                Process.Start(psi);
                _gameStarted = true;

                label3.Text = "已启动游戏，请在登录界面输入账号密码。";

                // 先把登录器界面隐藏掉
                this.Hide();

                // ★ 等待 2~3 秒，让游戏读取完 config.ini
                await Task.Delay(2000);

                // 3）抹除 / 还原 config.ini 里的关键信息
                WipeConfigIniAfterLaunch();

                // 4）再退出登录器
                Application.Exit();
            }
            catch (Exception ex)
            {
                label3.Text = "启动游戏失败：" + ex.Message;
                MessageBox.Show("启动游戏失败：\n" + ex.ToString(), "错误");
            }
        }
        private void UpdateConfigIni(ServerItem s)
        {
            string baseDir = AppDomain.CurrentDomain.BaseDirectory;
            string iniPath = Path.Combine(baseDir, CONFIG_INI_NAME);

            // 1）如果不存在 config.ini，就按当前区服新建一份
            if (!File.Exists(iniPath))
            {
                StringBuilder sb = new StringBuilder();
                sb.AppendLine("[server]");
                sb.AppendLine("loginaddress=" + (s.Ip ?? "127.0.0.1"));
                sb.AppendLine("servername=" + (s.Name ?? "默认区服"));
                sb.AppendLine("loginport=" + (s.Port > 0 ? s.Port.ToString() : "7000"));
                sb.AppendLine("zone=" + (s.Zone > 0 ? s.Zone.ToString() : "1"));
                sb.AppendLine("updatetype=703");
                sb.AppendLine("gametype=1");
                sb.AppendLine("gametypeex=1003");
                sb.AppendLine("checkednettype=1");
                sb.AppendLine("showexp=1");
                sb.AppendLine("showrightexp=1");
                sb.AppendLine("optimizechatdialog=0");
                sb.AppendLine("grouptipopen=1");
                sb.AppendLine("otheraccess=1");
                sb.AppendLine("minitasktrack=1");
                sb.AppendLine("tasktracktemp=0");
                sb.AppendLine("[video]");
                sb.AppendLine("colorbits=32");
                sb.AppendLine("screenwidth=1024");
                sb.AppendLine("screenheight=768");
                sb.AppendLine("window=1");
                sb.AppendLine("[audio]");
                sb.AppendLine("sound=1");
                sb.AppendLine("scenesound=1");
                sb.AppendLine("circumstancesound=1");
                sb.AppendLine("backgroundsound=1");
                sb.AppendLine("soundvolume=50");
                sb.AppendLine("scenevolume=50");
                sb.AppendLine("circumstancevolume=50");
                sb.AppendLine("backvolume=50");
                sb.AppendLine("[system]");
                sb.AppendLine("autoclose=0");

                File.WriteAllText(iniPath, sb.ToString(), Encoding.Default);
                return;
            }

            // 2）如果已有 config.ini，先备份完整内容（只备一次）
            if (_configIniBackupContent == null)
            {
                try
                {
                    _configIniBackupContent = File.ReadAllText(iniPath, Encoding.Default);
                }
                catch
                {
                    _configIniBackupContent = null;
                }
            }

            // 后面这段你原来的代码保持不变 ↓↓↓
            string[] lines = File.ReadAllLines(iniPath, Encoding.Default);
            List<string> newLines = new List<string>(lines.Length);
            bool inServer = false;
            int serverHeaderIndex = -1;
            bool setAddr = false;
            bool setPort = false;
            bool setName = false;
            bool setZone = false;

            for (int i = 0; i < lines.Length; i++)
            {
                string line = lines[i];
                string trim = line.Trim();

                // 段头 [xxx]
                if (trim.StartsWith("[", StringComparison.OrdinalIgnoreCase) &&
                    trim.EndsWith("]", StringComparison.OrdinalIgnoreCase))
                {
                    if (trim.Equals("[server]", StringComparison.OrdinalIgnoreCase))
                    {
                        inServer = true;
                        serverHeaderIndex = newLines.Count;
                    }
                    else
                    {
                        inServer = false;
                    }

                    newLines.Add(line);
                    continue;
                }

                if (inServer)
                {
                    string lower = trim.ToLowerInvariant();

                    if (lower.StartsWith("loginaddress="))
                    {
                        newLines.Add("loginaddress=" + (s.Ip ?? "127.0.0.1"));
                        setAddr = true;
                        continue;
                    }
                    if (lower.StartsWith("loginport="))
                    {
                        newLines.Add("loginport=" + (s.Port > 0 ? s.Port.ToString() : "7000"));
                        setPort = true;
                        continue;
                    }
                    if (lower.StartsWith("servername="))
                    {
                        newLines.Add("servername=" + (s.Name ?? ""));
                        setName = true;
                        continue;
                    }
                    if (lower.StartsWith("zone="))
                    {
                        newLines.Add("zone=" + (s.Zone > 0 ? s.Zone.ToString() : "1"));
                        setZone = true;
                        continue;
                    }
                }

                newLines.Add(line);
            }

            // 3）如果 [server] 段里没找到某些字段，就自动插入
            if (serverHeaderIndex >= 0)
            {
                int insertIndex = serverHeaderIndex + 1;

                if (!setAddr)
                {
                    newLines.Insert(insertIndex, "loginaddress=" + (s.Ip ?? "127.0.0.1"));
                    insertIndex++;
                }
                if (!setPort)
                {
                    newLines.Insert(insertIndex, "loginport=" + (s.Port > 0 ? s.Port.ToString() : "7000"));
                    insertIndex++;
                }
                if (!setName)
                {
                    newLines.Insert(insertIndex, "servername=" + (s.Name ?? ""));
                    insertIndex++;
                }
                if (!setZone)
                {
                    newLines.Insert(insertIndex, "zone=" + (s.Zone > 0 ? s.Zone.ToString() : "1"));
                    insertIndex++;
                }
            }

            try
            {
                File.Copy(iniPath, iniPath + ".bak", true);
            }
            catch { }

            File.WriteAllLines(iniPath, newLines.ToArray(), Encoding.Default);
        }
        /// <summary>
        /// 游戏启动后，迅速抹除 / 还原 config.ini 里的关键信息
        /// </summary>
        private void WipeConfigIniAfterLaunch()
        {
            try
            {
                string baseDir = AppDomain.CurrentDomain.BaseDirectory;
                string iniPath = Path.Combine(baseDir, CONFIG_INI_NAME);
                if (!File.Exists(iniPath)) return;

                // 如果有启动前的备份，就直接恢复备份
                if (!string.IsNullOrEmpty(_configIniBackupContent))
                {
                    File.WriteAllText(iniPath, _configIniBackupContent, Encoding.Default);
                    return;
                }

                // 没有备份的情况：只把 [server] 段里的 4 个关键字段改成安全值
                string[] lines = File.ReadAllLines(iniPath, Encoding.Default);
                List<string> newLines = new List<string>(lines.Length);
                bool inServer = false;

                for (int i = 0; i < lines.Length; i++)
                {
                    string line = lines[i];
                    string trim = line.Trim();

                    if (trim.StartsWith("[", StringComparison.OrdinalIgnoreCase) &&
                        trim.EndsWith("]", StringComparison.OrdinalIgnoreCase))
                    {
                        inServer = trim.Equals("[server]", StringComparison.OrdinalIgnoreCase);
                        newLines.Add(line);
                        continue;
                    }

                    if (inServer)
                    {
                        string lower = trim.ToLowerInvariant();

                        if (lower.StartsWith("loginaddress="))
                        {
                            // 抹除真实 IP
                            newLines.Add("loginaddress=127.0.0.1");
                            continue;
                        }
                        if (lower.StartsWith("loginport="))
                        {
                            newLines.Add("loginport=9777");
                            continue;
                        }
                        if (lower.StartsWith("servername="))
                        {
                            newLines.Add("servername=大象征途");
                            continue;
                        }
                        if (lower.StartsWith("zone="))
                        {
                            newLines.Add("zone=1");
                            continue;
                        }
                    }

                    newLines.Add(line);
                }

                File.WriteAllLines(iniPath, newLines.ToArray(), Encoding.Default);
            }
            catch
            {
                // 抹除失败就算了，不影响玩家游戏
            }
        }

        // =================== 更新 / 自更新 ===================

        private string ComputeFileMd5(string path)
        {
            if (!File.Exists(path)) return null;

            using (FileStream stream = File.OpenRead(path))
            using (MD5 md5 = MD5.Create())
            {
                byte[] hash = md5.ComputeHash(stream);
                StringBuilder sb = new StringBuilder();
                foreach (byte b in hash)
                    sb.Append(b.ToString("x2"));
                return sb.ToString();
            }
        }

        // 登录器自更新（失败只提示，不中断）
        private async Task CheckSelfUpdateAsync()
        {
            try
            {
                // 没配 URL 或 MD5 就直接跳过自更新
                if (string.IsNullOrWhiteSpace(_selfUpdateZipUrl) ||
                    string.IsNullOrWhiteSpace(_selfExeMd5))
                {
                    return;
                }

                string baseDir = AppDomain.CurrentDomain.BaseDirectory;

                // 以 JSON 里的相对路径为准，找需要更新的 EXE
                string rel = string.IsNullOrWhiteSpace(_selfExeRelPath)
                    ? Path.GetFileName(Application.ExecutablePath)  // 如果没填路径，就更新当前 exe
                    : _selfExeRelPath.Replace('/', '\\').TrimStart('\\');

                string localExe = Path.Combine(baseDir, rel);

                // exe 不存在：先提示一嘴，但仍然尝试下载一次
                if (!File.Exists(localExe))
                {
                    label3.Text = "登录器主程序不存在，准备下载新版本：" + rel;
                }

                string currentMd5 = (ComputeFileMd5(localExe) ?? "").Trim().ToLowerInvariant();
                string configMd5 = _selfExeMd5.Trim().ToLowerInvariant();

                // ★ 调试信息：你可以用这个看两边 MD5 到底是不是一样
                if (!string.IsNullOrEmpty(currentMd5))
                {
                    label3.Text = $"当前登录器MD5={currentMd5}，配置MD5={configMd5}";
                }

                // 已经是最新版本就直接跳过
                if (!string.IsNullOrEmpty(currentMd5) && currentMd5 == configMd5)
                {
                    // 可选：label3.Text = "登录器已是最新版本。";
                    return;
                }

                // 走到这里说明需要更新
                label3.Text = "发现登录器新版本，正在下载…";

                string patchDir = Path.Combine(baseDir, "patch");
                if (!Directory.Exists(patchDir))
                {
                    Directory.CreateDirectory(patchDir);
                }

                string tmpZip = Path.Combine(patchDir, "launcher_update.zip");

                using (var client = new HttpClient())
                {
                    client.Timeout = TimeSpan.FromSeconds(15);
                    var resp = await client.GetAsync(_selfUpdateZipUrl);
                    if (!resp.IsSuccessStatusCode)
                    {
                        label3.Text = "登录器更新下载失败（HTTP " + (int)resp.StatusCode + "），已跳过。";
                        return;
                    }

                    byte[] data = await resp.Content.ReadAsByteArrayAsync();
                    File.WriteAllBytes(tmpZip, data);
                }

                // 解压：这里仍然解到游戏根目录，你的 zip 里应该只带一个新的 exe
                ExtractZipToDirectoryOverwrite(tmpZip, baseDir);

                // 再算一次 MD5
                string newMd5 = (ComputeFileMd5(localExe) ?? "").Trim().ToLowerInvariant();
                label3.Text = "登录器已更新，新MD5=" + newMd5 + "，请重新运行登录器。";
            }
            catch (Exception ex)
            {
                label3.Text = "登录器更新检查失败，已跳过：" + ex.Message;
            }
        }

        // 客户端文件更新（失败只跳过，不影响整体）
        private async Task CheckGameUpdateAsync()
        {
            // 原列表为空就直接返回
            if (_updates == null || _updates.Count == 0) return;

            string baseDir = AppDomain.CurrentDomain.BaseDirectory;

            // ★★★ 关键：先拷贝一份快照，后面只遍历快照，不再直接遍历 _updates
            List<UpdateItem> updatesSnapshot = new List<UpdateItem>(_updates);

            foreach (UpdateItem u in updatesSnapshot)
            {
                if (u == null ||
                    string.IsNullOrEmpty(u.RelPath) ||
                    string.IsNullOrEmpty(u.Md5) ||
                    string.IsNullOrEmpty(u.Url))
                    continue;

                try
                {
                    // JSON 里的目标路径，如 /Data/ztgame.dat
                    string rel = u.RelPath.Replace('/', '\\').TrimStart('\\');
                    string localPath = Path.Combine(baseDir, rel);

                    // 1）先算当前本地文件的 MD5
                    string curMd5 = ComputeFileMd5(localPath);

                    if (!string.IsNullOrEmpty(curMd5))
                    {
                        string cfgMd5 = u.Md5.Trim().ToLowerInvariant();
                        string nowMd5 = curMd5.Trim().ToLowerInvariant();

                        if (nowMd5 == cfgMd5)
                        {
                            // 已是最新，直接跳过
                            label3.Text = "文件已是最新(跳过下载)：" + u.RelPath;
                            continue;
                        }
                        else
                        {
                            label3.Text = "检测到文件需更新：" + u.RelPath +
                                          " 本地MD5=" + nowMd5 +
                                          " 配置MD5=" + cfgMd5;
                        }
                    }
                    else
                    {
                        // 文件不存在，必然要下载
                        label3.Text = "文件不存在，准备下载：" + u.RelPath;
                    }

                    // 2）下载补丁 ZIP（放到根目录的 patch 目录）
                    string patchDir = Path.Combine(baseDir, "patch");
                    if (!Directory.Exists(patchDir))
                    {
                        Directory.CreateDirectory(patchDir);
                    }

                    string tmpZip = Path.Combine(patchDir, "patch_" + Path.GetFileName(u.Url));

                    using (var client = new HttpClient())
                    {
                        client.Timeout = TimeSpan.FromSeconds(20);
                        var resp = await client.GetAsync(u.Url);
                        if (!resp.IsSuccessStatusCode)
                        {
                            label3.Text = "更新文件 " + u.RelPath + " 失败（HTTP " +
                                          (int)resp.StatusCode + "），已跳过。";
                            continue;
                        }

                        byte[] data = await resp.Content.ReadAsByteArrayAsync();
                        File.WriteAllBytes(tmpZip, data);
                    }

                    // 3）解压到 JSON 指定的目标文件（忽略 zip 内部路径）
                    ExtractZipSingleFileOverwrite(tmpZip, localPath);

                    // 4）解压后再算一次 MD5，看是否与 JSON 一致
                    string afterMd5 = (ComputeFileMd5(localPath) ?? "").ToLowerInvariant();
                    label3.Text = "更新完成：" + u.RelPath + " 新MD5=" + afterMd5;
                }
                catch (Exception ex)
                {
                    label3.Text = "更新文件 " + u.RelPath + " 失败，已跳过：" + ex.Message;
                }
            }

            label3.Text = "更新检查完成。";
        }


        // 覆盖解压
        private void ExtractZipToDirectoryOverwrite(string zipPath, string destDir)
        {
            try
            {
                using (ZipArchive archive = ZipFile.OpenRead(zipPath))
                {
                    foreach (ZipArchiveEntry entry in archive.Entries)
                    {
                        if (string.IsNullOrEmpty(entry.Name))
                        {
                            string dirPath = Path.Combine(destDir, entry.FullName.Replace('/', '\\'));
                            Directory.CreateDirectory(dirPath);
                            continue;
                        }

                        string fullPath = Path.Combine(destDir, entry.FullName.Replace('/', '\\'));
                        string folder = Path.GetDirectoryName(fullPath);
                        if (!Directory.Exists(folder))
                        {
                            Directory.CreateDirectory(folder);
                        }

                        entry.ExtractToFile(fullPath, true); // 覆盖
                    }
                }

                label3.Text = "已解压补丁：" + Path.GetFileName(zipPath);
            }
            catch (Exception ex)
            {
                label3.Text = "解压补丁失败：" + ex.Message;
            }
        }

        /// <summary>
        /// 将 zip 中第一个“真实文件”解压到 destFilePath（忽略 zip 自带目录结构）
        /// 例如：JSON 写的是 /Data/ztgame.dat，哪怕 zip 里只有 ztgame.dat，
        /// 也会强行解压到 游戏根目录\Data\ztgame.dat
        /// </summary>
        private void ExtractZipSingleFileOverwrite(string zipPath, string destFilePath)
        {
            try
            {
                using (ZipArchive archive = ZipFile.OpenRead(zipPath))
                {
                    ZipArchiveEntry targetEntry = null;

                    // 找到 zip 里第一个真正的文件（排除纯目录）
                    foreach (var entry in archive.Entries)
                    {
                        if (!string.IsNullOrEmpty(entry.Name))
                        {
                            targetEntry = entry;
                            break;
                        }
                    }

                    if (targetEntry == null)
                    {
                        label3.Text = "解压补丁失败：ZIP 中没有有效文件。";
                        return;
                    }

                    // 确保目标目录存在
                    string folder = Path.GetDirectoryName(destFilePath);
                    if (!Directory.Exists(folder))
                    {
                        Directory.CreateDirectory(folder);
                    }

                    // 已存在则先删除再写入
                    if (File.Exists(destFilePath))
                    {
                        File.Delete(destFilePath);
                    }

                    targetEntry.ExtractToFile(destFilePath);

                    label3.Text = "已更新文件：" + destFilePath;
                }
            }
            catch (Exception ex)
            {
                label3.Text = "解压补丁失败：" + ex.Message;
            }
        }

        // =================== 工具方法 & 导航 ===================

        private string GetString(string key)
        {
            if (_configRoot == null) return null;

            object value;
            if (_configRoot.TryGetValue(key, out value))
            {
                return value as string;
            }
            return null;
        }

        private int ToInt(object obj)
        {
            try
            {
                if (obj == null) return 0;
                if (obj is int) return (int)obj;
                if (obj is long) return (int)(long)obj;
                if (obj is double) return (int)(double)obj;

                int v;
                if (int.TryParse(obj.ToString(), out v)) return v;
            }
            catch { }
            return 0;
        }

        // 仿浏览器弹窗：改为使用系统默认浏览器打开，不再在登录器内嵌网页
        private void OpenPopup(string caption, string urlOrHtml, string emptyMsg)
        {
            if (string.IsNullOrWhiteSpace(urlOrHtml))
            {
                MessageBox.Show(emptyMsg, "提示");
                return;
            }

            using (var win = new WebPopup(caption, urlOrHtml))
            {
                win.ShowDialog(this);   // 模态弹窗
            }
        }
        //（暂时保留，不再使用）
        private void OpenInternal(string url, string fallbackMsg)
        {
            if (string.IsNullOrWhiteSpace(url))
            {
                MessageBox.Show(fallbackMsg, "提示");
                return;
            }

            WebBrowser target = WebBox2 ?? WebBox1;
            if (target == null)
            {
                MessageBox.Show("当前登录器未配置 WebBrowser 控件。", "提示");
                return;
            }

            try
            {
                string trimmed = url.TrimStart();
                if (trimmed.StartsWith("<"))
                {
                    target.DocumentText = url;
                }
                else
                {
                    target.Navigate(url);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("加载页面失败：\n" + ex.Message, "错误");
            }
        }

        private void btnOfficial_Click(object sender, EventArgs e)
        {
            OpenPopup("官网首页", _urlOfficial, "JSON 中未配置“官方首页”地址。");
        }

        private void btnRegister_Click(object sender, EventArgs e)
        {
            OpenPopup("账号注册", _urlRegister, "JSON 中未配置“注册地址”。");
        }

        private void btnRecharge_Click(object sender, EventArgs e)
        {
            OpenPopup("在线充值", _urlRecharge, "JSON 中未配置“充值地址”。");
        }

        private void btnLottery_Click(object sender, EventArgs e)
        {
            OpenPopup("在线抽奖", _urlLottery, "JSON 中未配置“抽奖地址”。");
        }

        private void btnRecover_Click(object sender, EventArgs e)
        {
            OpenPopup("找回密码", _urlPassword, "JSON 中未配置“密码地址”。");
        }

        // Designer 生成的一些空事件（如果你有的话）
        private void label1_Click(object sender, EventArgs e) { }
        private void label3_Click(object sender, EventArgs e) { }
        private void grpServers_Enter(object sender, EventArgs e) { }
        private void WebBox1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e) { }

        private void tvServers_AfterSelect_1(object sender, TreeViewEventArgs e)
        {

        }
    }
}

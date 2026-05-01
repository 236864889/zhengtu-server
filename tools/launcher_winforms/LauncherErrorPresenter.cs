using System.Collections.Generic;
using System.Windows.Forms;

namespace LauncherWinForms
{
    public static class LauncherErrorPresenter
    {
        private static readonly Dictionary<LauncherErrorCode, string> Messages = new Dictionary<LauncherErrorCode, string>
        {
            { LauncherErrorCode.E001, "主程序缺失（ztgame.dat/ztgame.exe）。" },
            { LauncherErrorCode.E002, "主程序不是有效 PE 可执行文件。" },
            { LauncherErrorCode.E003, "config.ini 写入失败。" },
            { LauncherErrorCode.E004, "关键资源缺失。" },
            { LauncherErrorCode.E005, "客户端进程启动失败。" },
            { LauncherErrorCode.E006, "工作目录错误。" },
            { LauncherErrorCode.E007, "区服配置无效。" },
            { LauncherErrorCode.E008, "权限不足。" },
            { LauncherErrorCode.E009, "疑似安全软件拦截。" }
        };

        public static void Show(LauncherErrorCode code, string details)
        {
            string msg = code + " - " + Messages[code];
            if (!string.IsNullOrEmpty(details)) msg += "\n\n" + details;
            MessageBox.Show(msg, "启动器错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}

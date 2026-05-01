using System;
using System.IO;

namespace LauncherWinForms
{
    public class LauncherConfig
    {
        public string ClientDir { get; set; }
        public bool UseCommandLineOverride { get; set; }
        public string LogDir
        {
            get { return Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "logs", "launcher"); }
        }

        public LauncherConfig()
        {
            ClientDir = AppDomain.CurrentDomain.BaseDirectory;
            UseCommandLineOverride = false;
        }
    }
}

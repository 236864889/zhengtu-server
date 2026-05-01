using System.Collections.Generic;
using System.IO;
using System.Text;

namespace LauncherWinForms
{
    public class ResourceCheckItem
    {
        public string Path { get; set; }
        public bool Required { get; set; }
        public bool Exists { get; set; }
        public string Message { get; set; }
    }

    public class ResourceCheckResult
    {
        public List<ResourceCheckItem> Items { get; set; }
        public bool CanLaunch { get; set; }

        public ResourceCheckResult()
        {
            Items = new List<ResourceCheckItem>();
        }

        public string ToUiSummary()
        {
            StringBuilder sb = new StringBuilder();
            sb.AppendLine(CanLaunch ? "关键资源检查通过" : "关键资源检查失败");
            foreach (ResourceCheckItem item in Items)
            {
                sb.AppendLine(string.Format("[{0}] {1} - {2}", item.Required ? "必须" : "建议", item.Path, item.Exists ? "存在" : "缺失"));
            }
            return sb.ToString();
        }
    }

    public static class ResourceIntegrityChecker
    {
        public static ResourceCheckResult Check(string clientDir)
        {
            ResourceCheckResult result = new ResourceCheckResult();
            Add(result, clientDir, "config.ini", true);

            bool hasDat = File.Exists(System.IO.Path.Combine(clientDir, "ztgame.dat"));
            bool hasExe = File.Exists(System.IO.Path.Combine(clientDir, "ztgame.exe"));
            result.Items.Add(new ResourceCheckItem
            {
                Path = "ztgame.dat|ztgame.exe",
                Required = true,
                Exists = hasDat || hasExe,
                Message = (hasDat || hasExe) ? "存在" : "缺失"
            });

            Add(result, clientDir, System.IO.Path.Combine("data", "datas.pak"), true);
            Add(result, clientDir, System.IO.Path.Combine("data", "interfaces.gl"), true);
            Add(result, clientDir, System.IO.Path.Combine("data", "login.gl"), true);
            Add(result, clientDir, System.IO.Path.Combine("data", "icons.gl"), true);
            Add(result, clientDir, System.IO.Path.Combine("data", "items.gl"), true);

            Add(result, clientDir, System.IO.Path.Combine("data", "logo.gl"), false);
            Add(result, clientDir, System.IO.Path.Combine("data", "sound.pak"), false);
            Add(result, clientDir, System.IO.Path.Combine("data", "Minimap.gl"), false);

            result.CanLaunch = true;
            foreach (ResourceCheckItem item in result.Items)
            {
                if (item.Required && !item.Exists)
                {
                    result.CanLaunch = false;
                    break;
                }
            }

            return result;
        }

        private static void Add(ResourceCheckResult result, string clientDir, string relPath, bool required)
        {
            string full = System.IO.Path.Combine(clientDir, relPath);
            bool exists = File.Exists(full);
            result.Items.Add(new ResourceCheckItem
            {
                Path = relPath,
                Required = required,
                Exists = exists,
                Message = exists ? "存在" : "缺失"
            });
        }
    }
}

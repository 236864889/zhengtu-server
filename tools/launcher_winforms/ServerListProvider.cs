using System;
using System.Collections.Generic;
using System.IO;
using System.Web.Script.Serialization;

namespace LauncherWinForms
{
    public class ServerEntry
    {
        public string name { get; set; }
        public string ip { get; set; }
        public int port { get; set; }
        public int zone { get; set; }
        public int version { get; set; }
        public string status { get; set; }

        public override string ToString()
        {
            return string.Format("{0} ({1}:{2})", name, ip, port);
        }
    }

    public class ServerListRoot
    {
        public List<ServerEntry> servers { get; set; }
    }

    public static class ServerListProvider
    {
        public static List<ServerEntry> Load(string jsonPath)
        {
            if (!File.Exists(jsonPath)) throw new FileNotFoundException("servers.json 不存在", jsonPath);
            string json = File.ReadAllText(jsonPath);
            JavaScriptSerializer serializer = new JavaScriptSerializer();
            ServerListRoot root = serializer.Deserialize<ServerListRoot>(json);
            if (root == null || root.servers == null || root.servers.Count == 0)
                throw new Exception("E007: 区服配置为空");

            foreach (ServerEntry s in root.servers)
            {
                if (string.IsNullOrWhiteSpace(s.name) || string.IsNullOrWhiteSpace(s.ip))
                    throw new Exception("E007: 区服 name/ip 不能为空");
                if (s.port < 1 || s.port > 65535)
                    throw new Exception("E007: 区服端口非法");
                if (s.zone < 0)
                    throw new Exception("E007: 区服 zone 非法");
                if (string.IsNullOrWhiteSpace(s.status)) s.status = "open";
            }
            return root.servers;
        }
    }
}

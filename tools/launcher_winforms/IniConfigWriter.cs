using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace LauncherWinForms
{
    public class IniWriteResult
    {
        public bool Success { get; set; }
        public string Message { get; set; }
    }

    public static class IniConfigWriter
    {
        public static IniWriteResult WriteServerConfig(string iniPath, ServerEntry server)
        {
            string backupPath = iniPath + ".bak";
            try
            {
                File.Copy(iniPath, backupPath, true);
                Dictionary<string, Dictionary<string, string>> ini = Parse(File.ReadAllLines(iniPath, Encoding.UTF8));

                Set(ini, "Server", "servername", server.name);
                Set(ini, "Server", "loginAddress", server.ip);
                Set(ini, "Server", "loginPort", server.port.ToString());
                Set(ini, "Server", "zone", server.zone.ToString());
                Set(ini, "Server", "autoreconnect", "1");
                Set(ini, "other", "version", server.version.ToString());

                File.WriteAllText(iniPath, Serialize(ini), Encoding.UTF8);

                Dictionary<string, Dictionary<string, string>> verify = Parse(File.ReadAllLines(iniPath, Encoding.UTF8));
                if (!Equal(Get(verify, "Server", "loginAddress"), server.ip) ||
                    !Equal(Get(verify, "Server", "loginPort"), server.port.ToString()) ||
                    !Equal(Get(verify, "Server", "zone"), server.zone.ToString()))
                {
                    throw new Exception("回读校验失败");
                }

                return new IniWriteResult { Success = true, Message = "OK" };
            }
            catch (Exception ex)
            {
                try
                {
                    if (File.Exists(backupPath)) File.Copy(backupPath, iniPath, true);
                }
                catch { }
                return new IniWriteResult { Success = false, Message = ex.Message };
            }
        }

        private static bool Equal(string a, string b) { return string.Equals(a ?? "", b ?? "", StringComparison.Ordinal); }
        private static string Get(Dictionary<string, Dictionary<string, string>> ini, string section, string key)
        {
            if (!ini.ContainsKey(section) || !ini[section].ContainsKey(key)) return "";
            return ini[section][key];
        }
        private static void Set(Dictionary<string, Dictionary<string, string>> ini, string section, string key, string value)
        {
            if (!ini.ContainsKey(section)) ini[section] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            ini[section][key] = value;
        }

        private static Dictionary<string, Dictionary<string, string>> Parse(string[] lines)
        {
            var result = new Dictionary<string, Dictionary<string, string>>(StringComparer.OrdinalIgnoreCase);
            string sec = "";
            foreach (string raw in lines)
            {
                string line = raw.Trim();
                if (line.Length == 0 || line.StartsWith(";") || line.StartsWith("#")) continue;
                if (line.StartsWith("[") && line.EndsWith("]"))
                {
                    sec = line.Substring(1, line.Length - 2);
                    if (!result.ContainsKey(sec)) result[sec] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                    continue;
                }
                int i = line.IndexOf('=');
                if (i > 0)
                {
                    string k = line.Substring(0, i).Trim();
                    string v = line.Substring(i + 1).Trim();
                    if (!result.ContainsKey(sec)) result[sec] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                    result[sec][k] = v;
                }
            }
            return result;
        }

        private static string Serialize(Dictionary<string, Dictionary<string, string>> ini)
        {
            StringBuilder sb = new StringBuilder();
            foreach (var sec in ini)
            {
                sb.AppendLine("[" + sec.Key + "]");
                foreach (var kv in sec.Value)
                {
                    sb.AppendLine(kv.Key + "=" + kv.Value);
                }
                sb.AppendLine();
            }
            return sb.ToString();
        }
    }
}

using System;
using System.IO;
using System.Text;

namespace LauncherWinForms
{
    public static class LauncherLogger
    {
        public static string EnsureLogDir()
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "logs", "launcher");
            if (!Directory.Exists(path)) Directory.CreateDirectory(path);
            return path;
        }

        public static void LogLaunch(string clientDir, string clientExe, ServerEntry server, ResourceCheckResult check, IniWriteResult iniResult, int pid, Exception ex)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("{\"time\":\"").Append(DateTime.UtcNow.ToString("o")).Append("\"");
            sb.Append(",\"clientDir\":\"").Append(Escape(clientDir)).Append("\"");
            sb.Append(",\"clientExe\":\"").Append(Escape(clientExe)).Append("\"");
            sb.Append(",\"server\":\"").Append(Escape(server != null ? server.name : "")).Append("\"");
            sb.Append(",\"ip\":\"").Append(Escape(server != null ? server.ip : "")).Append("\"");
            sb.Append(",\"port\":").Append(server != null ? server.port.ToString() : "0");
            sb.Append(",\"zone\":").Append(server != null ? server.zone.ToString() : "0");
            sb.Append(",\"version\":").Append(server != null ? server.version.ToString() : "0");
            sb.Append(",\"resourceCanLaunch\":").Append(check != null && check.CanLaunch ? "true" : "false");
            sb.Append(",\"iniWriteSuccess\":").Append(iniResult != null && iniResult.Success ? "true" : "false");
            sb.Append(",\"pid\":").Append(pid);
            if (ex != null) sb.Append(",\"exception\":\"").Append(Escape(ex.ToString())).Append("\"");
            sb.Append("}");
            WriteLine(sb.ToString());
        }

        public static void LogException(string code, string message)
        {
            WriteLine("{\"time\":\"" + DateTime.UtcNow.ToString("o") + "\",\"error\":\"" + Escape(code) + "\",\"message\":\"" + Escape(message) + "\"}");
        }

        private static void WriteLine(string line)
        {
            string logPath = Path.Combine(EnsureLogDir(), "launcher.log");
            File.AppendAllText(logPath, line + Environment.NewLine, Encoding.UTF8);
        }

        private static string Escape(string s)
        {
            return (s ?? "").Replace("\\", "\\\\").Replace("\"", "\\\"");
        }
    }
}

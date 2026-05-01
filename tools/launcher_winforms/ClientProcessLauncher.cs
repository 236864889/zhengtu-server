using System;
using System.Diagnostics;
using System.IO;

namespace LauncherWinForms
{
    public static class ClientProcessLauncher
    {
        public static int StartClient(string exePath, string workingDirectory, string arguments)
        {
            if (!Directory.Exists(workingDirectory))
                throw new LauncherException(LauncherErrorCode.E006, "工作目录不存在");

            try
            {
                ProcessStartInfo psi = new ProcessStartInfo();
                psi.FileName = exePath;
                psi.WorkingDirectory = workingDirectory;
                psi.UseShellExecute = false;
                psi.Arguments = arguments ?? string.Empty;

                Process process = Process.Start(psi);
                if (process == null)
                    throw new LauncherException(LauncherErrorCode.E005, "启动失败，未返回进程");

                return process.Id;
            }
            catch (LauncherException)
            {
                throw;
            }
            catch (UnauthorizedAccessException ex)
            {
                throw new LauncherException(LauncherErrorCode.E008, ex.Message, ex);
            }
            catch (Exception ex)
            {
                throw new LauncherException(LauncherErrorCode.E005, ex.Message, ex);
            }
        }
    }
}

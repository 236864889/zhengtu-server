using System;
using System.IO;

namespace LauncherWinForms
{
    public static class ClientExecutableDetector
    {
        public static bool IsWindowsPE(string path)
        {
            try
            {
                if (!File.Exists(path)) return false;
                using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                using (BinaryReader br = new BinaryReader(fs))
                {
                    if (fs.Length < 0x40) return false;
                    if (br.ReadUInt16() != 0x5A4D) return false; // MZ
                    fs.Position = 0x3C;
                    int peOffset = br.ReadInt32();
                    if (peOffset <= 0 || peOffset > fs.Length - 4) return false;
                    fs.Position = peOffset;
                    return br.ReadUInt32() == 0x00004550; // PE\0\0
                }
            }
            catch
            {
                return false;
            }
        }

        public static string PickClientExecutable(string clientDir)
        {
            string dat = Path.Combine(clientDir, "ztgame.dat");
            string exe = Path.Combine(clientDir, "ztgame.exe");

            if (File.Exists(dat) && IsWindowsPE(dat)) return dat;
            if (File.Exists(exe) && IsWindowsPE(exe)) return exe;

            if (!File.Exists(dat) && !File.Exists(exe))
                throw new LauncherException(LauncherErrorCode.E001, "主程序缺失");
            throw new LauncherException(LauncherErrorCode.E002, "主程序不是有效 PE 文件");
        }
    }
}

using System.Diagnostics;
using Microsoft.Win32;

namespace WindowsFormsApplication1
{
    internal static class BrowserEmulation
    {
        /// <summary>
        /// 把当前 EXE 加到 IE 浏览器仿真列表里，强制用 IE11 模式渲染 WebBrowser
        /// </summary>
        public static void EnableIE11()
        {
            try
            {
                string exeName = Process.GetCurrentProcess().ProcessName + ".exe";

                // ★ IE 内核仿真版本
                using (var key = Registry.CurrentUser.CreateSubKey(
                    @"Software\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_BROWSER_EMULATION",
                    RegistryKeyPermissionCheck.ReadWriteSubTree))
                {
                    // 11001 = IE11 Edge 模式，11000 = IE11 标准模式
                    const int ie11 = 11001;
                    object cur = key.GetValue(exeName);

                    if (cur == null || (cur is int && (int)cur != ie11))
                    {
                        key.SetValue(exeName, ie11, RegistryValueKind.DWord);
                    }
                }

                // ★ 开启 GPU 渲染（可选，提升一些渲染效果）
                using (var gpuKey = Registry.CurrentUser.CreateSubKey(
                    @"Software\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_GPU_RENDERING",
                    RegistryKeyPermissionCheck.ReadWriteSubTree))
                {
                    gpuKey.SetValue(exeName, 1, RegistryValueKind.DWord);
                }
            }
            catch
            {
                // 写注册表失败就忽略，不影响主程序
            }
        }
    }
}

# 08 Launcher Patch Round1 Summary

## 本轮定位到的登录器源码
- 旧登录器：`登录器/WindowsFormsApplication1`（含 `Form1/WebBox/更新/自更新` 逻辑）。
- 本轮主用项目：`tools/launcher_winforms`（VS2015 兼容 WinForms 最小改造工程）。

## 本轮新增文件（已在主用项目中实现）
- `tools/launcher_winforms/ClientExecutableDetector.cs`
- `tools/launcher_winforms/IniConfigWriter.cs`
- `tools/launcher_winforms/ServerListProvider.cs`
- `tools/launcher_winforms/ResourceIntegrityChecker.cs`
- `tools/launcher_winforms/ClientProcessLauncher.cs`
- `tools/launcher_winforms/LauncherLogger.cs`
- `tools/launcher_winforms/LauncherErrorCode.cs`
- `tools/launcher_winforms/LauncherErrorPresenter.cs`
- `tools/launcher_winforms/servers.json`

## 本轮修改文件
- `tools/launcher_winforms/MainForm.cs`
- `tools/launcher_winforms/LauncherWinForms.csproj`
- `tools/launcher_winforms/launcher_winforms.sln`
- `tools/launcher_winforms/Program.cs`

## 核心流程说明
1. 读取客户端目录并加载 `servers.json`。
2. 获取所选区服并校验字段。
3. `ClientExecutableDetector` 识别 `ztgame.dat` / `ztgame.exe`，优先 `ztgame.dat`。
4. `ResourceIntegrityChecker` 校验必须项与建议项资源。
5. `IniConfigWriter` 备份并写入 `config.ini` 指定字段，回读校验，失败回滚。
6. `ClientProcessLauncher` 以 `UseShellExecute=false` 启动客户端，默认空参数。
7. `LauncherLogger` 输出结构化日志到 `logs/launcher/launcher.log`。
8. 出错时通过 `LauncherErrorCode + LauncherErrorPresenter` 显示错误码与可读提示。

## 风险点
- 目标机器若缺失 .NET Framework 4.6.1 运行时，程序无法启动。
- 若客户端目录权限受限，`config.ini` 备份或回写可能触发 `E008`。
- 某些安全软件可能拦截进程启动，需结合 `E009` 日志定位。
- `servers.json` 字段类型异常会被判为 `E007`。

## 未完成项
- 未引入“命令行覆盖灰度开关”的 UI 配置入口，当前保持默认 `false`，不传参数。
- 未对旧登录器 `登录器/WindowsFormsApplication1` 进行并行迁移，本轮按最小可联调目标落在 `tools/launcher_winforms`。

## 下一轮建议
- 增加“命令行覆盖灰度开关”并默认关闭。
- 增加日志查看器与日志滚动切分。
- 增加更细粒度资源检查报告导出，例如 JSON。
- 补充 Win7 / Win10 / Win11 实机联调记录与错误码闭环样例。

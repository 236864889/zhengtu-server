# 09 Launcher Round2 Build Check

## 1. 检查到的项目路径
- `tools/launcher_winforms`

## 2. sln/csproj 路径
- `tools/launcher_winforms/launcher_winforms.sln`
- `tools/launcher_winforms/LauncherWinForms.csproj`

## 3. TargetFrameworkVersion
- `v4.6.1`（.NET Framework 4.x，VS2015 可支持）

## 4. 引用检查结果
已确认包含：
- `System`
- `System.Core`
- `System.Data`
- `System.Drawing`
- `System.Windows.Forms`
- `System.Web.Extensions`（`JavaScriptSerializer` 依赖）
- `System.Xml`

## 5. Compile Include 检查结果
已包含以下关键源文件：
- `Program.cs`
- `MainForm.cs`
- `ClientExecutableDetector.cs`
- `IniConfigWriter.cs`
- `ServerListProvider.cs`
- `ResourceIntegrityChecker.cs`
- `ClientProcessLauncher.cs`
- `LauncherLogger.cs`
- `LauncherErrorCode.cs`
- `LauncherErrorPresenter.cs`

## 6. Content Include 检查结果
- `servers.json` 已调整为 `Content Include`，并设置 `CopyToOutputDirectory=PreserveNewest`，运行目录可用。

## 7. VS2015 兼容性检查结果
- 项目为旧式 csproj（`ToolsVersion=14.0`），符合 VS2015 工程格式。
- 未发现 `using var`、`?.`、`nameof`、`record`、`init` 等高版本语法。
- 代码使用传统 `using (...)`、显式类型、WinForms 经典入口，兼容 C# 5/6。

## 8. 实际编译结果或无法编译原因
- 已尝试执行：
  - `msbuild tools/launcher_winforms/launcher_winforms.sln /p:Configuration=Debug`
- 当前环境返回：`msbuild: command not found`。
- 结论：当前环境无法实际调用 VS2015/MSBuild，已完成静态编译风险检查。

## 9. 已修复问题列表
1. `servers.json` 项类型从 `None Include` 修正为 `Content Include`，满足运行目录资源复制要求。
2. `ClientExecutableDetector.PickClientExecutable` 的异常类型由通用 `Exception` 改为 `LauncherException`，与错误码体系对齐（E001/E002），避免主流程误归类为 E005。

## 10. 剩余风险
- 未在 Windows + VS2015 实机执行真实编译，仍可能存在环境特有引用/平台问题。
- `MainForm.cs` 存在未使用 using（如 `System.Text`），不影响编译但建议后续清理。

## 11. 下一步建议
1. 在 Windows 7/10/11 + VS2015 安装环境中打开 `launcher_winforms.sln` 实编。
2. 若有编译错误，按错误文件/行号逐项修复并补充 `10_launcher_compile_errors.md`。
3. 编译通过后进入受控客户端目录联调（只做目录选择、资源检查、config.ini 写入、进程启动与日志验证）。

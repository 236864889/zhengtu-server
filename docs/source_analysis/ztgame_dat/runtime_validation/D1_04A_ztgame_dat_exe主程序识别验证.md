# D1-04A ztgame.dat / ztgame.exe 主程序识别验证

## 1. 验证目标

本方案用于确认 C# 登录器应启动 `ztgame.dat` 还是 `ztgame.exe`，避免误启动、无法启动、工作目录错误或客户端目录被破坏。

---

## 2. 文件识别原则

识别主程序时应遵循：

1. 不能只看扩展名（`.dat` 不等于“数据文件”，可能是改名 EXE）；
2. 必须检查文件头（DOS Header）；
3. Windows PE 文件通常以 `MZ` 开头；
4. 还应检查 `e_lfanew` 指向位置是否为 `PE\0\0`；
5. 只有同时满足 `MZ + PE`，才判定为可执行 PE。

---

## 3. 推荐检测方法

> 说明：以下命令用于**只读检测**，不修改客户端文件。

### 3.1 查看候选文件

```powershell
dir ztgame.*
```

### 3.2 查看 `ztgame.dat` 前 64 字节

```powershell
Format-Hex -Path .\ztgame.dat -Count 64
```

### 3.3 查看 `ztgame.exe` 前 64 字节

```powershell
Format-Hex -Path .\ztgame.exe -Count 64
```

### 3.4 计算 MD5 / SHA256

```powershell
Get-FileHash .\ztgame.dat -Algorithm MD5
Get-FileHash .\ztgame.dat -Algorithm SHA256
Get-FileHash .\ztgame.exe -Algorithm MD5
Get-FileHash .\ztgame.exe -Algorithm SHA256
```

### 3.5 判断 MZ / PE（示意）

- 偏移 `0x00` 读到 `4D 5A` -> `MZ`；
- 偏移 `0x3C` 读 `e_lfanew`；
- 在 `e_lfanew` 指向位置读到 `50 45 00 00` -> `PE\0\0`。

---

## 4. C# 登录器识别逻辑

推荐逻辑（伪代码）：

1. 检查 `ztgame.dat` 是否存在；
2. 若存在，执行 `IsWindowsPE(datPath)`；
3. 若 `dat` 是 PE，优先选 `dat`；
4. 否则检查 `ztgame.exe` 是否存在且是 PE；
5. 若 `exe` 是 PE，选 `exe`；
6. 两者都不可用则提示“主程序缺失或损坏”。

原因：在该类老项目中，`ztgame.dat` 可能就是实际主程序命名。

---

## 5. 是否需要重命名

明确结论：

- 不建议登录器自动重命名 `ztgame.dat`；
- 不建议自动覆盖 `ztgame.exe`；
- 如需人工重命名，必须先完整备份；
- 登录器应优先采用“只读识别 + 直接启动”。

---

## 6. 启动路径与工作目录

启动时必须满足：

- `ProcessStartInfo.FileName` = 主程序完整路径（dat 或 exe）；
- `ProcessStartInfo.WorkingDirectory` = 客户端根目录；
- 不能将工作目录设置为 `launcher` 目录。

否则可能导致客户端找不到：

- `config.ini`
- `data/datas.pak`
- `data/*.gl`

进而出现“黑屏/资源缺失/登录异常”等误判问题。

---

## 7. 命令行参数关系

结合 P0-01 / P1-04 结论：

- 常规建议：不传参数，仅写 `config.ini`；
- 灰度覆盖时：可传 `__argc==5` 格式覆盖 `loginAddress/loginPort/zone`；
- 参数数量错误可能触发异常分支，登录器应严格约束参数构造。

---

## 8. 测试矩阵

| 场景 | ztgame.dat | ztgame.exe | 预期选择 | 预期提示 |
|---|---|---|---|---|
| 1 | 存在且是PE | 不存在 | 启动 dat | 无错误 |
| 2 | 不存在 | 存在且是PE | 启动 exe | 无错误 |
| 3 | 存在且是PE | 存在且是PE | 优先 dat | 记录“DAT优先”日志 |
| 4 | 存在但非PE | 存在且是PE | 启动 exe | 提示 dat 非PE（可选警告） |
| 5 | 存在但非PE | 存在但非PE | 不启动 | 主程序不是PE |
| 6 | 不存在 | 不存在 | 不启动 | 主程序不存在 |
| 7 | 存在但损坏 | 不存在 | 不启动 | dat 文件损坏 |
| 8 | 不存在 | 存在但损坏 | 不启动 | exe 文件损坏 |

---

## 9. 登录器错误提示策略

| 错误 | 判断方式 | 提示文案 | 是否阻断启动 |
|---|---|---|---|
| 主程序不存在 | dat/exe 均不存在 | 未找到客户端主程序（ztgame.dat/ztgame.exe） | 是 |
| 主程序不是 PE | 文件存在但 `MZ/PE` 校验失败 | 客户端主程序格式异常（非Windows可执行文件） | 是 |
| 无权限启动 | Process.Start 抛 `UnauthorizedAccessException` | 无权限启动客户端，请以管理员权限重试 | 是 |
| 工作目录错误 | 启动后资源读取失败/日志指向错误目录 | 客户端工作目录错误，请检查启动配置 | 是 |
| 杀软拦截 | 启动返回码/系统事件/进程瞬退 | 客户端可能被安全软件拦截，请加入白名单 | 是 |
| 文件损坏 | PE结构不完整或哈希异常 | 客户端文件损坏，请修复或重装 | 是 |

---

## 10. 推荐 C# 实现片段

> 简化示例（伪代码风格，强调只读判断与日志输出）：

```csharp
bool IsWindowsPE(string path)
{
    if (!File.Exists(path)) return false;
    using var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
    using var br = new BinaryReader(fs);

    // MZ
    if (br.ReadUInt16() != 0x5A4D) return false;

    // e_lfanew @ 0x3C
    fs.Position = 0x3C;
    int peOffset = br.ReadInt32();
    if (peOffset <= 0 || peOffset > fs.Length - 4) return false;

    // PE\0\0
    fs.Position = peOffset;
    return br.ReadUInt32() == 0x00004550;
}

string PickClientExecutable(string clientDir)
{
    string dat = Path.Combine(clientDir, "ztgame.dat");
    string exe = Path.Combine(clientDir, "ztgame.exe");

    if (File.Exists(dat) && IsWindowsPE(dat)) return dat; // DAT 优先
    if (File.Exists(exe) && IsWindowsPE(exe)) return exe;

    throw new InvalidOperationException("主程序缺失或损坏");
}

Process StartClient(string exePath, string clientDir, string args)
{
    var psi = new ProcessStartInfo
    {
        FileName = exePath,
        WorkingDirectory = clientDir,
        Arguments = args ?? string.Empty,
        UseShellExecute = false
    };
    // 只启动，不改名、不改文件
    return Process.Start(psi) ?? throw new Exception("启动失败");
}
```

实现要求：

- 不修改文件；
- 不重命名；
- 只读判断；
- 输出日志（选中路径、PE判断结果、启动参数、PID）。

---

## 11. 联调验收标准

必须满足：

- 能稳定识别主程序（DAT/EXE）；
- 不破坏客户端文件；
- 工作目录设置正确；
- `config.ini` 能被客户端读取；
- `data` 资源能被客户端读取；
- 错误提示明确且可操作。

---

## 12. 结论与下一步

- 若主程序识别已清楚：下一步进入 **C# 登录器源码级改造方案**；
- 若仍不确定：需要用户提供实际客户端目录中 `ztgame.dat/ztgame.exe` 的文件头检测结果（MZ/PE 与哈希），再定最终优先策略。

---

## 禁止事项（执行约束）

- 不要修改客户端源码。
- 不要重命名 dat/exe。
- 不要运行未知 exe。
- 不要删除文件。
- 不要解包资源。
- 不要生成自动修复代码。
- 不要做空泛总结。

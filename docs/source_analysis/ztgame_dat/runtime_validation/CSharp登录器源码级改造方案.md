# C# 登录器源码级改造方案

## 1. 改造目标

本方案用于指导 C# 登录器源码改造，让登录器在不改客户端协议与核心逻辑的前提下，稳定联动 ztgame.dat 客户端，具备可维护、可诊断、可扩展的工程结构。

核心目标：

1. 自动识别 `ztgame.dat/ztgame.exe`；
2. PE 文件头校验；
3. `config.ini` 安全写入；
4. 区服 JSON 配置；
5. 资源完整性检查；
6. 启动前错误提示；
7. 启动日志；
8. 可选命令行参数；
9. 保留客户端 `GuiLogin` 流程；
10. 不处理 RC5/DES，不直接发协议。

---

## 2. 改造边界

- 登录器负责：区服配置、资源检查、`config.ini` 写入、启动客户端进程。
- 客户端负责：登录 UI、账号协议、RC5/DES 切换、二跳连接、选角进场景。
- 服务端负责：账号验证、版本验证、下发二跳地址与会话数据。

边界原则：登录器只做“准备 + 启动 + 提示”，不做“协议/会话/加密逻辑替代”。

---

## 3. 推荐模块划分

| 模块 | 类名建议 | 职责 |
|---|---|---|
| 启动器配置 | `LauncherConfig` | 客户端路径、日志路径、参数策略、开关项 |
| 区服提供器 | `ServerListProvider` | 加载/校验 JSON 区服列表并提供给 UI |
| 主程序识别 | `ClientExecutableDetector` | `dat/exe` 存在性与 PE 校验、选择启动目标 |
| INI 写入器 | `IniConfigWriter` | 备份、写入、回读校验、失败回滚 |
| 资源检查器 | `ResourceIntegrityChecker` | 按“必须/建议”清单检查资源 |
| 进程启动器 | `ClientProcessLauncher` | 组装 `ProcessStartInfo`、启动并返回 PID |
| 日志器 | `LauncherLogger` | JSON Lines 结构化日志输出 |
| 错误展示器 | `LauncherErrorPresenter` | 错误码到用户提示映射、弹窗与复制详情 |

---

## 4. 主程序识别模块

设计要点：

- `IsWindowsPE(path)`：只读校验 `MZ + PE\0\0`；
- `PickClientExecutable(clientDir)`：dat 优先、exe 兜底；
- 不重命名、不覆盖、不改文件。

示例代码：

```csharp
public static class ClientExecutableDetector
{
    public static bool IsWindowsPE(string path)
    {
        if (!File.Exists(path)) return false;
        using var fs = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
        using var br = new BinaryReader(fs);

        if (fs.Length < 0x40) return false;
        if (br.ReadUInt16() != 0x5A4D) return false; // MZ

        fs.Position = 0x3C;
        int peOffset = br.ReadInt32();
        if (peOffset <= 0 || peOffset > fs.Length - 4) return false;

        fs.Position = peOffset;
        return br.ReadUInt32() == 0x00004550; // PE\0\0
    }

    public static string PickClientExecutable(string clientDir)
    {
        var dat = Path.Combine(clientDir, "ztgame.dat");
        var exe = Path.Combine(clientDir, "ztgame.exe");

        if (File.Exists(dat) && IsWindowsPE(dat)) return dat; // DAT 优先
        if (File.Exists(exe) && IsWindowsPE(exe)) return exe;

        throw new InvalidOperationException("E001/E002: 主程序缺失或不是PE");
    }
}
```

---

## 5. config.ini 写入模块

设计目标：

- 写入：
  - `Server/servername`
  - `Server/loginAddress`
  - `Server/loginPort`
  - `Server/zone`
  - `Server/autoreconnect`
  - `other/version`
- 保留：`video/*`、`audio/*`；
- 写入前备份；
- 写入后回读校验；
- 失败自动回滚。

伪代码结构：

```csharp
public WriteResult WriteServerConfig(string iniPath, ServerEntry server)
{
    string bak = iniPath + ".bak";
    File.Copy(iniPath, bak, overwrite: true);

    try
    {
        var ini = IniFile.Load(iniPath);
        ini.Set("Server", "servername", server.Name);
        ini.Set("Server", "loginAddress", server.Ip);
        ini.Set("Server", "loginPort", server.Port.ToString());
        ini.Set("Server", "zone", server.Zone.ToString());
        ini.Set("Server", "autoreconnect", server.AutoReconnect ? "1" : "0");
        ini.Set("other",  "version", server.Version.ToString());

        ini.Save(iniPath);

        var verify = IniFile.Load(iniPath);
        if (!VerifyServerFields(verify, server))
            throw new Exception("Verify failed");

        return WriteResult.Success();
    }
    catch
    {
        File.Copy(bak, iniPath, overwrite: true); // 回滚
        return WriteResult.Fail("E003", "config.ini 写入失败并已回滚");
    }
}
```

---

## 6. 区服 JSON 配置模块

推荐格式：

```json
{
  "servers": [
    {
      "name": "测试一区",
      "ip": "127.0.0.1",
      "port": 7000,
      "zone": 1,
      "version": 1999,
      "status": "open"
    }
  ]
}
```

实现建议：

1. 启动时加载 JSON；
2. 校验字段：`name/ip/port/zone/version/status`；
3. 校验范围：`port(1~65535)`、`zone>=0`；
4. 绑定到区服下拉列表；
5. `status!=open` 的区服可展示但禁用启动按钮。

---

## 7. 资源完整性检查模块

检查清单：

- `config.ini`
- `ztgame.dat / ztgame.exe`（至少一个且可执行）
- `data/datas.pak`
- `data/interfaces.gl`
- `data/login.gl`
- `data/logo.gl`
- `data/icons.gl`
- `data/items.gl`
- `data/sound.pak`
- `data/Minimap.gl`

分级建议：

- **必须项（缺失阻断）**：`config.ini`、主程序、`datas.pak`、`interfaces.gl`、`login.gl`、`icons.gl`、`items.gl`
- **建议项（缺失警告）**：`logo.gl`、`sound.pak`、`Minimap.gl`

---

## 8. 启动进程模块

设计要点：

- `ProcessStartInfo.FileName` = 主程序完整路径；
- `WorkingDirectory` = 客户端根目录；
- `Arguments` 可选；
- `UseShellExecute` 建议 `false`；
- 启动成功记录 PID；
- 启动失败捕获异常并映射错误码。

示例：

```csharp
public Process StartClient(string exePath, string clientDir, string args)
{
    var psi = new ProcessStartInfo
    {
        FileName = exePath,
        WorkingDirectory = clientDir,
        Arguments = args ?? string.Empty,
        UseShellExecute = false
    };

    return Process.Start(psi) ?? throw new InvalidOperationException("E005: 启动失败");
}
```

---

## 9. 命令行参数策略

- 默认不传参数（依赖 `config.ini`）；
- 灰度切服时支持 `__argc==5` 兼容格式；
- 参数必须严格构造（避免多余参数）；
- 参数优先级高于 `config.ini`；
- 非灰度场景禁止乱传参数。

推荐封装：`BuildClientArgs(overrideMode, ip, port, zone)`，统一参数生成。

---

## 10. 错误提示与错误码

| 错误码 | 含义 | 是否阻断 | 用户提示 |
|---|---|---|---|
| E001 | 主程序缺失 | 是 | 未找到客户端主程序（ztgame.dat/ztgame.exe） |
| E002 | 主程序不是 PE | 是 | 客户端主程序格式异常（非可执行文件） |
| E003 | config.ini 写入失败 | 是 | 配置写入失败，请检查权限或磁盘状态 |
| E004 | 关键资源缺失 | 是 | 客户端资源不完整，请修复后重试 |
| E005 | 进程启动失败 | 是 | 客户端启动失败，请检查环境或安全软件 |
| E006 | 工作目录错误 | 是 | 启动目录错误，请检查客户端路径配置 |
| E007 | 区服配置无效 | 是 | 区服配置错误，请检查 JSON 配置 |
| E008 | 无权限 | 是 | 权限不足，请以管理员身份运行 |
| E009 | 杀软拦截疑似 | 是 | 可能被安全软件拦截，请加入白名单后重试 |

---

## 11. 日志设计

建议使用 JSON Lines（每行一条事件），字段至少包含：

- `time`
- `clientDir`
- `exePath`
- `exeType`（dat/exe）
- `serverName`
- `ip`
- `port`
- `zone`
- `version`
- `resourceCheck`
- `iniWrite`
- `args`
- `pid`
- `errorCode`
- `exception`

示例：

```json
{"time":"2026-05-01T12:00:00Z","event":"launch","exePath":"D:/client/ztgame.dat","pid":1234}
```

---

## 12. UI 改造建议

建议在登录器 UI 中提供：

- 区服列表；
- 公告页；
- 资源检查按钮；
- 启动按钮；
- 错误提示弹窗；
- 日志导出按钮。

明确禁止：

- 不直接嵌入协议登录逻辑；
- 不替代客户端 `GuiLogin`。

---

## 13. 安全与兼容建议

- 不保存明文密码；
- 不修改 `dat/exe` 文件；
- 不绕过客户端登录；
- 不直接连接游戏服；
- 保留旧 `config.ini` 字段兼容；
- 兼容 Windows 7/10/11；
- 涉及写文件/启动进程时注意权限控制。

---

## 14. 联调测试清单

| 测试项 | 操作 | 预期结果 |
|---|---|---|
| 主程序识别 | dat/exe 多场景测试 | 选择正确主程序 |
| 区服切换 | 选择不同区服 | UI 与配置同步 |
| ini写入 | 写入后回读 | 字段一致且可回滚 |
| 资源缺失 | 临时缺失关键文件 | 阻断并提示 E004 |
| 正常启动 | 正确配置启动 | 出现客户端登录界面 |
| 错误IP | 写错 loginAddress | 登录失败并可提示 |
| 错误端口 | 写错 loginPort | 登录失败并可提示 |
| 错误版本 | 写错 version | 版本失败提示 |
| 权限不足 | ini只读/无权限 | E003/E008 提示 |
| 工作目录错误 | 故意设错目录 | E006 或资源读取失败提示 |

---

## 15. 推荐实施顺序

1. 主程序识别；
2. `config.ini` 写入；
3. 资源检查；
4. 区服 JSON；
5. 启动日志；
6. UI 优化；
7. 自动更新。

---

## 16. 结论与下一步

- 若方案明确：下一步进入 **C# 登录器代码补丁任务**；
- 若暂无登录器源码：先建立最小登录器源码框架（模块化骨架 + 日志 + 启动链路）。

---

## 禁止事项（执行约束）

- 不要修改客户端源码。
- 不要修改协议。
- 不要处理 RC5/DES。
- 不要直接发登录协议。
- 不要运行未知 exe。
- 不要生成自动修复代码。
- 不要做空泛总结。

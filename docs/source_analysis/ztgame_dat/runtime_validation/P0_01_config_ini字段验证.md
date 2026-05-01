# P0-01 config.ini 字段验证

## 1. 验证目标
静态确认 `ztgame.dat` 客户端在**启动、登录、选服、重连**相关流程中，对 `config.ini` 的全部读写字段、来源与优先级，为 C# 登录器写入策略提供依据。

## 2. 搜索范围与方法
- 重点文件：
  - `Client/GameAppation.cpp`
  - `Client/NetAutoConnect.cpp/.h`
  - `Client/Client.cpp/.h`
  - `Client/GuiLogin.cpp`
  - `Client/GuiSelectCharDialog.cpp`
- 检索关键词：`config.ini / ini.Open / ini.Save / GetInt / GetString / GetBool / SetInt / SetString / __argc / __targv / loginAddress / loginPort / zone / version / CClient::Connect / LoginGameServer`。

## 3. config.ini 读取字段总表

| 序号 | 字段名 | section | 默认值 | 源码文件 | 函数 | 用途 | 是否适合登录器写入 |
|---|---|---|---|---|---|---|---|
| 1 | servername | Server | 空字符串 | GameAppation.cpp | `CGameAppation::CGameAppation` | 拼接窗口标题显示区服名 | 是（推荐） |
| 2 | limitFPS | video | true | GameAppation.cpp | `LoadConfig` | 调试/测试帧率限制 | 否 |
| 3 | ScreenWidth | video | 1024 | GameAppation.cpp | `LoadConfig` | 分辨率宽 | 否 |
| 4 | ScreenHeight | video | 768 | GameAppation.cpp | `LoadConfig` | 分辨率高 | 否 |
| 5 | Window | video | false | GameAppation.cpp | `LoadConfig` | 窗口模式 | 否 |
| 6 | ColorBits | video | 32 | GameAppation.cpp | `LoadConfig` | 色深 | 否 |
| 7 | VertSync | video | false | GameAppation.cpp | `LoadConfig` | 垂直同步 | 否 |
| 8 | LightMap | video | false | GameAppation.cpp | `LoadConfig` | 光照贴图开关 | 否 |
| 9 | Weather | video | false | GameAppation.cpp | `LoadConfig` | 天气效果开关 | 否 |
| 10 | MultiTex | video | true | GameAppation.cpp | `LoadConfig` | 多重纹理开关 | 否 |
| 11 | HardwareCursor | video | true | GameAppation.cpp | `LoadConfig` | 硬件光标 | 否 |
| 12 | UseDXT | video | false | GameAppation.cpp | `LoadConfig` | DXT 纹理开关 | 否 |
| 13 | sound | audio | true | GameAppation.cpp | `LoadConfig` | 总音效开关 | 否 |
| 14 | scenesound | audio | true | GameAppation.cpp | `LoadConfig` | 场景音效开关 | 否 |
| 15 | circumstancesound | audio | true | GameAppation.cpp | `LoadConfig` | 环境音开关 | 否 |
| 16 | backgroundsound | audio | true | GameAppation.cpp | `LoadConfig` | 背景音乐开关 | 否 |
| 17 | soundvolume | audio | 50 | GameAppation.cpp | `LoadConfig` | 总音量 | 否 |
| 18 | scenevolume | audio | 50 | GameAppation.cpp | `LoadConfig` | 场景音量 | 否 |
| 19 | circumstancevolume | audio | 50 | GameAppation.cpp | `LoadConfig` | 环境音量 | 否 |
| 20 | backvolume | audio | 50 | GameAppation.cpp | `LoadConfig` | 背景音乐音量 | 否 |
| 21 | autoreconnect | Server | false | GameAppation.cpp | `LoadConfig` | 自动重连开关 | 可选 |
| 22 | single | other | false | GameAppation.cpp | `LoadConfig` | 单机/调试模式（仅特定编译） | 否 |
| 23 | InitMap | other | fenghuangcheng-main | GameAppation.cpp | `LoadConfig` | 初始地图名 | 否 |
| 24 | InitX | other | 100 | GameAppation.cpp | `LoadConfig` | 初始坐标X | 否 |
| 25 | InitY | other | 100 | GameAppation.cpp | `LoadConfig` | 初始坐标Y | 否 |
| 26 | loginAddress | Server | 192.168.2.11 | GameAppation.cpp | `LoadConfig` | 登录服地址（可被命令行覆盖） | 是（强推荐） |
| 27 | loginPort | Server | 7000 | GameAppation.cpp | `LoadConfig` | 登录服端口（可被命令行覆盖） | 是（强推荐） |
| 28 | zone | Server | 1 | GameAppation.cpp | `LoadConfig` | 区号（可被命令行覆盖） | 是（强推荐） |
| 29 | Player | other | 空字符串 | GameAppation.cpp | `LoadConfig` | 默认角色/玩家名显示 | 可选 |
| 30 | autogame | other | false | GameAppation.cpp | `LoadConfig` | 自动玩法开关（AUTOPLAY） | 否 |
| 31 | version | other | ZTGAME_VERSION | NetAutoConnect.cpp | `GetGameVersion` | 自动模式下的版本号校验来源 | 是（AUTOPLAY场景） |

## 4. config.ini 写入字段总表

| 序号 | 字段名 | section | 写入值来源 | 源码文件 | 函数 | 触发时机 | 风险 |
|---|---|---|---|---|---|---|---|
| 1 | ScreenWidth | video | `m_Config.screenWidth` | GameAppation.cpp | `SaveConfig` | 登录UI触发保存（GuiLogin） | 低 |
| 2 | ScreenHeight | video | `m_Config.screenHeight` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 3 | Window | video | `m_Config.bWindow` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 4 | ColorBits | video | `m_Config.colorBits` | GameAppation.cpp | `SaveConfig` | 同上 | 中 |
| 5 | VertSync | video | `m_Config.bVertSync` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 6 | LightMap | video | `m_Config.bLightMap` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 7 | Weather | video | `m_Config.bWeather` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 8 | MultiTex | video | `m_Config.bMultiTex` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 9 | RenderTarget | video | `m_Config.bRenderTarget` | GameAppation.cpp | `SaveConfig` | 同上 | 中 |
| 10 | HardwareCursor | video | `m_Config.bHardwareCursor` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 11 | UseDXT | video | `m_Config.bUseDXT` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 12 | sound | audio | `m_Config.bMainSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 13 | scenesound | audio | `m_Config.bSceneSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 14 | circumstancesound | audio | `m_Config.bCircumstanceSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 15 | backgroundsound | audio | `m_Config.bBackgroundSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 16 | soundvolume | audio | `m_Config.nMainSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 17 | scenevolume | audio | `m_Config.nSceneSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 18 | circumstancevolume | audio | `m_Config.nCircumstanceSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 19 | backvolume | audio | `m_Config.nBackgroundSound` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 20 | Player | other | `m_Config.szPlayerName` | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 21 | autoreconnect | Server | 客户端当前自动重连设置 | GameAppation.cpp | `SaveConfig` | 同上 | 低 |
| 22 | version | other | `g_GameVersion` | NetAutoConnect.cpp | `SaveGameVersion` | AUTOPLAY重连前 | 中（版本验证相关） |

## 5. 登录器应写入字段建议
建议 C# 登录器稳定写入：
- `Server/servername`
- `Server/loginAddress`
- `Server/loginPort`
- `Server/zone`
- `other/version`（仅 AUTOPLAY 或版本联动场景）

## 6. 登录器不应随意写入字段
- `video/*`、`audio/*`：属于玩家本地偏好，登录器覆盖会破坏用户设置。
- `other/single`、`other/autogame`：调试/自动模式开关，错误写入可能造成异常流程。
- `other/InitMap/InitX/InitY`：错误写入可造成出生点异常或调试态偏移。

## 7. 命令行参数覆盖规则
源码存在明确覆盖逻辑：`if (__argc == 5)`，则覆盖 `loginAddress/loginPort/zone`。
- 参数顺序（按索引）：
  - `__targv[2]` -> loginAddress
  - `__targv[3]` -> loginPort
  - `__targv[4]` -> zone
- 因此优先级可判定为：**命令行覆盖 > config.ini 默认读取**。

## 8. 客户端连接 IP / Port 的最终来源
结论是**多来源分阶段**：
1. 启动登录阶段：读取 `config.ini`（或被 `__argc==5` 覆盖）得到登录服地址端口。  
2. 登录成功后进游戏阶段：`NetAutoConnect::LoginGameServer` 使用服务端回包 `stServerReturnLoginSuccessCmd` 中的 `pstrIP/wdPort` 调用 `GetClient()->Connect` 重连。  
3. 因此最终“进入场景服”的 IP/Port 并非 config.ini 固定值，而是**登录成功包下发优先**。

## 9. 推荐 config.ini 模板

```ini
[Server]
servername=测试一区
loginAddress=127.0.0.1
loginPort=7000
zone=1
autoreconnect=1

[other]
version=1
Player=

[video]
ScreenWidth=1024
ScreenHeight=768
Window=0
ColorBits=32
VertSync=0
LightMap=0
Weather=0
MultiTex=1
HardwareCursor=1
UseDXT=0

[audio]
sound=1
scenesound=1
circumstancesound=1
backgroundsound=1
soundvolume=50
scenevolume=50
circumstancevolume=50
backvolume=50
```

## 10. 风险结论
- `loginAddress/loginPort` 错误：无法连接登录服。  
- `zone` 错误：可能登录后区服逻辑错位（角色列表/分区不匹配）。  
- `version` 错误（AUTOPLAY/版本校验链路）：可能触发版本验证失败。  
- `autoreconnect` 错配：重连行为不符合预期。  
- 视频/音频字段乱写：通常不影响登录链路，但可能导致黑屏、性能或体验问题。

## 11. 下一步动态验证建议
1. 启动参数验证：构造 `__argc==5` 场景，确认地址端口区号覆盖是否生效。  
2. 抓包验证：区分“登录服连接”和“登录成功后二跳连接”目标地址是否一致/变化。  
3. 版本验证：改动 `other/version`，观察 `stUserVerifyVerCmd.version` 上送值。  
4. 重连验证：切断网络后检查 `autoreconnect` 对重连状态机影响。  
5. 登录器联调：C# 登录器仅改推荐字段，回归登录->选角->进场景完整流程。

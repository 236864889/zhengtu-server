# ztgame.dat/Client 目录源码深度分析（入口、登录、选服、网络、UI）

## 1) 入口与核心对象装配
- `CGameAppation::InitInstance()` 中创建全局对象：`g_pGameScene = new CGameScene; g_pClient = new CClient;`，随后加载配置、初始化声音、加载 `.gl/.pak` 资源、初始化 GUI。 
- `ShowWindow/UpdateWindow` 后开始进入正常运行态。

## 2) 登录与选服流程（可确认部分）
- 在自动重连模块 `NetAutoConnect.cpp` 可见：
  - 读取 `config.ini`。
  - 通过 `GetClient()->Connect(cmd->pstrIP, cmd->wdPort)` 发起连接。
  - 备份/重发 `stLoginSelectUserCmd`（`BackUpLoginSelectCmd`），说明“选服/选角登录指令”可被缓存并在断线后重发。
- `GuiSelectCharDialog.cpp` 中出现 `stLoginSelectUserCmd` 组包，侧面印证“角色选择 -> 登录请求”链路存在于 UI 层。

## 3) config.ini / 服务器配置链路
- 已确认 `NetAutoConnect.cpp` 直接 `ini.Open("config.ini")` 并 `ini.Save("config.ini")`，说明客户端配置持久化落盘在本地 ini。
- `InitInstance()` 调用 `LoadConfig()`，其后将配置应用到声音/图形等运行参数（例如音量开关），说明配置先于业务登录流程加载。

## 4) 客户端与 GatewayServer 连接点
- 客户端侧显式连接点是 `CClient::Connect(address, port)`，在自动重连与登录上下文被调用。 
- 协议结构引用 `base/Command.h` 中 `stUserRequestLoginCmd`、`stLoginSelectUserCmd` 等命令体，通常对应网关/登录服接入第一跳。
- 虽未在 Client 目录直接写死 “GatewayServer” 字符串，但从命令模型与连接时序可推断该连接就是网关入口（需与服务端 GatewayServer 模块联动核对端口映射）。

## 5) 协议收发入口
- `Client.h` 定义发送宏：`Client_SendCommand(...)`，统一打时间戳并调用 `GetClient()->Send(...)`。
- `Client.cpp` 中：
  - `Client_MsgThread` 从 socket 接收队列取包；
  - 按 8 字节块解密（登录阶段 RC5，后续阶段 DES）；
  - 根据包头标记处理压缩（zlib `uncompress`）；
  - 解包后进入消息队列供上层逻辑消费。
- 说明收发入口是“`Send`/`Client_SendCommand` + `Client_MsgThread` 解包线程”的双向模型。

## 6) UI 与游戏主循环关系
- `MsgProc` 先处理设备输入，再进入光标/声音/GUI 分发。
- 大量 `OnGuiEvent`（各 `Gui*.cpp`）负责界面行为并触发协议发送（例如聊天、登录选择、功能面板操作）。
- 即：**UI 事件驱动命令发送，主循环持续消费网络回包并更新场景对象（World/Scene/Role/Npc/Item/Skill/Chat 等）**。

## 7) 关键词覆盖映射（Client/Login/Game/World/...）
- 在 `Client` 目录中可直接看到：
  - 场景/世界：`GameScene`, `World` 相关 GUI/对象逻辑；
  - 角色/玩家：`MainCharacter`, `RoleItem`, `Npc`, `Monster` 相关行为；
  - 技能/法术：`Skill`, `Magic` 命令及表现；
  - 聊天：`Chat.cpp` 中构包并 `Client_SendCommand`；
  - 网络：`Client.{h,cpp}` + `TCPSocket`；
  - 协议：`st*Cmd` 体系来自 `base/Command.h`。

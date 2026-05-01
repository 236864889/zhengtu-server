# SessionServer 网络连接与服务通信

## 连接模型
- 入站：`SessionService::newTCPTask` -> `SessionTask`。
- 出站：`RecordClient` 主动连接 `RecordServer`。
- 管理：`SessionTaskManager` 维护 scene/gateway 容器，支持广播与按服转发。

## 服务通信
- Gateway：`CMD_GATE -> msgParse_Gate`。
- Scene：`CMD_SCENE -> msgParse_Scene`。
- Forward：`CMD_FORWARD -> msgParse_Forward`。
- Super：`SessionService::msgParse_SuperService`（含 GMTool、Scene 控制类命令）。
- Record：`RecordClient::cmdMsgParse`（目前见 `PARA_CHK_USER_EXIST` 路径）。

## 断线处理
- `RecordClient::run` 在与 Record 断链后触发 `SessionService::Terminate()`，并等待服务顺序退出，属于“关键依赖硬失败即停服”。

## 风险
- 单点强依赖 Record；抖动会放大为全服中断。
- Scene/Gateway/Forward 三通道分发逻辑集中在超大函数，回归风险高。

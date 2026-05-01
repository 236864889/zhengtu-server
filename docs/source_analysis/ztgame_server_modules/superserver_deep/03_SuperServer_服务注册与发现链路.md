# SuperServer 服务注册与服务发现链路

## 注册握手
- 子服务接入后进入 `ServerTask::verifyConn`，解析 `Cmd::Super::t_Startup_Request`。
- Super 按 `TYPE + IP` 查 `SERVERLIST`，返回 `t_Startup_Response(wdServerID/wdPort/extIP/extPort)`。
- 服务进入 `waitSync` 阶段：
  - `processSequence` 根据 `serverSequence[wdServerType]` 组依赖服务集合；
  - `notifyOther` 通知依赖方（`PARA_STARTUP_SERVERENTRY_NOTIFYOTHER`）；
  - 收齐后 `notifyMe` 回送完整 `ServerEntry[]`。
- 客户端回 `PARA_STARTUP_OK` 后同步完成。

## 注册表结构
- Super 侧：
  - `ServerManager::container`：在线连接列表。
  - `ServerManager::taskUniqueContainer`：`wdServerID -> ServerTask` 唯一表。
- 子服务侧（zSubNetService）：`serverList` deque，支持 `getServerEntryByType/getNextServerEntryByType`。

## 服务类型/编号/IP端口
- 协议结构：`Cmd::Super::ServerEntry` 包含 `wdServerID/wdServerType/pstrIP/wdPort/pstrExtIP/wdExtPort/state`。
- 数据来源：`SERVERLIST` 表（Super 自身和各业务服均依赖此映射）。

## 上下线/状态同步
- 上线：`ServerTask::addToContainer + uniqueAdd` 后可参与广播。
- 下线：`removeFromContainer`，若是 Gateway，主动向 FL 广播“维护状态/在线数0”。
- 重同步：支持 `PARA_RESTART_SERVERENTRY_NOTIFYOTHER` 触发对端重通知。

## 适合解决的问题
- `getServerEntryByType` 返回 NULL 的根因分析。
- 某服务重启后其他服看不到它（启动顺序通知链断点）。

## 待确认
- `serverSequence` 每种服务类型的精确定义清单（继续查 `ServerTask.cpp` 静态初始化段）。

# SuperServer 故障转移与状态清理

## 服务断线
- 连接断开触发 `ServerTask::removeFromContainer`，并从 `ServerManager` 移除。
- 若断的是 Gateway，立即向 FL 广播状态维护+在线数0，避免继续导流。

## 重连
- 重连会再次走 `verifyConn -> waitSync`，并通过 `uniqueAdd` 防止同 `wdServerID` 重复在线。
- 支持 `PARA_RESTART_SERVERENTRY_NOTIFYOTHER` 触发重新通知依赖服务。

## 状态清理
- 唯一表清理：`uniqueRemove`。
- 角色名链路失败缓存：`RoleregCache` 周期重放（10秒节拍）。
- TimeTick 持久化 `GAMETIME`（10分钟保存 + 退出保存）。

## 降级/容错现状
- 存在“部分降级”：Rolereg 下游不可用时先缓存待发。
- 不存在“服务发现替代源”：Super/DB 不可用会造成整体启动/发现阻断。

## 高风险点
- `ServerManager::broadcast*` 在持锁状态下发送，慢连接可能阻塞全局表操作。
- `RoleregCache` 无硬上限，极端情况下可能积压内存。

## 待确认
- 是否有外部 watchdog 做 Super 进程级故障切换（代码内未见主备选主逻辑）。

# GatewayServer 与 Session 通信链路

## Gateway -> Session（CMD_GATE）
- 登录/角色链：`GateUser::reg/unreg` 与 `GatewayTask` 登录选角流程向 Session 发送注册、反注册、删角等命令。
- 业务转发链：`GatewayTask::forwardSession()` 将用户侧包封装后送 `sessionClient->sendCmd()`。

## Session -> Gateway 回包路径
- `SessionClient::msgParse()`：
  - `CMD_GATE/PARA_GATE_UNREGUSER`：驱动 GateUser 返回选角或终止。
  - `CMD_GATE/PARA_GATE_DELCHAR`：删角结果转 Record 或回客户端。
  - `CMD_GATE/PARA_GATE_COUNTRY_ORDER`：同步国家排序。
  - `CMD_FORWARD`：用户定向、国家、全服广播下发到在线玩家。

## 登录态/踢人/断线/重连
- 踢人/反注册由 Session 回包决定最终动作；Gateway 本地根据 `retcode` 选择 `final+beginSelect` 或 `Terminate`。
- Session 连接断开时 `SessionClient::run()` 会触发 `GatewayService::Terminate()`，防止网关失去核心会话后继续服务。

## sendCmd/cmdMsgParse
- 发送：`SessionClient::sendCmd`（zTCPBufferClient 基类）。
- 解析：`SessionClient::msgParse`（Gateway 侧）与 `SessionTask::msgParse_Gate`（Session 侧，来自上一轮结论）。

## 风险
- `CMD_GATE`/`CMD_FORWARD` 分支过重，新增子命令易破坏兼容。
- Session 断链即全网关终止，需评估高可用策略。

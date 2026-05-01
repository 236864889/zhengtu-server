# GatewayServer 源码深度分析

## 1. 入口与生命周期
- `GatewayServer.cpp`：`int main(...)` 入口。
- `GatewayService::init/final/validate`：生命周期核心；`validate` 覆写并调用 `zSubNetService::validate()`。

## 2. 模块构成（已确认）
- 接入与用户：`GateUser.cpp/h`、`GateUserManager.cpp/h`
- 任务处理：`GatewayTask.cpp/h`、`GatewayTaskManager.cpp/h`
- 下游连接：
  - `SceneClient.cpp/h`、`SceneClientManager.cpp/h`
  - `SessionClient.cpp/h`
  - `RecordClient.cpp/h`
  - `BillClient.cpp/h`
- 配置：`zoneInfo.xml`、`billServerList.xml`、`loginServerList.xml`（目录级线索）

## 3. 关键调用链（抽象）
1. 客户端连接进入网关（GateUser）。
2. `GatewayTask` 收包，执行 `verifyLogin/verifyConn`。
3. 按命令分发到会话/场景/记录/计费下游。
4. 返回包回写到 GateUser。

## 4. 协议与命令
- 命令头 include 以 `Command.h` + 业务命令头为主。
- `Cmd::` 命名空间分发，结构体通常从 `stNullUserCmd` 派生。

## 5. 网络稳定性关键点
- `validate()`：连接巡检、异常连接清理。
- 下游客户端管理器：重连/切换策略是稳定性重点。

## 6. 二开风险点（高）
1. `GatewayTask.cpp` 命令路由分支修改。
2. `GateUserManager` 连接管理策略修改。
3. 下游 client failover 逻辑修改。

## 7. 建议补充证据
- 每个下游命令字转发表（cmd -> target service）。
- 登录态在网关缓存与过期策略。
- reconnect/backoff 具体参数与阈值。

## 8. RAG 标签
- `gateway` `access_layer` `task_dispatch` `client_manager` `validate` `high_risk`

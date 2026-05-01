# SessionServer 源码深度分析

## 1) 入口与生命周期
- `SessionServer.cpp` 含 `main`。
- `SessionService::init/final` 为生命周期主流程。
- `SessionConfile::parse("SessionServer")` 为配置入口。

## 2) 模块构成（高密度）
- 任务分发：
  - `SessionTask.cpp/h`、`SessionTaskManager.cpp/h`
- 用户与会话：
  - `SessionUser.cpp/h`、`SessionUserManager.cpp/h`
- 子系统（示例）：
  - `CUnion`、`CRelation`、`CCountry`、`CSept`、`CGem`、`CVote`、`CQuiz`、`CDare` 等
- 外部连接：
  - `GatewayClient`、`SceneClient`、`RecordClient`、`BillClient` 等

## 3) 命令分发事实
- `SessionTask.cpp` 对 `stNullUserCmd` 做高密度路由。
- 大量 include 与分支体现其“会话总线”角色。
- 改动一处命令处理，可能影响多个业务子系统。

## 4) 配置与 DB
- 使用 `zConfile` + `zXMLParser` 加载配置。
- 业务层可见 `SessionService::dbConnPool->exeSelectLimit(...)` 查询路径（如 `CUnion.cpp`、`CSept.cpp`）。
- 说明 Session 同时承担会话路由与部分数据访问职责。

## 5) 风险点
1. `SessionTask.cpp` 回归成本高（分支多、耦合广）。
2. 子系统 manager 之间的调用顺序依赖隐蔽。
3. 会话态与数据库状态一致性需重点关注。

## 6) 二开建议
- 先做“命令字 -> 子系统 -> 数据表”映射再改代码。
- 对每次改动建立最小回归集（登录、建角、社交、组织、商城、跨服）。
- 加 trace id 贯穿网关-会话-场景链路。

## 7) 待确认
- 命令字冲突检测机制是否存在自动化校验。
- 会话超时与踢线策略的统一配置来源。

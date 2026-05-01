# SuperServer 源码分析（v0.2）

## 入口与生命周期
- `SuperServer.cpp` 含 `main`。
- `SuperService::init/final` 生命周期。
- `SuperConfile::parse("SuperServer")` 配置入口。

## 关键初始化事实
- `SuperService::init` 明确初始化：
  - `FLClientManager`
  - `RoleregClientManager`
  - `InfoClientManager`
- 并调用 `zNetService::init(wdPort)` 完成网络启动。

## 任务与协议
- `ServerTask.cpp` 是核心分发点。
- include 多协议头，承担跨服务命令汇聚。

## 风险点
- 跨协议改动容易出现兼容性连锁问题。
- 上下游服务状态不一致时，Super 侧故障放大明显。

## 待确认
- 各协议失败重试与降级策略是否统一。

# FLServer 源码分析（v0.2）

## 入口与生命周期
- `FLServer.cpp` 含 `main`。
- `FLService::init/final` 负责生命周期。

## 关键初始化事实
- `FLService::init` 依赖 `DBAccessClientManager::init()`、`InfoClientManager::init()`。
- 表明 FL 是登录前置协调层，依赖外部信息/数据库访问服务。

## 任务与协议
- `LoginTask.cpp`、`ServerTask.cpp`、`PingTask.cpp`、`FLTaskManager.cpp` 组成任务链。
- 命令分发与连接校验范式同样存在。

## 风险点
- 登录链路变更影响面广（网关、会话、账号体系）。
- 与 DBAccess 交互超时会放大为登录失败。

## 待确认
- 登录失败码与客户端提示码映射表。

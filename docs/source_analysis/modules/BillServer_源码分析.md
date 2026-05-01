# BillServer 源码分析（v0.2）

## 入口与生命周期
- `BillServer.cpp` 存在 `main`。
- `BillService::init/final` 为生命周期核心。
- 存在 `BillService::reloadConfig()`，支持配置重读。

## 任务与协议
- 任务侧：`BillTask.cpp` + `BillTaskManager.cpp`。
- 协议侧：`BillCommand.h`（通过 Task include 进入）。
- 模式：`verifyLogin/verifyConn` + 命令分发。

## 依赖关系
- 客户端关系：`BillClient`、`SessionClient`、`SuperClient`。
- 说明：计费服务并非孤立，需与会话/超级服务协作。

## 风险点
- 计费相关命令改动需核对幂等与重放。
- reload 配置的线程安全与生效时机需谨慎。

## 待确认
- 与数据库表（账务流水）字段映射的完整证据。

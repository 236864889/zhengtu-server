# RecordServer 源码分析（v0.2）

## 入口与生命周期
- `RecordServer.cpp` 含 `main`。
- `RecordService::init/final` 生命周期。

## 任务与职责
- `RecordTask.cpp/h` + `RecordTaskManager.cpp/h`
- 负责记录/落地相关请求处理（日志、统计或业务记录）。

## 依赖关系
- 与其他服务通过 client/task 协作，接收需要持久化/记录的事件。

## 风险点
- 落地失败重试策略不当可能导致数据重复或丢失。
- 高峰期写入背压可能反向影响上游。

## 待确认
- 记录库表结构与幂等键策略。

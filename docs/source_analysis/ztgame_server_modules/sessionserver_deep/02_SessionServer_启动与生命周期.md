# SessionServer 启动与生命周期

## main
- `main` 创建并运行 `SessionService` 单例，生命周期沿用 base：`main -> init -> loop -> final`。

## 继承关系
- `SessionService : zSubNetService`，间接沿用 `zNetService/zService` 框架。
- `newTCPTask` 创建 `SessionTask` 并投递到 `zTCPTaskPool` 验证队列。

## init 关键链路
1. 初始化 DB 连接池 `zDBConnPool` 与 `MetaData`。
2. 初始化线程池 `zTCPTaskPool`。
3. 调 `zSubNetService::init()` 完成子服务层初始化/注册。
4. 通过 Super 发现 `RECORDSERVER`，创建并连接 `RecordClient`。
5. 依次初始化 User/Scene/国家家族等业务管理器。
6. 启动 `SessionTimeTick`。

## final
- 停止 TimeTick。
- 回收 taskPool、recordClient。
- 清理各 manager 单例。
- 调 `zSubNetService::final()`。

## serviceCallback / validate
- 本目录未见 `SessionService` 显式覆写 `validate/serviceCallback`，推断走基类行为（待在 base 继续确认）。

## 启动失败点
- mysql URL/MetaData 初始化失败。
- threadPool 初始化失败。
- `zSubNetService::init` 失败。
- 找不到 RECORDSERVER 或 RecordClient 连接失败。
- 任一业务 manager init 返回 false。

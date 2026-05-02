# 线程与线程池样例（静态）

## 覆盖文件
- `ztgame/test/ThreadTest.cpp`
- `ztgame/test/ThreadPoolTest.cpp`

## ThreadTest.cpp
- 自定义 `Thread : zThread`，`run()` 中 sleep 与日志输出。
- `main()` 持续创建线程并尝试 `start()`，属于极简线程启动样例。
- 价值：展示 `zThread` 子类化与 `start/final/join` 基础语义。

## ThreadPoolTest.cpp
- 定义 `TCPTaskTest : zTCPTask` 作为任务实体。
- `ThreadPoolService : zService` 在 `init()` 中创建 `zTCPTaskPool`、初始化 `zTCPServer`。
- `serviceCallback()` 中 `accept` 新连接并将任务投递到池中。
- 读取 `Zebra::global["initThreadPoolState"]`、`threadPoolCapacity` 做池策略。

## 调度/同步/回收特征
- 以连接驱动任务投递，线程池承载任务生命周期。
- 在 `final()` 中显式停止/释放 pool 与 server，体现历史代码的手工回收模式。

## 风险
- 依赖真实监听 socket，运行会进入阻塞 accept 循环。
- 若容量/状态配置不当，可能出现线程膨胀或资源占用异常。
- 不含现代观测指标与熔断保护，不建议直接用于生产压测。

## 可作为 Claw 模板的部分
- `ThreadPoolService` 初始化顺序可直接改造成“只建对象不 listen”的 dry-run 模板。
- `TCPTaskTest` 可替换为无副作用假任务进行逻辑回归。

## RAG 标签 & 适合问题
- 标签：`thread` `threadpool` `ztcp-taskpool` `service-loop`。
- 适合问题：
  - “zThread 与 zService 的组合范式是什么？”
  - “旧版线程池服务如何从 accept 分发到 task？”

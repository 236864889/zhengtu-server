# BillServer 启动与生命周期
- `main` 在 `BillServer.cpp`，argp 选项含 `mysql/ifname/server/port/log`。
- `BillService::init` 关键步骤：
  1) `zDBConnPool::newInstance + putURL`
  2) `MetaData::init(mysql)`
  3) 交易日志 `tradelog`
  4) `zTCPTaskPool` 初始化
  5) `Bill_init`（外部统一计费客户端系统）
  6) `Consign*Manager::init`
  7) `BillTimeTick::start`
  8) `zSubNetService::init`
- `newTCPTask`：每新连接构造 `BillTask` 并进 `taskPool->addVerify`。
- `validate/serviceCallback`：由基类 `zSubNetService/zService` 继承执行。
- `final`：停止 Tick、销毁 taskPool/manager、`Bill_final`、释放 DB pool。

启动失败点：DB URL 错误、MetaData 初始化失败、线程池失败、`Bill_init` 失败、基类 init 失败。

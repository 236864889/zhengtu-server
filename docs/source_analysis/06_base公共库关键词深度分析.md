# base 公共库关键词深度分析

## 分析目标
围绕以下关键词提取 `ztgame/base` 证据：
`zSocket, zTCPServer, zTCPClient, zTCPTask, zTCPTaskPool, zTCPClientTask, zTCPClientTaskPool, zThread, zThreadGroup, zService, zNetService, zMNetService, zSubNetService, zLogger, zDatabase, zDatabaseManager, zMysqlDBConnPool, zXMLParser, zProperties, zConfile, zMisc, zArg, zTime, zTimer, EncDec, libzebra.a, libencdec.a, libcrypto.a`。

## 1) 产物与构建关系
- `ztgame/base/Makefile` 的 `OBJS` 明确包含上述多数核心对象：
  - 网络链：`zSocket.o/zTCPClient.o/zTCPClientTask.o/zTCPClientTaskPool.o/zTCPServer.o/zTCPTask.o/zTCPTaskPool.o`
  - 服务链：`zService.o/zNetService.o/zMNetService.o/zSubNetService.o`
  - 基础设施：`zLogger.o/zTimer.o/zTime.o/zArg.o/zProperties.o/zConfile.o/zXMLParser.o/zDatabaseManager.o/zDatabase.o/zMysqlDBConnPool.o`
- `LIB_ZEBRA = libzebra.a`，说明 `base` 目录静态库产物为 `libzebra.a`。
- 根 `ztgame/Makefile` 将 `base/EncDec` 放在 `base` 前编译，确立 `libencdec.a` 的先置关系。
- `SuperServer/Makefile` 声明 `LIB_CRYPTO = ../base/libcrypto.a`，说明上层对 `libcrypto.a` 有显式链接依赖。

## 2) 网络与任务并发主干
- `zSubNetService.h` 直接 include `zService/zNetService/zThread/zSocket/zTCPServer/zTCPClient/zMisc`，并定义 `class zSubNetService : public zNetService`。
- `zTCPClientTaskPool.h` 展示客户端任务池模型：`zThreadGroup taskThreads`、`tasks` 容器、`addMain/put/timeAction` 等接口。
- `zSocket.cpp` 提供 IO 主实现：`recvToCmd/sendCmd/sync/checkIOForRead/checkIOForWrite/setNonblock/waitForRead/waitForWrite` 等。

## 3) 服务基类分层
- 关键词链可归纳为：
  - `zService`：服务生命周期抽象。
  - `zNetService`：网络服务抽象。
  - `zMNetService`：多网络服务变体。
  - `zSubNetService`：子网/跨服场景封装，供业务服继承。
- 该层次在多个服务源码中被复用（如 Bill/FL/Gateway/Scenes/Session/Record）。

## 4) 日志、配置、数据库三条基础链
- 日志：`zLogger.cpp` 定义了 `fatal/error/warn/info/debug/trace` 等完整接口。
- 配置：`zXMLParser` + `zConfile` + `zProperties` 构成 XML/配置读取基础能力。
- 数据库：`zMysqlDBConnPool` + `zDatabase` + `zDatabaseManager` 构成连接池与数据访问主链。

## 5) 时间与参数
- 时间：`zTime`、`zTimer`（含 `zTimerThread/zTimerTask`）承担定时任务调度。
- 参数：`zArg` 被各服务 main 流程用于命令行参数解析。

## 6) EncDec 与加密链
- `zType.h` include `EncDec/EncDec.h`，且根 Makefile 把 `base/EncDec` 置于前置编译。
- `libencdec.a` 由 EncDec 子模块提供，业务服务在 `CXXLFLAGS` 中普遍链接 `-lencdec`。
- `libcrypto.a` 在 `SuperServer` 侧有显式链接声明；其具体产物路径与构建细节仍需对 EncDec 子目录进一步核验（待确认）。

## 7) 风险与二开建议
- 高风险改动点：`zSocket.*`、`zTCPTaskPool.*`、`zSubNetService.*`、`zLogger.*`、`zMysqlDBConnPool.*`。
- 建议二开顺序：
  1. 先做关键词到调用点映射；
  2. 再做单模块编译验证（base/test）；
  3. 最后做跨服务启动回归。

## 8) 待确认
- `libcrypto.a` 的实际生成链是否完全在 `base/EncDec`（需结合该子目录构建脚本逐行确认）。
- `zMNetService` 在当前业务服中的实际使用覆盖率（需继续扫 `new`/继承关系）。

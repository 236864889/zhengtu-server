# base 源码深度分析

## 1. 模块定位
`ztgame/base` 是服务端共享基础库，负责编译产物、网络服务框架、线程/任务池、配置解析、数据库连接池、日志与基础工具。多数业务服务通过 `../base/libzebra.a` 复用该层能力。

## 2. 构建与产物
- `base/Makefile` 产出核心静态库 `libzebra.a`。
- `base/EncDec` 在根 `Makefile` 中位于 `base` 前编译，提供 `libencdec.a`。
- 上层服务 Makefile 中可见 `-lzebra`、`-lencdec`、`-llog4cxx`、`mysql_config`、`xml2-config` 等链接依赖。
- `libcrypto.a` 的实际生成链仍需结合 `base/EncDec` 与相关 Makefile 继续核验。

## 3. 网络与服务框架
- `zService`：服务生命周期抽象。
- `zNetService`：网络服务抽象，负责端口初始化与主循环能力。
- `zSubNetService`：继承 `zNetService`，为多业务服提供子网/跨服连接管理基础。
- `zMNetService`：多网络服务变体，实际启用范围待确认。
- `zSocket`：基础 socket 封装，包含 `recvToCmd`、`sendCmd`、`checkIOForRead`、`checkIOForWrite` 等关键读写能力。

## 4. 线程与任务池
- `zThread` / `zThreadGroup`：线程基础封装。
- `zTCPTask`：连接级任务对象。
- `zTCPTaskPool`：任务池与并发处理入口。
- `zTCPClientTaskPool`：客户端任务池模型，包含 `taskThreads`、`tasks`、`addMain`、`put`、`timeAction` 等接口。

## 5. 配置、数据库与日志
- 配置链：`zConfile`、`zProperties`、`zXMLParser`。
- 数据库链：`zDatabaseManager`、`zDatabase`、`zMysqlDBConnPool`。
- 日志链：`zLogger`，提供 `fatal/error/warn/info/debug/trace` 等日志接口。

## 6. 二开高风险点
1. `zSocket.*`：包读写、非阻塞、IO 等待逻辑，影响所有网络服务。
2. `zTCPTaskPool.*`：任务调度和并发策略，影响吞吐与延迟。
3. `zSubNetService.*`：服务校验与连接管理语义，改动可能引发误踢或连接泄漏。
4. `zMysqlDBConnPool.*`：连接池异常恢复和并发上限，影响全服 DB 稳定性。
5. `zLogger.*`：日志落地与格式，影响故障定位。

## 7. 建议补充证据
- `zNetService::init` 的返回码、端口绑定失败处理。
- `zSubNetService::validate` 的完整失败路径图。
- 各服务任务池线程数、队列阈值、背压策略。
- DB 连接池断线/超时后的自动恢复策略。

## 8. RAG 标签
- `base` `network` `service_lifecycle` `taskpool` `db_pool` `config` `logger` `high_risk`

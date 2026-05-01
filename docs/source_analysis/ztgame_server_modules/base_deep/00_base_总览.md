# ztgame/base 深度总览

## 1. 分析范围
- 源码目录：`ztgame/base/`、`ztgame/base/EncDec/`
- 重点文件：Makefile、网络/服务生命周期/日志/数据库/XML/协议/线程时间工具
- 本文不覆盖：`ScenesServer`、`SessionServer`、`GatewayServer`、`ztgame.dat`

## 2. 核心结论
- `base` 是服务端“公共运行时 + 通信 + 配置 + DB + 日志 + 协议头”基础层，产物核心为 `libzebra.a` 与 `libencdec.a`。证据：`base/Makefile` 与 `EncDec/Makefile` 直接输出静态库。 
- 生命周期骨架由 `zService::main -> init/validate -> serviceCallback loop -> final` 组成，网络服务在 `zNetService`/`zSubNetService` 扩展。 
- 网络链路由 `zSocket` 负责 pack/unpack + send/recv，`zTCPTask*`/`zTCPClientTask*` 负责连接生命周期与线程池并发。
- `Zebra::global` 和 `Zebra::logger` 是全局入口，缺配置时会落入默认值/初始化逻辑，风险较高。

## 3. 关键文件表（Top20）
| 文件 | 作用 | 关键类/函数 | 上游 | 下游 | 风险 |
|---|---|---|---|---|---|
| Makefile | base 构建 | OBJS/libzebra.a | 各服务 Makefile | libzebra.a | 高 |
| EncDec/Makefile | 加解密构建 | libencdec.a | base 链接参数 | libencdec.a | 中 |
| zService.cpp | 生命周期主循环 | main/init/final | 服务 main | zNetService 等 | 高 |
| zNetService.cpp | TCP 服务骨架 | init/serviceCallback | zService | zTCPServer/newTCPTask | 高 |
| zSubNetService.cpp | 子服务接入 Super | init/validate/sendCmdToSuperServer | zNetService | SuperClient | 高 |
| zSocket.cpp | socket I/O 封装 | sendCmd/recvToCmd | task/client | 协议收发 | 最高 |
| zTCPTaskPool.cpp | 服务端连接池 | init/final/recycle threads | zNetService | zTCPTask | 最高 |
| zTCPClientTaskPool.cpp | 客户端任务池 | init/checkconnect/checkwait | zSubNetService等 | zTCPClientTask | 高 |
| zTCPTask.cpp | 服务端连接任务 | sendCmd/listenSocket | taskpool | cmd parse | 高 |
| zTCPClientTask.cpp | 外连任务 | connect/sendCmd/listenSocket | clientpool | cmd parse | 高 |
| zTCPClient.cpp | 客户端连接 | connect/run | 上层服务 | zSocket | 中 |
| zLogger.cpp | 日志实现 | debug/info/error | 全模块 | 本地/控制台 | 高 |
| Zebra.cpp | 全局变量初始化 | initGlobal/finalGlobal | 程序启动 | global/logger | 高 |
| zMysqlDBConnPool.cpp | MySQL 连接池 | execSql/execSelectSql | zDatabase | DB 读写 | 最高 |
| zDatabaseManager.* | DB 管理器 | loadAllBM/unloadAllBM | 各服务数据层 | zDatabase | 高 |
| zXMLParser.cpp | XML 封装 | init/initFile/getNode* | Confile/IncludeXML | 配置加载 | 中 |
| zConfile.cpp | 服务配置解析 | parse/parseNormal | 服务 init | Zebra::global | 高 |
| zIncludeXML.cpp | include xml 展开 | expand | 配置初始化 | zXMLParser | 中 |
| Command.h | 全局协议头 | Cmd::t_NullCmd 等 | 所有服务 | 所有服务 | 最高 |
| zThread.cpp | 线程封装 | start/run/final | 线程池/计时器 | mysql_thread_init | 高 |

## 4. 最高风险文件（Top10）
`zSocket.cpp`, `zTCPTaskPool.cpp`, `zMysqlDBConnPool.cpp`, `Command.h`, `zService.cpp`, `zSubNetService.cpp`, `zLogger.cpp`, `zConfile.cpp`, `zThread.cpp`, `zTCPClientTaskPool.cpp`。

## 5. RAG 入库优先级
- S 级：02/03/05/07（网络、生命周期、数据库、协议）
- A 级：01/04/06/08
- B 级：10/11

## 6. 适合解决的问题
- 服务端 TCP 断连/粘包/回收线程排查
- 启动失败（配置/信号/初始化）定位
- MySQL 连接池抖动与事务失败定位
- 协议头变更对多服务兼容性评估

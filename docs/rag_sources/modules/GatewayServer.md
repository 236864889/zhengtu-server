---
doc_id: rag-module-gatewayserver
doc_type: module
project: zhengtu-server
module: GatewayServer
source_path: ztgame/GatewayServer
tags: ['GatewayServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：GatewayServer（v0.3）

## 1. 模块定位
- `GatewayServer` 在服务集群中承担独立职责，包含主进程入口、任务处理、连接管理与业务对象管理。

## 2. 目录职责
- 典型分层：`*Server` 入口、`*Task` 网络任务、`*Manager` 集合/状态管理、`*Client` 跨服务连接。

## 3. 核心文件表
| 文件 | 职责 |
|---|---|
| `GatewayServer.cpp` | 网关主进程入口，承接玩家入口连接。 |
| `GatewayTask.cpp` | 网关网络任务处理单元。 |
| `GatewayTaskManager.cpp` | 网关任务调度与管理。 |
| `GateUser.cpp` | 网关侧用户会话对象。 |
| `GateUserManager.cpp` | 网关用户连接集合管理。 |
| `SceneClient.cpp` | 连接 ScenesServer 的客户端通道。 |
| `SceneClientManager.cpp` | SceneClient 连接池管理。 |
| `SessionClient.cpp` | 连接 SessionServer 的客户端通道。 |
| `RecordClient.cpp` | 连接 RecordServer 的日志/记录通道。 |
| `BillClient.cpp` | 连接 BillServer 的计费通道。 |
| `LoginSessionManager.cpp` | 登录会话生命周期管理。 |
| `CmdChecker.cpp` | 协议命令检查/过滤。 |

## 4. 编译关系
- 由本目录 `Makefile` 产出对应目标（`tools` 为工具目标集合）。
- 普遍链接 `../base/libzebra.a` 与 `-lencdec`，具体附加库由目录 Makefile 决定。

## 5. 运行关系
- 通过 `*Client` 连接上游/下游服务，通过 `*Manager` 维护连接、用户或任务集合。

## 6. 配置依赖
- 依赖 `Config/*.xml`、根目录 XML 与任务脚本目录（精确读取路径待确认）。

## 7. 常见问题
- 上游未就绪导致 `*Client` 建连失败。
- `*Manager` 状态未同步导致会话/对象不一致。
- 配置版本与二进制不一致导致业务异常。

## 8. 二开入口
- 优先从 `GatewayServer.cpp`（或主入口文件）+ `*Task` + `*Manager` 切入，再扩展到 `*Client`。

## 9. 待确认事项
- 默认端口、重连退避策略、部分缩写类（如 GY）业务语义待确认。

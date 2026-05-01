---
doc_id: rag-module-flserver
doc_type: module
project: zhengtu-server
module: FLServer
source_path: ztgame/FLServer
tags: ['FLServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：FLServer（v0.3）

## 1. 模块定位
- `FLServer` 在服务集群中承担独立职责，包含主进程入口、任务处理、连接管理与业务对象管理。

## 2. 目录职责
- 典型分层：`*Server` 入口、`*Task` 网络任务、`*Manager` 集合/状态管理、`*Client` 跨服务连接。

## 3. 核心文件表
| 文件 | 职责 |
|---|---|
| `FLServer.cpp` | 服务入口，负责登录分发/服列表相关初始化。 |
| `ServerTask.cpp` | 服务端任务处理单元。 |
| `ServerManager.cpp` | 服务器列表与状态管理。 |
| `ServerACL.cpp` | 访问控制列表处理。 |
| `LoginTask.cpp` | 登录请求任务处理。 |
| `LoginManager.cpp` | 登录流程编排与状态管理。 |
| `GYListManager.cpp` | 国运/服列表类集合管理（缩写语义待确认）。 |
| `DBAccessClient.cpp` | 连接数据库访问服务的客户端封装。 |
| `DBAccessClientManager.cpp` | DBAccessClient 连接管理与重连。 |
| `InfoClient.cpp` | 连接信息服务的客户端。 |
| `InfoClientManager.cpp` | InfoClient 生命周期管理。 |
| `PingTask.cpp` | 探活/心跳任务。 |

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
- 优先从 `FLServer.cpp`（或主入口文件）+ `*Task` + `*Manager` 切入，再扩展到 `*Client`。

## 9. 待确认事项
- 默认端口、重连退避策略、部分缩写类（如 GY）业务语义待确认。

---
doc_id: rag-module-superserver
doc_type: module
project: zhengtu-server
module: SuperServer
source_path: ztgame/SuperServer
tags: ['SuperServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：SuperServer（v0.3）

## 1. 模块定位
- `SuperServer` 在服务集群中承担独立职责，包含主进程入口、任务处理、连接管理与业务对象管理。

## 2. 目录职责
- 典型分层：`*Server` 入口、`*Task` 网络任务、`*Manager` 集合/状态管理、`*Client` 跨服务连接。

## 3. 核心文件表
| 文件 | 职责 |
|---|---|
| `SuperServer.cpp` | 全局调度服务入口。 |
| `ServerTask.cpp` | 调度任务处理单元。 |
| `ServerManager.cpp` | 全局服务器状态管理。 |
| `FLClient.cpp` | 连接 FLServer 的客户端。 |
| `FLClientManager.cpp` | FLClient 管理与重连。 |
| `RoleregClient.cpp` | 连接角色注册服务客户端。 |
| `RoleregClientManager.cpp` | RoleregClient 管理。 |
| `RoleregCache.cpp` | 角色注册缓存层。 |
| `InfoClient.cpp` | 连接信息服务客户端。 |
| `InfoClientManager.cpp` | InfoClient 生命周期管理。 |
| `TimeTick.cpp` | 定时调度。 |
| `NetType.h` | 网络类型/枚举定义。 |

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
- 优先从 `SuperServer.cpp`（或主入口文件）+ `*Task` + `*Manager` 切入，再扩展到 `*Client`。

## 9. 待确认事项
- 默认端口、重连退避策略、部分缩写类（如 GY）业务语义待确认。

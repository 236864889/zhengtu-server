---
doc_id: rag-module-sessionserver
doc_type: module
project: zhengtu-server
module: SessionServer
source_path: ztgame/SessionServer
tags: ['SessionServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：SessionServer（v0.3）

## 1. 模块定位
- `SessionServer` 在服务集群中承担独立职责，包含主进程入口、任务处理、连接管理与业务对象管理。

## 2. 目录职责
- 典型分层：`*Server` 入口、`*Task` 网络任务、`*Manager` 集合/状态管理、`*Client` 跨服务连接。

## 3. 核心文件表
| 文件 | 职责 |
|---|---|
| `SessionServer.cpp` | 会话服务主入口。 |
| `SessionTask.cpp` | 会话网络任务处理。 |
| `SessionTaskManager.cpp` | 会话任务分发。 |
| `SessionManager.cpp` | 会话集合与状态管理。 |
| `Session.cpp` | 会话对象实现。 |
| `SessionChat.cpp` | 聊天相关会话逻辑。 |
| `MailService.cpp` | 邮件系统服务。 |
| `AuctionService.cpp` | 拍卖系统服务。 |
| `OfflineMessage.cpp` | 离线消息缓存/投递。 |
| `RecordClient.cpp` | 连接 RecordServer 写记录。 |
| `CCountryManager.cpp` | 国家系统数据管理。 |
| `CDareRecordManager.cpp` | 国战/挑战记录管理。 |

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
- 优先从 `SessionServer.cpp`（或主入口文件）+ `*Task` + `*Manager` 切入，再扩展到 `*Client`。

## 9. 待确认事项
- 默认端口、重连退避策略、部分缩写类（如 GY）业务语义待确认。

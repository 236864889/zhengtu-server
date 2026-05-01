---
doc_id: rag-module-recordserver
doc_type: module
project: zhengtu-server
module: RecordServer
source_path: ztgame/RecordServer
tags: ['RecordServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：RecordServer（v0.3）

## 1. 模块定位
- `RecordServer` 在服务集群中承担独立职责，包含主进程入口、任务处理、连接管理与业务对象管理。

## 2. 目录职责
- 典型分层：`*Server` 入口、`*Task` 网络任务、`*Manager` 集合/状态管理、`*Client` 跨服务连接。

## 3. 核心文件表
| 文件 | 职责 |
|---|---|
| `RecordServer.cpp` | 记录服务主入口。 |
| `RecordTask.cpp` | 记录服务网络任务处理。 |
| `RecordSessionManager.cpp` | 记录会话管理。 |
| `RecordEditor.cpp` | 记录编辑/格式化处理（细节待确认）。 |
| `RecordServer.h` | 主服务类声明。 |
| `RecordTask.h` | 任务类声明。 |
| `RecordSessionManager.h` | 会话管理声明。 |
| `RecordEditor.h` | 记录编辑声明。 |
| `Makefile` | 定义目标 `RecordServer` 与链接参数。 |
| `.RecordServer.d(历史构建产物)` | 依赖文件线索，仅构建期使用（若存在，待确认）。 |

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
- 优先从 `RecordServer.cpp`（或主入口文件）+ `*Task` + `*Manager` 切入，再扩展到 `*Client`。

## 9. 待确认事项
- 默认端口、重连退避策略、部分缩写类（如 GY）业务语义待确认。

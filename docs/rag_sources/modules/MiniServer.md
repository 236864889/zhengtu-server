---
doc_id: rag-module-miniserver
doc_type: module
project: zhengtu-server
module: MiniServer
source_path: ztgame/MiniServer
tags: ['MiniServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：MiniServer（v0.3）

## 1. 模块定位
- `MiniServer` 在服务集群中承担独立职责，包含主进程入口、任务处理、连接管理与业务对象管理。

## 2. 目录职责
- 典型分层：`*Server` 入口、`*Task` 网络任务、`*Manager` 集合/状态管理、`*Client` 跨服务连接。

## 3. 核心文件表
| 文件 | 职责 |
|---|---|
| `MiniServer.cpp` | 小游戏服务入口。 |
| `MiniTask.cpp` | 小游戏网络任务处理。 |
| `MiniTaskManager.cpp` | 小游戏任务调度。 |
| `MiniUser.cpp` | 小游戏用户状态对象。 |
| `MiniUserManager.cpp` | 小游戏用户集合管理。 |
| `MiniGame.cpp` | 小游戏逻辑主封装。 |
| `MiniHall.cpp` | 大厅逻辑管理。 |
| `MiniRoom.cpp` | 房间逻辑管理。 |
| `DDZCardGame.cpp` | 斗地主玩法核心逻辑。 |
| `CardPattern.cpp` | 牌型判定算法。 |
| `Top100.cpp` | 排行榜/Top100 处理。 |
| `TimeTick.cpp` | 定时器驱动逻辑。 |

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
- 优先从 `MiniServer.cpp`（或主入口文件）+ `*Task` + `*Manager` 切入，再扩展到 `*Client`。

## 9. 待确认事项
- 默认端口、重连退避策略、部分缩写类（如 GY）业务语义待确认。

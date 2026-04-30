---
doc_id: rag-module-scenesserver
doc_type: module
project: zhengtu-server
module: ScenesServer
source_path: ztgame/ScenesServer
tags: ['ScenesServer', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：ScenesServer（v0.2）

## 1. 模块定位
- `ScenesServer` 是场景服主进程，承载地图、对象、战斗、任务、交易、队伍、NPC、脚本联动等核心玩法逻辑。

## 2. 目录职责
- 场景内对象与用户管理：`SceneUser*`、`ObjectManager*`、`SceneManager*`。
- 任务与脚本：`Quest*`、`script*`。
- 跨服交互：`SessionClient`、`RecordClient`、`MiniClient`。

## 3. 核心文件表（>=10）
| 文件 | 线索 | 用途说明 |
|---|---|---|
| `ScenesServer.cpp` | 主文件 | 场景服主入口与初始化流程。 |
| `SceneTask.cpp` | Task模型 | 网络任务处理。 |
| `SceneTaskManager.cpp` | Manager模型 | 任务分发与管理。 |
| `SceneUser.cpp` | User模型 | 玩家场景态逻辑。 |
| `SceneUserManager.cpp` | Manager模型 | 场景用户集合管理。 |
| `SceneNpcManager.cpp` | Manager模型 | NPC 实体管理。 |
| `SceneNpcAI.cpp` | AI模型 | NPC AI 行为。 |
| `script.cpp` | 脚本入口 | Lua 脚本执行桥接。 |
| `script.h` | 接口定义 | 脚本系统对外接口。 |
| `script_func.h` | 函数导出 | 脚本可调用函数声明。 |
| `SessionClient.cpp` | 跨服务Client | 与 SessionServer 交互。 |
| `RecordClient.cpp` | 跨服务Client | 与 RecordServer 交互。 |
| `MiniClient.cpp` | 跨服务Client | 与 MiniServer 交互。 |
| `Quest.cpp` | 任务逻辑 | 任务主流程。 |
| `QuestCondition.cpp` | 条件系统 | 任务条件判定。 |

## 4. 编译关系
- `Makefile` 中对象列表最多，目标产物为 `ScenesServer`。
- 链接 `../base/libzebra.a`、`-lencdec`，并额外链接 `-L../script -llua -lluabind`。

## 5. 运行关系
- 上游/平级依赖线索：
  - `SessionServer`：通过 `SessionClient*` 同步跨场景状态。
  - `RecordServer`：通过 `RecordClient*` 写日志/记录。
  - `MiniServer`：通过 `MiniClient*` 处理小游戏联动。

## 6. 配置依赖
- `Config/*.xml`（玩法参数）
- 根目录 `scenesinfo.xml`、`skilltable.xml`、`npctrade.xml` 等
- 脚本目录 `quest/`、`newquest/`

## 7. 常见问题
- Lua 绑定风险：userdata 生命周期、panic 未保护、脚本异常传播。
- 配置读取风险：XML/Lua 版本不一致、字段缺失。
- 启动顺序风险：Session/Record/Mini 未就绪导致 client 连接失败。

## 8. 二开入口
- 强烈建议按顺序切入：
  1) `SceneUser*`（玩家态）
  2) `SceneTask*`（网络事件）
  3) `SceneNpcManager*` + `SceneNpcAI*`（NPC行为）
  4) `script*` + `Quest*`（脚本/任务）

## 9. 待确认事项
- 场景服启动后“标准日志关键字”与错误码清单待确认。
- 与 Session/Record 的协议版本字段待确认。

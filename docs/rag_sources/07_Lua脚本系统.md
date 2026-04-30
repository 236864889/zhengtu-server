---
doc_id: rag-overview-07
doc_type: overview
project: zhengtu-server
module: global
source_path: ztgame
tags: ['rag','overview']
confidence: medium
last_verified: 2026-04-30
---
# 07_Lua脚本系统

Lua版本、脚本目录和场景服集成点。

## 事实来源
- `ztgame/Makefile`：定义 `SUB_DIRS`、编译参数和发布复制目标。
- `ztgame/start.sh`、`ztgame/start1.sh`、`ztgame/stop.sh`：体现启动/停止顺序。
- 各子目录 `Makefile`：定义二进制目标与依赖库。
- `ztgame/script/lua.h`：Lua版本信息。
- `ztgame/base/*.h/*.cpp`：网络、数据库、日志基础组件。

## 待确认范围
- 端口号、具体数据库库名/表结构、生产部署拓扑在当前仓库中未直接固化，统一标记“待确认”。

## Lua脚本系统线索
- `script/lua.h` 标注 `Lua 5.1`。
- `ScenesServer` Makefile 链接 `-L../script -llua -lluabind`。
- 脚本数据目录：`quest/`、`newquest/`。

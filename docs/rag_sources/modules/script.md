---
doc_id: rag-module-script
doc_type: module
project: zhengtu-server
module: script
source_path: ztgame/script
tags: ['script', 'lua', 'rag']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：script（v0.2）

## 1. 模块定位
- `script` 提供 Lua 运行时与 C++ 绑定能力，是场景服脚本执行基础层。

## 2. 目录职责
- 提供 Lua 头与 luabind 头。
- 向业务模块提供 `liblua.a`、`libluabind.a`、`liblualib.a` 静态库。

## 3. 核心文件表
| 文件 | 依据 | 用途说明 |
|---|---|---|
| `lua.h` | 文件内容 | 明确版本为 Lua 5.1。 |
| `lauxlib.h` | Lua辅助库 | C API 辅助函数声明。 |
| `lualib.h` | Lua标准库 | 标准库导出接口。 |
| `luaconf.h` | Lua配置 | 编译配置项。 |
| `lua.hpp` | C++封装 | C++ 包装入口。 |
| `luabind/` | 目录结构 | C++ 与 Lua 绑定实现资源。 |
| `../ScenesServer/script.cpp` | 调用方线索 | 场景服脚本逻辑入口。 |
| `../ScenesServer/script.h` | 调用方线索 | 脚本封装接口。 |
| `../ScenesServer/script_func.h` | 调用方线索 | Lua 导出函数接口。 |
| `../ScenesServer/Makefile` | 链接参数 | `-L../script -llua -lluabind`。 |

## 4. 编译关系
- `script` 当前不是顶层 `SUB_DIRS` 独立 make 子模块。
- 但静态库作为依赖被 `ScenesServer` 等模块链接。

## 5. 运行关系
- `ScenesServer` 通过 `script.cpp` / `script_func.cpp` 触发 Lua 脚本执行。
- 脚本数据来源包括 `quest/` 与 `newquest/`。

## 6. 配置依赖
- `newquest/*.lua`、`quest/` 各事件目录（`on_enter`、`on_kill` 等）。

## 7. 常见问题
- Lua panic 未兜底导致服务进程退出（待确认当前是否全量保护）。
- userdata 生命周期错误导致崩溃/野指针。
- quest/newquest 脚本版本不一致导致行为偏差。

## 8. 二开入口
- 优先查看 `ScenesServer/script.cpp`、`script_func.cpp` 与任务脚本目录。

## 9. 待确认事项
- `liblualib.a` 的实际构建链与装载方式待确认。
- Lua 脚本热更新机制与失败回滚策略待确认。

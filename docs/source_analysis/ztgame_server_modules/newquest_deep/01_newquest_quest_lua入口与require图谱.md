# quest.lua 入口与 require 图谱

- `LUA_PATH = "./newquest/?.lua"`。
- 当前仅显式 `require "./newquest/npc/npc_848"`，其余脚本通过 C++ 事件按函数名动态调用。
- 导出全局：`enter()`、`quit()`、`die()`，均返回 1。

## 加载顺序
1. ScenesServer 启动创建 VM。
2. Binder 绑定 C++->Lua API。
3. 执行 `newquest/quest.lua`。
4. 运行时通过 `execute_script_event` 调 `visit_*`/`state_*`/`kill_*`。

## 缺失表现
- `luaL_dofile` 失败会记录 `FATAL ERROR` 日志。
- luabind 调用异常被捕获并返回 0，业务层可能静默失败。

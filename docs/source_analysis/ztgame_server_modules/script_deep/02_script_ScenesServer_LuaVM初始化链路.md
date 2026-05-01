# ScenesServer Lua VM 初始化链路
1. `ScenesServer.cpp` 启动阶段创建 VM：`ScriptingSystemLua::instance().createVM()`。
2. 创建脚本对象：`createScriptFromFile("newquest/quest.lua")`。
3. `Binder::bind(vm)` 导出 C++ 对象与函数。
4. `vm->execute(script)` -> `luaL_dofile` 执行入口脚本。
5. 之后 `ScriptQuest::get_instance().sort()` 做事件索引加速。

## VM 初始化细节（script.cpp）
- `mLuaState = lua_open()`。
- 按 bitmask 打开库：`luaopen_base/table/io/string/math/debug/package`（5.1 分支用 `lua_cpcall`）。
- 调用 `luabind::open(mLuaState)` 启用绑定系统。

## 失败表现
- dofile 失败：记录 `FATAL ERROR:%s`（`lua_tostring`）但无强制中止逻辑。
- 若脚本函数调用异常：在 `execute_script_event` 捕获 luabind 异常并返回0，业务侧可能仅表现为“任务无响应”。

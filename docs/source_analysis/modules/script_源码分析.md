# script / Lua / luabind 源码分析

## 1. 模块定位
`ztgame/script` 提供 Lua 5.1 运行时头文件、Lua 标准库接口与 `luabind/` 绑定层。场景服 `ScenesServer` 通过 Makefile 链接 `-L../script -llua -lluabind`，说明 Lua/luabind 直接参与玩法与任务执行链路。

## 2. 核心文件与目录
- `lua.h`：Lua C API 头，标识 Lua 5.1 系列接口。
- `lauxlib.h` / `lualib.h` / `lua.hpp`：Lua 辅助库与 C++ 包装入口。
- `luabind/`：C++ 对象、函数、属性暴露给 Lua 的绑定支撑目录。
- `ScenesServer/script.cpp`、`script.h`、`script_func.cpp`、`script_func.h`：场景服侧 Lua 绑定与脚本导出入口。

## 3. 与任务系统关系
- `Quest.cpp` include `luabind/luabind.hpp`，说明任务主流程可直接进入 Lua 绑定层。
- `Quest::execute(...)` 是 C++ 任务状态机与 Lua 事件脚本的汇合点。
- `ScriptQuest::get_instance().has(...)` 命中后会进入 `execute_script_event(...)` 等脚本执行分支。
- `quest/` 与 `newquest/` 并存，存在旧任务与新任务脚本体系并行的版本风险。

## 4. 常见二开入口
- NPC 对话与访问事件：`NPC_VISIT`、`on_visit`、`npc_*.lua`。
- 物品使用与触发事件：`OBJ_USE`、`on_use`、`on_get`、`on_drop`。
- 任务状态：`QuestList`、`FINISHED_NAME`、`state`、`save/load/update/notify`。
- GM 热加载：`loadquest`、`loadnewquest`、`setquest`、`checkquest`。

## 5. 高风险点
1. Lua panic 或 userdata 生命周期错误，可能导致场景服异常或任务卡死。
2. C++ 已修改任务状态但 Lua 执行失败，可能造成状态不一致。
3. `loadnewquest` 热加载后新旧脚本版本漂移，可能导致同一任务 ID 行为不同。
4. C++ 绑定属性名变更，会破坏已有 Lua 脚本调用。

## 6. 建议补充证据
- `execute_script_event(...)` 的参数映射、异常处理、返回码语义。
- `ScriptQuest` 的索引结构：事件类型 -> 对象 ID -> 脚本函数。
- `QuestList::save/load` 的持久化字段与协议快照。
- Lua 错误日志格式与快速定位模板。

## 7. RAG 标签
- `script` `lua` `luabind` `quest` `newquest` `npc_script` `userdata` `high_risk`

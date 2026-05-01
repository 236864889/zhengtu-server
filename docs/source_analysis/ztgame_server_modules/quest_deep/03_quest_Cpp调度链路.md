# Quest.cpp 调度链路
1. 客户端发送 `stRequestQuestUserCmd`。
2. `Quest::execute` 解析 `request->target`：首字符 `v` 走 `OnVisit`，否则走 `OnUse`。
3. `EventTable::instance().execute(user,event)` 执行 XML 动作链。
4. 从 `request->target` 提取对象ID，若 `ScriptQuest::has(OBJ_USE,id)` 命中，调用 `execute_script_event`。
5. 当前 NPC 存在且 offset>0 且 `ScriptQuest::has(NPC_VISIT,npcid)`，再次 Lua 调用重载版本。

## 关键点
- `request->target`：兼容 XML 事件与 Lua 函数路由。
- `execute_script_event`：`script.h` 模板重载（0/1/2参数），返回 `int`；异常捕获后返回0并打日志。
- Lua 入口：`ScriptingSystemLua::reloadVM()` 固定加载 `newquest/quest.lua`。

## 异常表现
- luabind 异常：`CATCHED Luabind EXCEPTION`。
- func 不存在：调用失败返回0，常表现为 NPC 菜单无后续动作。
- target 格式非法：`atoi` 解析对象ID错误，导致 ScriptQuest miss。

待确认：`QuestTable/EventManager` 内部 source 缓存策略（建议查 `ScenesServer/QuestTable.*`、`QuestEvent.*`）。
RAG标签：`quest-cpp-dispatch`, `request-target`, `execute_script_event`。

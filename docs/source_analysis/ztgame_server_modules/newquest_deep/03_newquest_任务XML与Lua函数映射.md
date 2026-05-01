# 任务 XML 与 Lua 函数映射

- C++ `Quest.cpp` 从 `request->target` 解析事件函数名并执行 Lua。
- `quest/quests.xml` 提供 `<event type=... source=...>` 与任务变量定义。
- 三段链路：`quests.xml event/source` -> `request->target` -> `execute_script_event(user,target,...)`。

## 关键事件
- `on_visit`：NPC 对话/任务状态。
- `on_kill`：击杀事件累计变量。
- `on_enter`：进入地图/阶段任务触发。

待确认：`quest/` source xml 与 `newquest` 中具体函数名的自动生成规则。

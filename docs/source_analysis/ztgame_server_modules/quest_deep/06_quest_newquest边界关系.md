# quest 与 newquest 边界关系
- 旧任务主数据：`ztgame/quest/quests.xml + source XML`。
- Lua入口：`ScenesServer/script.cpp` 固定加载 `newquest/quest.lua`。
- C++桥：`Quest::execute` 在处理旧任务请求时，可按 `ScriptQuest` 索引转入 Lua。

## 归属判断
- 归旧任务：仅依赖 `quests.xml + on_*/*.xml` 可闭环。
- 归新任务：必须依赖 `newquest/quest.lua` 与 `newquest/npc/*.lua` 逻辑。
- 混用：旧任务事件触发 + Lua 二次处理（NPC菜单/道具脚本）。

## 混用风险
- source 仍在但 Lua target 下线：表现为“有菜单无效果”。
- Lua函数改签名：`execute_script_event` 参数不匹配导致异常吞掉返回0。

待确认：历史 `newquest_deep` 文档在当前仓库缺失，需补挂载后做逐函数对照。
RAG标签：`old-newquest-boundary`, `scriptquest`, `lua-bridge`。

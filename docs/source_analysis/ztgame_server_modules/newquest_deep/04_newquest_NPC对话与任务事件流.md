# NPC 对话与任务事件流

1. 玩家点击 NPC。
2. `SceneNpc::set_quest_status` 先尝试 `state_<npcid>`。
3. 玩家提交菜单后 `Quest::execute` 调 `execute_script_event(target...)`。
4. Lua 中 `visit_<id>` 进入 `show_dialog` / 接任务 / 交任务分支。
5. 击杀时 `SceneNpc` 死亡逻辑触发 `kill_<npcid>`。

## 常见分支
- 接任务：检查任务状态变量 -> `show_dialog` -> 写入用户变量。
- 完成任务：校验击杀/访问变量 -> 发奖励 -> `refresh_npc`。
- 失败提示：条件不满足时展示错误菜单。

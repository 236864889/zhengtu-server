# quest 二开模板与安全规范
## 新增任务 XML 模板
1) 在 `quests.xml` 新增 `<quest id name>`。
2) 在 `<events>` 绑定 `type+source`。
3) 在 `quest/<type>/` 新增 source XML。

## 新增 source 模板（示意）
- 条件：`<if><var type="Less" name="kill_x" value="N"/></if>`
- 动作：`<var type="Add" name="kill_x" value="1"/>`
- 刷新：`<refresh name="kill_x"/>`
- 完成阈值：`<var type="Great" name="kill_x" value="N-1"/>`

## 新增 NPC/kill/奖励模板
- NPC触发：优先 on_visit source；若需 Lua，登记 ScriptQuest(NPC_VISIT,id) 并保证 quest.lua 函数存在。
- kill触发：落在 `on_kill/<monsterid>.xml`。
- 奖励：统一在 source 动作层配置并做上限审计。

## 灰度与回滚
- 灰度：白名单角色 + 单地图 + 单任务ID。
- 回滚：先回滚 `quests.xml` 入口，再回滚新增 source 文件。
- 监控：grep 日志关键字 `CATCHED Luabind EXCEPTION` / 任务无响应投诉。

RAG标签：`dev-template`, `rollback`, `gray-release`。

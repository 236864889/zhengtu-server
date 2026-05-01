# C++ 导出 Lua 绑定清单（核心）
绑定位于 `ScenesServer/script.cpp` 的 `Binder::bind`。

## 全局函数（节选）
- `me`, `sys`, `all_sys`, `show_dialog`, `show_obj_dialog`, `refresh_npc`, `the_script`
- 概率/随机：`zMisc::randBetween/selectByPercent/selectByTenTh/selectByLakh/selectByMilh`
- 时间：`time`, `difftime`

## class_ 绑定（节选）
- `Channel`
- `String`(std::string)
- `t_Object`, `ObjectB`, `Object`
- `Scene`（如 `country`）
- `CharBase`, `Pos`
- `SceneUser`（升级、传送、增删道具、货币、变量等大量接口）
- `NpcBase`, `SceneNpc`（refresh/tempid/id/drop/summon）
- `Quest` + `.enum_("constants")`
- `Vars`, `GlobalVars`, `QuestList`, `ScriptQuest`

## 高风险绑定
- `SceneUser` 相关经济与物品接口（可直接改资产）。
- `SceneNpc` 召唤/掉落接口（可被脚本滥用）。
- `Quest/ScriptQuest` 常量与 add 流程（事件分发正确性关键）。

待确认：完整 `property(...)` 暴露点较少，当前主要是 `def(...)`；如需字段级清单，建议逐段抽取 `Binder::bind` 全函数。

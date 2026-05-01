# NPC、怪物与任务触发链路
- 核心对象：`SceneNpc`、`SceneNpcManager`、`ScenePet/GuardNpc`、`NpcTrade/MallTrade`。
- 刷新链路：`Scene::init/loadMapFile` 初始刷怪 + 运行时 `summonNpc/summonByNpcMap`。
- AI链路：`SceneNpcAI` 处理巡逻、追击、技能、跨图行为。
- 交互：对话/商店/任务触发 -> `execute_script_event` 或 Quest 系统（Quest/QuestEvent/QuestCondition）。
- 死亡与掉落：`ScenePk` 结算后调用 NPC death 逻辑，广播移除并触发 Session 全局公告等。

待确认：部分特殊活动NPC（国家战/副本）状态机分布在 `SceneCountry.cpp/SceneDare.cpp`。

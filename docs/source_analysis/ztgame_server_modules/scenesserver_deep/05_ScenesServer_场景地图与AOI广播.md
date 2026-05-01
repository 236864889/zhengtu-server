# 场景地图与AOI广播
- `SceneManager::init` 读取国家/地图配置，建立 `country_info/map_info` 与 temp scene。
- `Scene::loadMapFile` 读取 `.mps` + XML，加载阻挡、出生点、传送点、npc定义。
- AOI核心：`sendCmdToNine/sendCmdToDirect/sendCmdToReverseDirect/sendCmdToNineExceptMe`。
- 旁路广播：`SceneTaskManager::broadcastCmd` 跨 SceneTask 广播 maptempid 级消息。
- 周边同步：玩家移动、NPC移动、增删对象均通过 map-screen 指令族同步。
- 性能热点：
  1) 高频九屏广播；
  2) `SceneNpc/SceneUser` 大对象拷贝；
  3) map切换时批量重发 mapdata。

适合问题：卡顿、丢可见、跨屏闪烁。

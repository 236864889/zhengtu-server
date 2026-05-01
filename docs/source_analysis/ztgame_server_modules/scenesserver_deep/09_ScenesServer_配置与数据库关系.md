# 配置与数据库关系
- XML读取：`SceneManager.cpp`（国家/地图）、`Scene.cpp`（地图/NPC/waypoint）、`SceneNpcManager.cpp`（NPC模板）。
- 地图资源：`mapdir + *.mps`（`LoadMap`）。
- 技能/NPC/任务配置：`SkillManager/QuestTable/NpcTrade/MallTrade/GameConfigMgr` 分别加载。
- MySQL：服务支持 `--mysql` 参数，Record链路承担大量持久化；Scene内也有直接记录/异步请求。
- 表名/SQL：本轮在 Scene 目录未完整枚举（待确认：`RecordClient.cpp` + RecordServer 协议映射）。
- 缺配置表现：init失败直接启动中止；运行期缺脚本常见为事件无响应或错误日志。
- 编码风险：大量 `char[]/bcopy/strncpy` 与 GBK/UTF8 混用潜在截断。

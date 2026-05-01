# ScenesServer 总览
- 真实职责：世界场景模拟器，负责地图实例、玩家/NPC状态、AOI广播、战斗与任务驱动，并通过 Session 做跨服中转，通过 Record 做持久化回写。
- 编译产物：`ScenesServer` 可执行（`ScenesServer/Makefile` `ALL_TARGETS`）。
- 服务入口：`ScenesServer.cpp::main` -> `ScenesService::getInstance().main()`。
- 继承关系：`ScenesService : zSubNetService`；`SceneTask : zTCPTask`；`RecordClient : zTCPClient`。

## 关键类/文件 Top30
`ScenesServer.cpp` `SceneTask.cpp` `Scene.cpp` `SceneManager.cpp` `SceneUser.cpp` `SceneNpc.cpp` `SceneNpcManager.cpp` `SceneTaskManager.cpp` `SceneUserManager.cpp` `ScenePk.cpp` `SceneSkill.cpp` `Skill.cpp` `SkillManager.cpp` `SkillStatusManager.cpp` `script.cpp` `script.h` `RecordClient.cpp` `SessionClient.cpp` `LoadMap.cpp` `WayPoint.cpp` `Quest.cpp` `QuestTable.cpp` `QuestEvent.cpp` `NpcTrade.cpp` `MallTrade.cpp` `GameConfigMgr.cpp` `FuBenMgr.cpp` `ObjectManager.cpp` `TimeTick.cpp` `SceneDefine.h`。

## 最高风险文件 Top15
`SceneUser.cpp` `SceneNpc.cpp` `SceneTask.cpp` `Scene.cpp` `SkillStatusManager.cpp` `ScenePk.cpp` `script.h` `RecordClient.cpp` `SceneManager.cpp` `SessionClient.cpp` `LoadMap.cpp` `Quest.cpp` `SceneNpcAI.cpp` `SceneTaskManager.cpp` `ObjectManager.cpp`。

## 与 Gateway/Session/Record/Bill 关系
- Gateway 不直连 Scene 业务逻辑；通常经 Session 路由到 SceneTask 分发。
- Session 是 Scene 的上游控制/跨服广播中心（注册地图索引、跨服消息、踢人/惩罚）。
- RecordClient 处理角色落地、部分异步查询/回包。
- Bill 主要由 SceneUser 的 `sendCmdToBill`/充值消费相关命令触发（链路需与 BillServer 文档联读）。

## RAG 入库优先级
- S：`00 02 03 04 05 07 08 09 10`
- A：`01 06 11 13`
- B：`12`

## 适合解决的问题
- 卡地图/切图失败、九屏广播异常、战斗结算争议、任务脚本崩溃、下线不保存。

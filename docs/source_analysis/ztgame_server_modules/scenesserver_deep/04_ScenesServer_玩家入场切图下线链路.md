# 玩家入场、切图、下线链路
- 创建：`SceneTask` 收到用户数据/进入命令后定位或创建 `SceneUser`，绑定 `gatetask/session`。
- 入场：`SceneUser::changeMap` + `Scene::sendMeToNine/sendCmdToNine` 下发周边玩家/NPC。
- 切图：`Scene::changeMap` 根据 backto/death/country/fuben 规则选目标 Scene；失败时回送 session 修正。
- 传送/回城：依赖 WayPoint、地图配置 backto 字段、剧情/任务脚本触发。
- 下线：触发 `execute_script_event(user,"quit")`，发送保存命令到 Record，清理 team/npc/对象索引。
- 断线回收：task 回收线程 + SceneUserManager 清理，广播 remove 包到九屏。

高风险：跨线程状态（task/gate/session）不一致导致“幽灵在线”。

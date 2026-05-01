# Scene 与 Session/Gateway 通信链路
- Session->Scene：`SceneTask::cmdMsgParse` 按 cmd/para 分发到玩家、地图、系统处理。
- Scene->Session：`sessionClient->sendCmd` 发送场景注册、地图索引刷新、Boss/活动状态、用户跨服通知。
- Gateway经Session：Gateway命令进入 Session 后转发 Scene（与 session/gateway 既有文档对齐）。
- 关键转发结构：`Cmd::Scene::t_*ForwardScene`、`Cmd::Session::t_*SceneSession`、`t_User_ForwardMap`。
- 入场/切图/下线：`SceneTask`接收指令 -> `SceneUser/Scene::changeMap` -> 向 session/gate/record 各自回执。

待确认：Session 侧 para 编号精确对齐表（建议下一步补 `SessionCommand.h` + `SceneTask.cpp case` 对照表）。

# MiniServer 总览

## 真实职责
- MiniServer 是独立小游戏后端（房间/牌局/积分/金币子账本），并非主战斗场景服；核心对象链路是 `MiniHall -> MiniRoom -> MiniGame(DDZCardGame)`。 
- 它通过 Gateway 接入玩家登录/登出/用户消息转发，通过 Scene 处理充值提现联动（`PARA_SCENE_DEPOSIT/CHECK_DRAW/DRAW_RET`）。
- 它还维护独立 `MINIGAME` 表分值与 money 字段，具备离线写回能力。 

## 编译产物
- 可执行：`MiniServer`。 
- 编译对象含 `DDZCardGame/Top100/MiniHall/MiniTask/MiniUser*`，说明功能以小游戏玩法与榜单为中心。 

## 服务入口
- 入口在 `MiniServer.cpp::main`，启动 `MiniService::getInstance().main()`。
- `MiniService` 继承 `zSubNetService`，默认参与 Super 注册/发现体系。 

## 继承 base 哪些类
- `MiniService : zSubNetService`（网络+Super 子服务语义）。
- `MiniTask : zTCPTask, MessageQueue`（连接/消息分发）。
- `MiniTimeTick : zThread`（玩法计时线程）。

## 关键类/文件 Top30
1. MiniServer.cpp/h
2. MiniTask.cpp/h
3. MiniTaskManager.cpp/h
4. MiniHall.cpp/h
5. MiniRoom.cpp/h
6. MiniGame.cpp/h
7. DDZCardGame.cpp/h
8. CardPattern.cpp/h
9. MiniUser.cpp/h
10. MiniUserManager.cpp/h
11. Top100.cpp/h
12. TimeTick.cpp/h
13. Makefile
14. base/MiniCommand.h
15. base/MiniUserCommand.h
16. GatewayServer/MiniClient.cpp/h
17. GatewayServer/GatewayTask.cpp
18. ScenesServer/MiniClient.cpp/h
19. ScenesServer/RecordClient.cpp
20. ScenesServer/SceneTask.cpp
21. ScenesServer/SceneUser.cpp
22. SuperServer 文档（发现链路）
23. base/zSubNetService.cpp
24. base/zService.cpp
25. base/zTCPTaskPool.cpp
26. base/zDBConnPool.*
27. base/zMetaData.*
28. Config 中 MiniServer 配置节点（待确认）
29. 顶层 *.xml（server/port/mysql）
30. MiniServer/README.md（仅对照，不覆盖）

## 最高风险文件 Top15
- `MiniTask.cpp`（三路协议分发+DB 补偿写）。
- `MiniUserManager.cpp`（登录建档/读档/断线清理）。
- `MiniUser.cpp`（money 与积分写库）。
- `MiniHall.cpp`（玩法入口与场景提款/充值桥接）。
- `DDZCardGame.cpp`（回合状态机+超时自动操作）。
- `MiniGame.cpp`（座位/状态机基础逻辑）。
- `Top100.cpp`（榜单一致性）。
- `MiniServer.cpp`（启动与资源初始化顺序）。
- `MiniTaskManager.cpp`（serverID->task 唯一映射）。
- `TimeTick.cpp`（线程时序）。
- `Makefile`（mysql/xml/gd/jpeg 链接风险）。
- `base/MiniCommand.h`（跨服协议编号）。
- `GatewayServer/MiniClient.cpp`（网关对接重连与回包）。
- `ScenesServer/MiniClient.cpp`（场景对接资金回执）。
- `base/MiniUserCommand.h`（客户端小游戏命令结构）。

## 与 Gateway/Session/Scenes/Super 的关系
- Gateway：用户首次发 MiniGame cmd 时触发 `t_UserLogin_Gateway`，后续通过 `PARA_USER_FORWARD_MINI` 转发到 Mini。 
- Scenes：Scene 也直连 Mini（独立 MiniClient），用于 `SetScene/Deposit/Draw` 资金与场景绑定。 
- Session：未见 Mini 与 Session 直连处理代码，当前链路更像 Gateway/Scene 双入口。待确认 Session 是否仅透传。 
- Super：MiniService 继承 zSubNetService 且支持 `--server/--port`，按框架应注册 Super 并消费 serverList。 

## RAG 入库优先级
- S：`00/02/03/04/05/06`
- A：`01/08/09`
- B：`07`（索引文件）

## 适合解决的问题
- Mini 登录后为什么消息不回包。
- 充值/提现失败时 money 是否丢失。
- Mini 玩法卡死/超时自动出牌争议。
- 网关或场景重连后 Mini 用户残留。

## 待确认
- `ztgame/Config` 中 MiniServer 具体节点名、端口与线程参数来源文件。下一步查 `ztgame/Config/*.xml` 与顶层 `ztgame/*.xml`。

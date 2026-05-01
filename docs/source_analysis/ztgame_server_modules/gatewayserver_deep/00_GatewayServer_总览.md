# GatewayServer 总览

## 真实职责
- `GatewayService` 继承 `zSubNetService`，对外承接客户端 TCP 连接，对内连接 Session/Bill/Record/Mini/Scene 等后端服务，承担“接入层 + 转发层 + 初始鉴权层”。
- 核心职责不是业务计算，而是：版本校验、登录会话校验、连接状态机推进、按协议转发、断线清理、在线数上报。关键实现集中在 `GatewayTask` 与 `SessionClient`。

## 编译产物
- 单一可执行产物：`GatewayServer`（由 `GatewayServer/Makefile` 的 `ALL_TARGETS` 定义）。

## 服务入口
- 入口在 `GatewayServer.cpp` 的 `main`（通过 `GatewayService::getInstance().main(argc,argv)` 进入基类生命周期）。
- 生命周期主路径：`init -> validate -> serviceCallback(loop, inherited) -> final`。

## 继承 base 哪些类
- 服务：`GatewayService : public zSubNetService`。
- 玩家连接任务：`GatewayTask : public zTCPTask`。
- 对 Session/Record/Bill/Mini 的外连：`*Client : public zTCPBufferClient`。
- Scene 外连池任务：`SceneClient : public zTCPClientTask`（按条件编译有另一实现）。

## 关键类/文件 Top20
1. `GatewayServer.cpp`
2. `GatewayServer.h`
3. `GatewayTask.cpp`
4. `GatewayTask.h`
5. `SessionClient.cpp`
6. `SessionClient.h`
7. `GateUser.cpp`
8. `GateUser.h`
9. `GateUserManager.cpp`
10. `GateUserManager.h`
11. `GatewayTaskManager.cpp`
12. `GatewayTaskManager.h`
13. `LoginSessionManager.cpp`
14. `BillClient.cpp`
15. `SceneClient.cpp`
16. `SceneClientManager.cpp`
17. `RecordClient.cpp`
18. `MiniClient.cpp`
19. `TimeTick.cpp`
20. `CmdChecker.*`

## 最高风险文件 Top10
1. `GatewayTask.cpp`（登录态与转发总枢纽）
2. `SessionClient.cpp`（Session->Gate 回包驱动状态）
3. `GateUser.cpp`（用户状态机）
4. `GateUserManager.cpp`（用户索引与批量广播）
5. `LoginSessionManager.cpp`（登录临时票据）
6. `BillClient.cpp`（账务验证结果影响放行）
7. `GatewayServer.cpp`（外连初始化与失败策略）
8. `GatewayTaskManager.cpp`（按 accid 唯一连接）
9. `SceneClient.cpp`（Scene 路由与索引）
10. `TimeTick.cpp`（周期清理与状态推进）

## 与 SessionServer/客户端/ScenesServer 的关系
- 客户端先连 Gateway，先过 `verifyVersion` + `verifyACCID`，再经 Bill 异步校验放行。
- Gateway 与 Session 之间使用 `Cmd::Session::CMD_GATE` 与 `CMD_FORWARD` 双向通信：
  - Gate->Session：注册/反注册/删角/转发。
  - Session->Gate：踢下线、用户定向/国家/全服转发、黑名单同步等。
- 与 Scene 为多连接池模型：Gateway 转发场景包到具体 `SceneClient`，并维护屏索引辅助范围广播。

## RAG 入库优先级
- S 级：`02/03/04/05/06/07`
- A 级：`00/01/08/10`
- B 级：`09`

## 适合解决的问题
- 登录后“卡在网关”问题。
- Session 发踢人但客户端未断的链路断点定位。
- 网关误封/误限流导致的登录失败排障。
- Gateway 与 Scene 转发错路由问题。

## 待确认
- 部分防刷细则在 `GateUser.cpp` 内分散实现，建议下一轮逐段细化（见 `10_下一步深挖建议`）。

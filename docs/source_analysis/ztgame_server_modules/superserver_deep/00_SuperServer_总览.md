# SuperServer 总览（函数级/链路级）

## 真实职责
- `SuperService` 是“服务注册中心 + 启动编排器 + 全局转发枢纽”，负责维护各后端服务连接 `ServerTask`，做唯一性校验、服务发现广播、启动时依赖顺序同步（`processSequence/notifyOther/notifyMe`）。
- 它同时承担三类外联网关：
  - 到 FL（登录/列表）链路：转发 Gateway 列表与新会话；
  - 到 Rolereg 链路：角色名占用检测/写入/清理；
  - 到 Info 链路：GM 日志、聊天、惩罚、国家在线数查询回包。

## 编译产物
- 可执行：`SuperServer`（`ALL_TARGETS = SuperServer`）。
- 对象：`SuperServer.o ServerTask.o ServerManager.o TimeTick.o FLClient*.o Rolereg*.o Info*.o`。

## 服务入口
- 入口：`ztgame/SuperServer/SuperServer.cpp::main`。
- 生命周期：`SuperService::getInstance().main()` -> `init` -> `zService` 循环 -> `final`。

## 继承 base 哪些类
- `SuperService : zNetService`（注意：不是 `zSubNetService`）。
- `ServerTask : zTCPTask`（管理接入的业务服连接）。
- `FLClient/RoleregClient/InfoClient : zTCPClientTask`（Super 主动外连的三类上游）。

## 关键类/文件 Top30
1. SuperServer.cpp/h
2. ServerTask.cpp/h
3. ServerManager.cpp/h
4. TimeTick.cpp/h
5. FLClient.cpp/h
6. FLClientManager.cpp/h
7. RoleregClient.cpp/h
8. RoleregClientManager.cpp/h
9. RoleregCache.cpp/h
10. InfoClient.cpp/h
11. InfoClientManager.cpp/h
12. NetType.h
13. SuperServer/Makefile
14. base/SuperCommand.h
15. base/zSubNetService.cpp（对照服务发现消费端）
16. base/zConfile.cpp（SuperServer 配置读取行为）
17. GatewayServer/GatewayServer.cpp（按 Super serverList 取 Session/Bill/Scene/Record/Mini）
18. GatewayServer/SceneClientManager.cpp
19. GatewayServer/BillClient.cpp
20. SessionServer/SessionServer.cpp
21. ScenesServer/ScenesServer.cpp
22. BillServer/BillServer.cpp
23. GatewayServer/GatewayTask.cpp
24. SessionServer/SessionTask.cpp
25. ScenesServer/Chat.cpp
26. base/zService.cpp
27. base/zNetService.cpp
28. base/zTCPTaskPool.cpp
29. base/zSocket.cpp
30. SuperServer/README.md（仅参考，不覆盖）

## 最高风险文件 Top15
- `ServerTask.cpp`（启动握手 + 多协议路由 + GM/关服路径）
- `ServerManager.cpp`（注册表容器 + 广播 + 唯一性）
- `SuperServer.cpp`（DB/线程池/监听口初始化关键路径）
- `TimeTick.cpp`（全局时钟持久化 + 全服广播）
- `FLClient.cpp`（新会话 FL->Bill 关键中转）
- `InfoClient.cpp`（GM 消息回流 + 国家在线查询触发）
- `RoleregCache.cpp`（失败缓存重放，存在积压/一致性风险）
- `RoleregClientManager.cpp`（角色名服务可用性单点）
- `FLClientManager.cpp`
- `InfoClientManager.cpp`
- `base/SuperCommand.h`（协议编号中心）
- `base/zSubNetService.cpp`（消费端 serverList 结构）
- `GatewayServer/GatewayServer.cpp`（依赖 Super 发现的初始化阻断点）
- `SessionServer/SessionServer.cpp`
- `ScenesServer/ScenesServer.cpp`

## 与 Gateway/Session/Scenes/Record/Bill/Mini/FL 的关系
- Gateway：上报 `GYList/NewSession/Charname` 到 Super；Super 再转发给 FL / Bill / Rolereg。
- Session/Scenes/Record/Bill/Mini：通过 `zSubNetService` 接入 Super 的 `ServerEntry` 列表做发现；Super 负责上线通知与启动同步。
- FL：Super 主动连 FL，回传区服列表、会话中转、ID占用结果。
- Info：Super 主动连 Info，转发 GM 与在线统计链路。

## RAG 入库优先级
- S级：`00/02/03/04/05/06/07/08`
- A级：`01/10/11`
- B级：`09`（manifest 索引）

## 适合解决的问题
- “为什么子服务拿不到 ServerList/取错服务IP端口？”
- “登录后 NewSession 在 FL/Bill 链路哪一段丢失？”
- “GM 广播/处罚命令为何未到达 Info/Scene/Session？”

## 待确认
- `serverSequence` 的完整依赖图定义位置（当前见 `ServerTask` 使用，建议下一步查 `ServerTask.cpp` 顶部静态表完整定义段）。

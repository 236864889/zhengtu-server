# SessionServer 总览

## 真实职责
- `SessionService` 是 **全局会话与跨服中转核心**：承接 Gateway/Scene 连接、维护用户与场景会话、进行跨服务广播/转发，并承载大量“全局系统”业务管理器（国家、家族、盟友、推荐、邮件等）。
- 它不是纯“登录认证服务”，而是“登录后在线态路由 + 全局业务协调层”。

## 编译产物
- 目标二进制：`SessionServer`（由 `SessionServer/Makefile` 的 `ALL_TARGETS` 指定）。

## 服务入口
- 入口函数：`SessionServer.cpp::main`。
- 主体实例：`SessionService::getInstance().main()`（继承自 base 生命周期骨架）。

## 继承 base 哪些类
- `SessionService : public zSubNetService`。
- `RecordClient : public zTCPClient, public MessageQueue`。
- `SessionTask : public zTCPTask, public MessageQueue`（由 `SessionTask` 对外 msgParse/cmdMsgParse 可见）。

## 关键类/文件 Top20
1. `SessionServer.cpp/.h`（启动、生命周期、Super 消息处理）
2. `SessionTask.cpp/.h`（协议分发主战场）
3. `SessionTaskManager.cpp/.h`（Gateway/Scene 连接集合与广播）
4. `SessionManager.cpp/.h`（UserSession/SceneSession 管理）
5. `Session.cpp/.h`（会话对象）
6. `RecordClient.cpp/.h`（到 RecordServer 客户端）
7. `SessionChat.cpp/.h`（世界/国家/私聊广播）
8. `CCountryManager.cpp/.h`
9. `CSept.cpp/.h`
10. `CUnion.cpp/.h`
11. `CCityManager.cpp/.h`
12. `CDare.cpp/.h`
13. `CQuiz.cpp/.h`
14. `CNpcDare.cpp/.h`
15. `CSort.cpp/.h`
16. `MailService.cpp/.h`
17. `RecommendManager.cpp/.h`
18. `Ally.cpp/.h`
19. `GameConfigMgrX.cpp/.h`
20. `TimeTick.cpp/.h`

## 最高风险文件 Top10
- `SessionTask.cpp`（超大分发、状态流转复杂）
- `SessionServer.cpp`（初始化链路长、失败点多）
- `SessionManager.cpp`（在线用户索引与批量操作）
- `SessionTaskManager.cpp`（连接容器并发/一致性）
- `RecordClient.cpp`（Record 断开触发服务终止）
- `CSept.cpp`（协议+DB+在线状态耦合深）
- `CCountryManager.cpp`
- `CUnion.cpp`
- `RecommendManager.cpp`
- `MailService.cpp`

## 与其他服务关系
- **SuperServer**：通过 `zSubNetService` 注册/服务发现，且处理 GMTool/Scene 类下行。
- **RecordServer**：主动 TCP 客户端连接，登录握手后处理回包（如推荐系统用户存在校验）。
- **GatewayServer**：以 `CMD_GATE` 路径进入 `SessionTask::msgParse_Gate`。
- **ScenesServer**：以 `CMD_SCENE` 路径进入 `SessionTask::msgParse_Scene`，并有大量 `*_SceneSession` 转发。
- **BillServer**：本目录未见直接 client 类，推断主要通过其他服务链路/协议透传（待确认）。

## RAG 入库优先级
- S：`SessionServer.cpp/.h`、`SessionTask.cpp/.h`、`SessionTaskManager.*`、`SessionManager.*`
- A：`RecordClient.*`、`Session.*`、`SessionChat.*`
- B：各业务管理器（按问题域取用）

## 适合解决的问题
- 登录后“网关 -> 会话 -> 场景”命令失联排查
- 跨服广播/国家广播异常
- 在线用户状态不一致、踢人/重连异常
- Session 启动失败定位

## 待确认
- `validate/serviceCallback` 的 Session 层覆写点未在本目录直接出现，需回溯 `zSubNetService` 与 `zService` 默认实现。

# FLServer 总览

## 真实职责
- FLServer 是“登录前置聚合层”：对外处理客户端登录口(`login_port`)，对内维护与 Super 的内部连接口(`inside_port`)以及 ping 列表口(`ping_port`)；负责账号口令校验、验证码校验、tempid 会话跟踪、区服/Gateway 列表缓存与选择、把 NewSession/IDInUse 回包转给原登录连接。源码：`ztgame/FLServer/FLServer.cpp`、`LoginTask.cpp`、`LoginManager.cpp`、`ServerTask.cpp`。
- 职责边界：
  - **做**：登录前校验、可用网关选择、向 Super 发 NewSession、接收 IDInUse/登录成功回包。  
  - **不做**：真实角色上线与场景流量（Gateway/Scene 负责）、计费业务处理（Bill 负责）、登录器实现（本轮不涉及）。

## 编译产物
- 可执行文件：`FLServer`。`ztgame/FLServer/Makefile`
- 关键对象：`FLServer.o LoginTask.o LoginManager.o GYListManager.o ServerTask.o TimeTick.o ...`

## 服务入口与生命周期
- 入口：`main()` -> `FLService::getInstance().main()`。`ztgame/FLServer/FLServer.cpp`
- 继承：`FLService : zMNetService`（非 `zSubNetService`，说明它通过“内部端口+ServerTask”接入 Super，而不是 base 的标准 SubNet SuperClient 模式）。`ztgame/FLServer/FLServer.h`
- 生命周期关键点：`init()` 初始化 DB/ACL/InfoClient/taskpool/bind 三端口；`newTCPTask()` 按端口分发 LoginTask/ServerTask/PingTask；`final()` 回收 managers 与定时线程。

## 关键类/文件 Top30
1. FLServer.cpp 2. FLServer.h 3. LoginTask.cpp 4. LoginTask.h 5. LoginManager.cpp 6. LoginManager.h 7. ServerTask.cpp 8. ServerTask.h 9. ServerManager.cpp 10. ServerManager.h 11. GYListManager.cpp 12. GYListManager.h 13. TimeTick.cpp 14. TimeTick.h 15. PingTask.cpp 16. PingTask.h 17. ServerACL.cpp 18. ServerACL.h 19. InfoClient.cpp 20. InfoClient.h 21. InfoClientManager.cpp 22. InfoClientManager.h 23. DBAccessClient.cpp 24. DBAccessClient.h 25. DBAccessClientManager.cpp 26. DBAccessClientManager.h 27. Makefile 28. ../base/FLCommand.h 29. ../base/SuperCommand.h 30. ../base/DBAccessCommand.h

## 最高风险文件 Top15
- `LoginTask.cpp`（口令校验、验证码、版本过滤、tempid）
- `LoginManager.cpp`（网关选择、并发会话映射、超时回包）
- `ServerTask.cpp`（Super->FL 的 GYList/NewSession/IDInUse 分发）
- `GYListManager.cpp`（区服列表维护、版本门槛）
- `FLServer.cpp`（三端口、线程池、配置默认值）
- `ServerManager.cpp`（内部服务连接唯一性与发送）
- `TimeTick.cpp`（超时清理驱动）
- `PingTask.cpp`（客户端 ping 列表返回）
- `ServerACL.cpp`（zoneInfo ACL 读取）
- `InfoClient*.cpp`（上报链路）
- `DBAccessClient*.cpp`（备用外置 DBAccess 链路，现主路径多为本地 DB 直连）

## 与 Super/Gateway/Bill/客户端入口关系
- 客户端 -> FL(LoginTask)：先 `USER_VERIFY_VER`，再 `USER_REQUEST_LOGIN`。
- FL -> Super(ServerTask.sendCmd)：初连后发 `PARA_FL_RQGYLIST` 请求全量网关列表。
- Super -> FL：转发 Gateway 的 `PARA_GATEWAY_GYLIST` 给 FL，FL 写入 `GYListManager`。
- FL(LoginManager) -> Super：发 `PARA_SESSION_NEWSESSION`。
- Super -> Bill：转 `PARA_BILL_NEWSESSION`；Bill 再回 Gateway `PARA_GATE_NEWSESSION`；Gateway 再上送 Super `PARA_GATEWAY_NEWSESSION`；Super 回 FL `PARA_GATEWAY_NEWSESSION`，FL 最终通知客户端登录成功。
- Bill 重复登录回 `PARA_BILL_IDINUSE`，经 Super 转 FL `PARA_SESSION_IDINUSE`，FL 回客户端失败。

## RAG 入库优先级
- S级：`00/02/03/04/05/07`
- A级：`01/06/10`
- B级：`08/09`

## 适合解决的问题
- 登录成功率低、随机“账号已在线”、区服列表为空/错乱、版本门槛误杀、网关维护态不同步、tempid 回包丢失。

## 待确认
- `networkType` 字段在 FL 层是否被实用过滤（结构体有字段，但当前 FL 代码主链路未见主动筛选）；建议下一轮补查客户端协议消费点与 Gateway 上报扩展字段。

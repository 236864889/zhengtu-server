# BillServer 总览

## 真实职责
- `BillService` 是账务与点数中枢：负责 Gateway 登录放行二次校验、点数查询/消耗、金币/月卡兑换、钱庄委托交易、异常补单记录。核心入口在 `BillTask::cmdMsgParse` 与 `BillUser::*`。源码：`ztgame/BillServer/BillTask.cpp`、`ztgame/BillServer/BillUser.cpp`。
- 同时它还是“上游统一计费网关客户端”：通过 `BillClientManager/BillClient/BillCache` 把消耗/充值请求发到外部统一 BillServer，再回写结果。源码：`ztgame/BillServer/BillClientManager.cpp`、`BillClient.cpp`、`BillCache.cpp`。

## 编译产物
- 产物：`BillServer` 可执行文件；静态依赖 `../base/libzebra.a`。见 `ztgame/BillServer/Makefile`。

## 服务入口
- `main -> BillService::getInstance().main(argc,argv)`。
- 生命周期：`init -> zSubNetService::init -> 循环(serviceCallback 继承链) -> final`。见 `ztgame/BillServer/BillServer.cpp`。

## 继承 base 的类
- `BillService : zSubNetService`。
- `BillTask : zTCPTask`（网关/场景侧连接任务）。
- `BillClient : zTCPClient`（外部统一计费外连）。

## 关键类/文件 Top30
`BillServer.cpp/.h` `BillTask.cpp/.h` `BillUser.cpp/.h` `BillUserManager.cpp/.h` `BillTaskManager.cpp/.h` `BillManager.cpp/.h` `BillSessionManager.cpp/.h` `BillClient.cpp/.h` `BillClientManager.cpp/.h` `BillCache.cpp/.h` `BillCallback.cpp/.h` `StockConsign.cpp/.h` `TimeTick.cpp/.h` `Makefile` `../base/BillCommand.h`。

## 最高风险文件 Top15
1. `BillUser.cpp`（资金增减、补单、无显式事务）
2. `StockConsign.cpp`（撮合与批量 SQL、退款/税）
3. `BillTask.cpp`（登录放行、转发分发总入口）
4. `BillClient.cpp`（外部计费回包与状态判定）
5. `BillCache.cpp`（tid 缓存与超时）
6. `BillServer.cpp`（DB 初始化与服务装配）
7. `BillClientManager.cpp`（多外连与轮询）
8. `BillUserManager.cpp`（用户容器并发）
9. `BillManager.cpp`（登录态状态机）
10. `BillSessionManager.cpp`（会话去重）
11. `TimeTick.cpp`（定时驱动）
12. `BillCallback.cpp`（C 回调桥）
13. `StockConsign.h`
14. `BillUser.h`
15. `BillTaskManager.cpp`

## 与 Gateway/Scene/Record/Session 关系
- Gateway -> Bill：`PARA_GATE_LOGINVERIFY`、`PARA_GATE_LOGOUT`、点数/兑换命令。Bill 回 `t_LoginVerify_Gateway_Return` 等。协议定义在 `base/BillCommand.h`。
- Scene -> Bill：经 `CMD_FORWARD/PARA_SCENE_FORWARD_BILL` 转发，Bill 在 `BillUser::usermsgParseScene` 做钱庄存取/扣费。
- Session/Record：Bill 不直接重度依赖 Record；通过 Super/Forward 机制与 Session 间接协作（登录会话分配、跨服路由）。

## RAG 入库优先级
- S: `00/03/04/05/06/07`
- A: `01/02/09/10`
- B: `08`

## 适合解决的问题
- “登录卡在网关校验”链路断点。
- “充值/消费后余额异常”定位。
- “重复扣费/补单失败”风险分析。

## 待确认
- 外部统一计费返回码语义需结合外部 BillServer 协议文档进一步确认（建议下轮补看 `ztgame/Config` 下账务配置与上游接口文档）。

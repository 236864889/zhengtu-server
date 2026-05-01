# GatewayServer 启动与生命周期

## 启动入口
- `main -> GatewayService::getInstance().main(argc,argv)`，复用 base 生命周期框架。

## 服务继承关系
- `GatewayService : zSubNetService`。
- 因此拥有 SuperServer 注册/回调、TCP 监听、任务池调度等基类能力。

## init 链路（关键顺序）
1. 读取并设置 `verify_client_version`。
2. 初始化 `country_info`。
3. 构建 `zTCPTaskPool(4096, state, 65000)`。
4. 按网卡名取监听 IP。
5. 调 `zSubNetService::init()`。
6. 外连 Session 并启动线程。
7. 外连 Bill 并启动线程。
8. 初始化 SceneClientManager（若配置存在 Scene）。
9. 外连 Record、Mini 并启动线程。
10. 初始化 `GateUserManager`。
11. 启动 `GatewayTimeTick`。

## validate
- 覆盖实现里先调 `zSubNetService::validate()`，随后 `notifyLoginServer()` 上报网关地址、端口、在线数、状态、版本。

## serviceCallback
- 未显式覆盖，沿用 `zNetService/zSubNetService` 回调循环。

## final
- `GatewayService::final()` 与 `delInstance()` 负责：回收 taskPool、客户端对象、管理器单例等（含线程 `join`）。

## 启动失败点
- 任一核心后端（Session/Bill/Record/Mini）缺配置或连接失败均可能 `return false`；其中 Session 连接失败仅日志不立即退出，存在“半启动”风险。
- taskPool 初始化失败会直接失败。

## 上下游
- 上游：客户端 TCP、SuperServer 管控。
- 下游：Session/Bill/Record/Mini/Scene。

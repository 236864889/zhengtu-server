# FLServer 启动与生命周期

## 主入口
- `main()` 设默认配置（7000/7001/7002、zoneInfo、InfoServer、jpeg_passport），解析 `FLServer` 配置段，解析命令行，再进入 `FLService::main()`。

## 继承关系
- `FLService : zMNetService`。
- 连接任务：`LoginTask/ServerTask/PingTask : zTCPTask`。

## init
- 初始化 DB 连接池 `zDBConnPool`。
- 初始化 ACL(`ServerACL`)、InfoClientManager。
- 建立三个 `zTCPTaskPool`。
- `bind` 三端口：login/inside/ping。
- 启动 `FLTimeTick`。

## validate/serviceCallback/final
- `validate/serviceCallback` 走 `zMNetService` 基类主循环。
- `newTCPTask` 按监听端口分流到不同 Task。
- `final`：先 `zMNetService::final`，再销毁 Manager、停止 TimeTick。

## 启动失败点
- mysql URL 无效；
- 任一 taskpool init 失败；
- 端口 bind 失败；
- `zoneInfo.xml`/InfoClient 初始化失败。

## 待确认
- `zMNetService::validate()` 的 FL 专属扩展行为；下一步查 `ztgame/base/zMNetService.*`。

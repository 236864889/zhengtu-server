# 网络样例链路：TCPClient / TCPServer（静态）

## 覆盖文件
- `ztgame/test/TCPClient.cpp`
- `ztgame/test/TCPServer.cpp`

## TCPClient.cpp 关键链路
- `TCPClient : zTCPClient`，在 `run()` 中调用 `recvToCmd` 收包，空包回发，其他命令进入 `msgParse`。
- `SendThread : zThread` 在循环中构造 `Cmd::t_NullCmd` 并 `sendCmd`。
- `TCPClientService : zService` 在 `init()` 中批量创建客户端并 `connect("192.168.8.129", 9000)`，再启动发送线程。
- `final()` 中 `final/join` 线程并回收 client。

## TCPServer.cpp 关键链路
- `TCPServerTask : zTCPTask` 负责连接上的命令处理。
- `TCPServer : zNetService` 维护 `zTCPTaskPool` 与监听 socket，服务循环中 `accept` 后创建任务入池。
- 依赖 `Zebra::global` 读线程池初始化状态、容量等参数。

## 使用到的 base 网络类
- `zTCPClient`、`zTCPServer`、`zTCPTask`、`zTCPTaskPool`、`zNetService`、`zService`。

## 连接/监听流程（模板价值）
- 客户端模板：`connect -> send thread -> recv loop -> final/join`。
- 服务端模板：`init pool -> listen -> accept -> new task -> pool dispatch`。
- 适合作为“最小联调骨架”，可迁移为 dry-run 版本（禁真实外连、禁真实命令）。

## 运行风险
- 写死 IP/端口与老协议结构，跨环境直接失败概率高。
- 无限循环 + 高频发送，易造成压测级流量或阻塞。
- 命令解析/边界检查较弱，不适合直接暴露生产网络。

## RAG 标签 & 适合问题
- 标签：`tcp-client` `tcp-server` `task-pool` `legacy-protocol`。
- 适合问题：
  - “zTCPClient/zTCPTaskPool 在旧项目里怎么搭建？”
  - “如何从 test 提炼网络联调最小样例？”

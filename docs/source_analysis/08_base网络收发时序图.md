# base 网络收发时序图（文字版）

## 场景 1：服务启动到监听
1. `main()` 创建 `*Service`。
2. `*Service::init()` 调用 `zNetService::init(port)`。
3. `zNetService` 创建监听 socket（`zSocket`）。
4. 初始化任务池（`zTCPTaskPool` 或衍生）。
5. 进入主循环，等待连接。

## 场景 2：新连接接入到任务分发
1. 监听 socket 接受新连接。
2. 创建 `zTCPTask`（或衍生 task）。
3. task 注册到 `zTCPTaskPool`。
4. 任务线程从队列取 task 并处理读写。
5. 上层 `*Task` 执行业务协议分发。

## 场景 3：收包处理
1. task 调用 `zSocket::recvToCmd()` 读取命令头/包体。
2. 校验长度、命令字、会话状态。
3. 通过 `verifyLogin/verifyConn`（在业务 Task 中常见）。
4. 分发到 `Cmd::*` 对应处理分支。
5. 如需回包，调用 `sendCmd()` 或等价封装。

## 场景 4：连接巡检
1. 周期触发 `validate()`（服务层/子网层）。
2. 检查连接活性、超时、心跳。
3. 异常连接执行断开与清理。
4. 必要时触发重连策略（客户端管理器侧）。

## 场景 5：关闭与清理
1. 服务退出触发 `final()`。
2. 停止接收新连接。
3. 任务池停止并回收线程。
4. 关闭 socket、释放连接对象。
5. 输出结束日志。

## 关键风险点
- 包长校验不严：可能引发越界/协议错位。
- 任务池背压不足：高峰期排队增长导致超时。
- validate 误判：误踢正常连接或放过僵尸连接。
- final 清理不完整：资源泄漏或进程无法平滑退出。

## 排查关键词
- `recvToCmd` `sendCmd` `checkIOForRead` `checkIOForWrite`
- `zTCPTaskPool` `addMain` `put` `timeAction`
- `validate` `final` `disconnect` `timeout`

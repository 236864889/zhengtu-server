# SuperServer 启动与生命周期

## 启动入口
- `main`：初始化 logger、默认配置（`loginServerListFile/mysql/logfilename`），解析 `SuperServer` 配置与命令行参数后进入 `SuperService::main()`。

## 继承关系
- `SuperService : zNetService`，覆写：`init/newTCPTask/final/reloadConfig`。
- `ServerTask : zTCPTask`，覆写：`verifyConn/waitSync/recycleConn/msgParse/...`。

## init 关键链路
1. 初始化 DB 连接池 `zDBConnPool::newInstance + putURL(mysql)`。
2. `ifname` 取本机 IP (`zSocket::getIPByIfName`)。
3. `getServerInfo()` 从 `SERVERLIST` 查询/回填 `SUPERSERVER` 自身 ID/端口/外网IP。
4. 初始化 `FLClientManager/RoleregClientManager/InfoClientManager`。
5. 初始化 `zTCPTaskPool(threadPoolCapacity, state)`。
6. `zNetService::init(wdPort)` 建监听。
7. 启动 `SuperTimeTick` 线程。

## validate/serviceCallback/final
- `validate/serviceCallback` 沿用 `zService/zNetService` 基类行为（accept + 任务分派）。
- `newTCPTask`：每个入站连接包装为 `ServerTask` 后进校验池。
- `final`：停止 TimeTick、停 taskPool、销毁 ClientManager、销毁 `ServerManager/RoleregCache/dbConnPool`。

## 启动失败点
- MySQL URL 不可用。
- `SERVERLIST` 记录冲突（同类型 SUPERSERVER 多条或IP不一致）。
- `loginServerList.xml` 不可读（FL/Rolereg/Info 管理器 init 失败）。
- 线程池或监听端口初始化失败。

## 适合解决的问题
- SuperServer 起不来（按 init 链路快速定位）。
- 启动后能监听但无服务注册（排查 `newTCPTask/ServerTask::verifyConn`）。

## 待确认
- 历史部署中 `SuperServer` 节点数 >1 的主备策略（代码默认单记录）。建议查部署脚本与 DB 初始化SQL。

# GatewayServer 客户端连接与任务模型

## 客户端连接入口
- `GatewayService::newTCPTask(sock,addr)` 创建 `GatewayTask`，并投递到 `taskPool->addVerify()` 进入“验证队列”。

## GatewayTask 状态机
1. `verifyConn()`：先版本校验，再账号临时票据校验。
2. `waitSync()`：向 Bill 发 `t_LoginVerify_Gateway`，等待异步回调设定 `ACCOUNTVERIFY_SUCCESS/FAILURE`。
3. `addToContainer()`：构建 `GateUser`，发时间/国家信息包，进入选角。
4. 运行期 `msgParse()`：按用户命令分派到 Session/Scene/Bill/Mini/Record。
5. `recycleConn()/Terminate()`：发登出、反注册、清理用户。

## 验证队列/连接池
- `zTCPTaskPool` 容量初始化为 4096，`GatewayTaskManager` 以 `accid` 做唯一索引，避免同账号多连接。

## 断线回收
- `GatewayTask::recycleConn()` 发 `t_Logout_Gateway` 到 Bill；`Terminate()` 中触发 mini 登出、Session 反注册和本地用户清理。

## 非法连接处理
- 版本/临时登录票据不通过直接失败。
- 选角/删角等流程对越界角色、禁用国家、异常状态做告警并拒绝。
- 初始加密从 RC5 切到 DES（依据登录会话下发 key）。

## 适合解决的问题
- “握手过了但卡验证中”
- “同账号重复登录互踢”
- “断线后会话残留”

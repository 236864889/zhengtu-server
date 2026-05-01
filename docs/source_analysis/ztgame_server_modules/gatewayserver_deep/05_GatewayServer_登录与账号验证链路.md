# GatewayServer 登录与账号验证链路

## 版本验证
- `GatewayTask::verifyVersion` 仅接受 `LOGON_USERCMD/USER_VERIFY_VER_PARA`，并比较 `ptCmd->version >= verify_client_version`。

## 账号/密码/票据验证
- `verifyACCID` 读取 `stPasswdLogonUserCmd`，调用 `LoginSessionManager::verify(loginTempID,accid,...)`。
- 成功后：记录 accid/loginTempID/account，切换 DES key，回发 merge version 检查包。

## 连接登录服/Session/Bill
- 会话票据来自 Super/Gateway 协作写入 `LoginSessionManager::put`。
- `waitSync` 向 Bill 发登录验证，Bill 回包通过 `GatewayTaskManager::accountVerifyOK` 修改任务状态。

## 用户认证状态
- 状态枚举：`NONE -> WAITING -> SUCCESS/FAILURE`。
- 只有 SUCCESS 才能进入 `addToContainer` 成为正式 `GateUser`。

## 登录失败提示
- 版本错误、票据错误、国家禁登、角色非法、验证码/passport 失败等均有日志及部分客户端提示包。

## 安全风险
1. `LoginSessionManager::put` 存在双锁区间与迭代器生命周期风险（先解锁再用旧迭代器）。
2. 初始 RC5 固定 key 常量，若泄露可被重放利用。
3. 部分失败只记日志不强制惩罚，可能被高频试探。

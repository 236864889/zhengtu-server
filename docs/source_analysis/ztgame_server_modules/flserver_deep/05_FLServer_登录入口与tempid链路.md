# FLServer 登录入口与 tempid 会话链路

## 客户端登录入口
1. `LoginTask::verifyConn` 必须先收到 `USER_VERIFY_VER`。
2. `LoginTask::addToContainer` 下发验证码 JPEG。
3. `LoginTask::msgParse` 收 `USER_REQUEST_LOGIN` -> `requestLogin`。

## tempid 分配/映射
- `LoginTask::tempid = ++uniqueID`。
- `LoginManager::add/remove` 维护 `hash_map<tempid, LoginTask*>`。

## 认证与 NewSession
- `requestLogin` 做验证码、账号口令、账号状态校验（读 `ACCSTORE0000`）。
- 通过后 `LoginManager::verifyReturn`：
  - 选择可用 gateway；
  - 封装 `t_NewSession_Session` 发 Super。

## Bill/Gateway 协作
- Bill 判重失败 -> IDInUse 回 FL。
- Gateway 建立会话成功 -> NewSession 回 FL，FL 再回客户端登录成功包。

## 超时与重试
- `FLTimeTick` 每秒循环，每 10 秒执行 `LoginManager::execAll`，超时任务 `Terminate()`。
- `loginReturn(..., tm=true)` 在失败分支可触发立即回包并清理。

## 重复登录
- 主要由 Bill 侧 `BillUserManager::addUser` 判重；失败路径回 `IDINUSE`。

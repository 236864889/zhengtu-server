# FL 与 Super 通信链路

## 1) FL -> Super 请求 GYList
- `ServerTask::verifyConn` 登录内部连接成功后发送：`Cmd::FL::t_RQGYList_FL` (`PARA_FL_RQGYLIST`)。
- Super `FLClient::msgParse_gyList` 收到后转发 `Cmd::Super::t_RQGYList_Gateway`，让 Gateway 上报。

## 2) Super -> FL 回 Gateway 列表
- Gateway `notifyLoginServer()` 上报 `PARA_GATEWAY_GYLIST`(serverid/ip/port/online/state/version)。
- Super `ServerTask::msgParse_Gateway` 转成 `Cmd::FL::t_GYList_FL` 广播到 FLClient。
- FL `ServerTask::msgParse_GYList` 写入 `GYListManager::put`。

## 3) FL -> Super NewSession
- `LoginManager::verifyReturn` 拿到可用 Gateway 后构造 `Cmd::FL::t_NewSession_Session`，经 `ServerManager::sendCmd` 发给 Super。

## 4) Super/Bill/Gateway 回包
- Super `FLClient::msgParse_session` 把 `PARA_SESSION_NEWSESSION` 转 `PARA_BILL_NEWSESSION` 给 Bill。
- Bill 创建用户后给 Gateway `PARA_GATE_NEWSESSION`；Gateway 再发 Super `PARA_GATEWAY_NEWSESSION`。
- Super 转 `Cmd::FL::t_NewSession_Session` 回 FL，FL `ServerTask::msgParse_Session` 广播到对应 loginTempID。

## 5) IDInUse
- Bill 判重失败发 `PARA_BILL_IDINUSE` 到 Super。
- Super 转 `Cmd::FL::t_idinuse_Session` 到 FL 指定 wdLoginID。
- FL `LoginManager::loginReturn(loginTempID, LOGIN_RETURN_IDINUSE)` 回客户端。

## 6) 断线重连
- FL 侧 `ServerTask::recycleConn` 调 `GYListManager::disableAll`，避免使用失效网关。
- Super 侧 `FLClientManager::timeAction` 驱动重连。

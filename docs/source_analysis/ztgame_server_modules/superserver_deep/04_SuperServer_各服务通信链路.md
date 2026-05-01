# SuperServer 与各业务服务通信链路

## Gateway -> Super
- `PARA_GATEWAY_GYLIST`：上报网关IP/端口/在线数/状态，Super 转 `FLClientManager::broadcast`。
- `PARA_GATEWAY_NEWSESSION`：新登录会话，Super 转发到指定 FL 登录链路 tempid。
- `PARA_CHARNAME_GATEWAY`：角色名检测/写入/清理，Super 转 Rolereg；失败时写入 `RoleregCache`。

## FL -> Super
- `PARA_FL_RQGYLIST`：请求网关列表，Super 广播 `t_RQGYList_Gateway` 给所有 Gateway。
- `PARA_SESSION_NEWSESSION`：FL 回推新会话，Super 改写 `wdLoginID=tempid` 后广播到 Bill。

## Bill -> Super
- `PARA_BILL_IDINUSE`：账号/角色占用反馈，Super 转发到 FL 对应登录连接。

## Session/Scenes/Record/Mini -> Super
- 主要通过 `zSubNetService` 完成启动发现、ServerList 同步与通用转发。
- 其中 Session 可发 `PARA_SHUTDOWN` 到 Super，触发 `Terminate`。

## Info <-> Super
- Super 把 GM 日志/聊天/消息/处罚广播给 Info。
- Info 可请求国家在线数，Super 转发请求给 Session，再将回包转给指定 Info 连接。

## 适合解决的问题
- 登录链路“FL->Bill->FL 回执”丢包定位。
- GYList 不更新导致客户端区服列表陈旧。

## 待确认
- Mini/Record 在 Super 专有命令中的占比（当前 Super 目录未见显式 `msgParse_Mini/Record` 分支）。

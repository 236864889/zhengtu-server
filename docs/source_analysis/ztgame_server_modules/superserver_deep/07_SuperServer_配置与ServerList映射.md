# SuperServer 配置与 ServerList 映射

## 配置读取
- `main` 默认：
  - `loginServerListFile=SuperServer/loginServerList.xml`
  - `mysql=mysql://.../SuperServer`
  - `logfilename=/tmp/superserver.log`
- `zConfile::parse("SuperServer")` 读取 `<global>` 与 `<SuperServer>` 节点。
- `parseSuperServer` 将节点内容写入 `global[server]`、属性 `port` 写入 `global[port]`。

## XML 结构（被读取）
- `loginServerList.xml`：
  - `LoginServerList/server(ip,port)` -> FLClient
  - `RoleregServerList/server(ip,port)` -> RoleregClient
  - `InfoServerList/server(ip,port)` -> InfoClient

## ServerList 映射
- DB 表 `SERVERLIST` 是核心来源；`SuperService::getServerInfo` 会按 `TYPE=SUPERSERVER` 查询自身条目。
- 子服务注册时（`ServerTask::verifyConn`）同样按 `TYPE + IP` 查 `SERVERLIST` 取 `ID/PORT/EXTIP/EXTPORT`。

## 缺失表现
- 配置缺失：相关 manager init 失败，Super 启动失败。
- DB 映射缺失：可能无法给子服务返回正确 `Startup_Response`。
- DB 映射冲突：同类型多条/同IP不匹配会报错并阻断启动。

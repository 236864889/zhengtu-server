# FLServer 配置与部署关系

## 配置读取
- `zConfile` 读取 `FLServer` 配置段。
- 默认值在 `main` 写入 `Zebra::global`：
  - `login_port=7000`
  - `inside_port=7001`
  - `ping_port=7002`
  - `zoneInfoFile=zoneInfo.xml`
  - `dbAccessServerListFile=dbAccessServerList.xml`
  - `InfoServer/InfoPort`

## 端口与角色
- `login_port`：客户端登录接入。
- `inside_port`：Super/内部服务连接。
- `ping_port`：区服 ping 列表接口。

## 外网IP与服务编号
- FL 不直接决定 Gateway 外网IP；该字段由 Gateway 上报 (`pstrIP/pstrPort`) 并经 Super 转发。
- 服务编号关联通过 `wdLoginID/wdGatewayID` 在 Super/FL 间路由。

## ServerList/loginServerList
- FL 侧关键外部列表：`zoneInfo.xml`（区服ACL），`dbAccessServerList.xml`（备用 DBAccess）。
- Super 侧有 `FLServer` 节点配置（见 `SuperServer/FLClientManager.cpp`）。

## 配置缺失表现
- `zoneInfo.xml` 缺失 -> ACL 初始化失败，服务 init 失败。
- mysql 配置错误 -> 登录 DB 校验不可用。
- 端口冲突 -> bind 失败，服务无法启动。

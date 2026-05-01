# ServerList 与服务发现映射

## 核心对象
- DB 表：`SERVERLIST`
- 协议结构：`Cmd::Super::ServerEntry`
- 消费入口：`zSubNetService::addServerEntry/getServerEntryByType/getNextServerEntryByType`

## 发现链路
1. 子服务启动后连接 Super（由 `server/port`）。
2. Super 依据 `TYPE + IP` 查 `SERVERLIST` 生成注册响应（含 `ID/PORT/EXTIP/EXTPORT`）。
3. 子服务收到 `ServerEntry` 后缓存到 `serverList`。
4. Gateway/Scenes/Session 等按 `getServerEntryByType(TYPE)` 获取 Record/Bill/Scene/Mini/Session 目标。

## 字段级映射
- `TYPE`: 服务类型（SUPERSERVER/SCENESSERVER/...）
- `ID`: 服务编号（跨服路由键）
- `IP/PORT`: 内网通信
- `EXTIP/EXTPORT`: 外网公布给客户端/跨网入口

## 配置错误表现
- `TYPE` 缺失：`getServerEntryByType` 返回空，依赖服务连不上。
- `IP` 不匹配：注册失败或拿错服务条目。
- `EXTIP` 错误：客户端可见入口不可达。

## 待确认
- `SERVERLIST` 具体 schema/索引（建议查 SuperServer SQL 初始化脚本）。

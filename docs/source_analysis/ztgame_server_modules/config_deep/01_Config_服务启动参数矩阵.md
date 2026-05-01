# 服务启动参数矩阵

> 读取链路：`Zebra::initGlobal` → 服务 `main` 默认值 → `zConfile::parse("Service")` → `argp` 命令行覆盖。

| 服务 | 配置文件/段 | port/server | ifname | mysql | threadPoolCapacity/initThreadPoolState | logfilename | 读取源码位置 | 缺失表现 |
|---|---|---|---|---|---|---|---|---|
| SuperServer | 主配置 `<global>` + `<SuperServer>`；`loginServerList.xml` | 需要 | 需要 | `SuperServer`库默认URL | 使用全局 | `/tmp/superserver.log` | `SuperServer.cpp`, `zConfile.cpp` | 无法对外服务发现 |
| FLServer | `<FLServer>` + `zoneInfo.xml` + `dbAccessServerList.xml` | `login/inside/ping`三端口+Super | 需要 | 依赖 | 使用全局 | `/tmp/flserver.log` | `FLServer.cpp`,`ServerACL.cpp`,`DBAccessClientManager.cpp` | 登录/ACL失败 |
| GatewayServer | `<GatewayServer>` + `scenesinfo.xml` | Super连接 + 监听端口 | 需要 | 可配置 | `initThreadPoolState` 有，线程池容量代码硬编码 4096 | `/tmp/gatewayserver.log` | `GatewayServer.cpp` | 无法连Session/Record/Bill/Mini |
| SessionServer | `<SessionServer>` | Super连接 | 需要 | 可配置 | 使用全局 | 服务默认日志路径（待确认） | `SessionServer.cpp` | 广播/跨服链路异常 |
| ScenesServer | `<ScenesServer>` + Config/*.xml + quest.lua | Super连接 + scene监听 | 需要 | 可配置 | 使用全局 | 服务默认日志路径（待确认） | `ScenesServer.cpp` | 地图/NPC/任务系统初始化失败 |
| RecordServer | `<RecordServer>` | Super连接 | 需要 | 指向Record库 | 使用全局 | `/tmp/recordserver.log`(待确认) | `RecordServer.cpp` | 角色读写失败 |
| BillServer | `<BillServer>` + `BillServer/billServerList.xml` | Super连接 | 需要 | 默认 BillServer 库 | 使用全局 | `/tmp/billserver.log` | `BillServer.cpp` | 充值/余额链路失败 |
| MiniServer | `<MiniServer>` | Super连接 | 需要 | 默认 MiniServer 库 | 使用全局 | `/tmp/miniserver.log` | `MiniServer.cpp` | 小游戏功能失效 |

## 字段用途/覆盖顺序
- `server/port`: SuperServer 地址，`<SuperServer port="...">ip</SuperServer>` 特殊解析。
- `mysql`: 连接串，支持 `encode=yes`（base64 解码）。
- `ifname`: 本机网卡名，`getIPByIfName` 取上报 IP。
- `threadPoolCapacity`: 线程池容量（Gateway 当前实现未直接用该键）。
- `initThreadPoolState`: `run/repair/maintain`。

## RAG 标签
`startup-params`,`argp-override`,`zconfile-parse`,`global-defaults`

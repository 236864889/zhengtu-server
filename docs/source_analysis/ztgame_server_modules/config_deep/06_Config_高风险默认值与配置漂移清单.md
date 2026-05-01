# 高风险默认值与配置漂移清单

## 默认值风险
- `server=192.168.8.129`, `port=10000`, `ifname=eth0`, `mysql=.../TxGameDB`（base 全局默认）。
- 服务级默认：Bill/Mini/Super/Gateway/FL 的日志与库地址硬编码到 192.168.* 和 `/tmp`。

## 漂移场景
1. 机房迁移后只改了 `<global><mysql>`，漏改服务段 `mysql`。
2. 新网卡名 `ens*` 未同步 `ifname`。
3. Gateway 线程池容量参数与实现不一致（4096硬编码）。
4. DB `SERVERLIST` 与 XML `loginServerList/zoneInfo` 不一致。

## 运维建议
- 把 `server/port/mysql/ifname` 作为变更单强校验项。
- 每次发版比对配置快照（hash+diff）。

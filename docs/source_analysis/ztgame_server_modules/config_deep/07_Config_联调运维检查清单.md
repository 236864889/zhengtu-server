# 配置联调与运维检查清单

## 启动前检查
- 核对 `server/port/ifname/mysql/logfilename/threadPoolCapacity`。
- 校验所有服务段是否存在。

## DB 可连性检查
- 按服务 `mysql://` 逐一连通测试。
- 校验目标库是否含服务关键表（Record/Bill/Session 等）。

## 端口占用检查
- FL 三端口 + Super 端口 + 各业务服务监听端口。

## SERVERLIST 一致性
- `TYPE-ID-IP-PORT-EXTIP-EXTPORT` 完整且唯一。
- 与部署清单一一对应。

## 服务注册检查
- 子服务启动日志中是否收到 Super 注册返回。
- `getServerEntryByType` 是否可取到目标服务。

## 客户端链路检查
- `loginServerList.xml`、`zoneInfo.xml`、Gateway 场景信息一致。

## 资源检查
- `scenesinfo.xml`、地图/NPC/技能/任务、`newquest/quest.lua` 文件完整可读。

## 日志路径检查
- `/tmp` 或自定义路径写权限、滚动策略、磁盘配额。

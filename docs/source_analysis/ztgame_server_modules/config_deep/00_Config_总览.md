# Config 深挖总览（字段级统一映射）

## 配置目录真实职责边界
- `ztgame/Config/` 主要承载“玩法/系统参数 XML”，不是主启动入口配置（主启动入口在 `ztgame/*.xml` 与服务目录内 XML）。
- 启动级全局参数由 `zConfile::parse(Zebra)` + `<global>` + `<ServiceName>` 段写入 `Zebra::global`。见 `ztgame/base/zConfile.cpp`。
- 默认值来源有三层：`Zebra::initGlobal`（通用默认）→ 各服务 `main` 内默认赋值 → 配置文件覆盖 → 命令行覆盖。

## Config 文件清单
`ztgame/Config/` 共 16 文件（含 README），核心 15 个 XML：
- 12BeastsConfig.xml, BieshuConfig.xml, ChenghaoConfig.xml, ErnvConfig.xml, FuBenConfig.xml
- GameConfig.xml, HuishouConfig.xml, JingMaiConfig.xml, Mall.xml, MessageSystem.xml
- ShenJianConfig.xml, ShengQiConfig.xml, TouxianConfig.xml, XiulianConfig.xml, ZhengTuZhuanConfig.xml

## 顶层 XML/配置清单（maxdepth=2）
- `ztgame/scenesinfo.xml`, `ztgame/zoneInfo.xml`, `ztgame/skilltable.xml`, `ztgame/npctrade.xml`
- `ztgame/quest/quests.xml`
- `ztgame/Config/*.xml`（上表）

## 被哪些服务读取（统一结论）
- `ScenesServer`: GameConfigMgr/MessageSystem/Quest/Lua 脚本桥接，强依赖 `Config/*.xml` 与 `newquest/quest.lua`。
- `GatewayServer`: `sceneinfofile`（默认 `scenesinfo.xml`）。
- `FLServer`: `zoneInfo.xml`, `dbAccessServerList.xml`。
- `SuperServer`: `loginServerList.xml`。
- `BillServer`: `BillServer/billServerList.xml`。
- 各 SubNet 服务（Gateway/Session/Scenes/Record/Bill/Mini/FL）依赖 `server/port/ifname/mysql/threadPoolCapacity` 等全局键。

## 最重要配置 Top30（字段）
`mysql,server,port,ifname,threadPoolCapacity,initThreadPoolState,log,logfilename,daemon,`
`login_port,inside_port,ping_port,sceneinfofile,zoneInfoFile,dbAccessServerListFile,`
`loginServerListFile,billServerList,jpeg_passport,rolereg_verify,cmdswitch,merge_version,`
`maxGatewayUser,InfoServer,InfoPort,countryorder,mapdir,newquest/quest.lua,`
`ServerEntry.extip,ServerEntry.extport,SERVERLIST.TYPE`

## 最高风险配置 Top15
1. `mysql`（连错库/错实例）
2. `server`/`port`（Super 地址漂移）
3. `ifname`（网卡名在容器/云主机不一致）
4. `threadPoolCapacity`
5. `initThreadPoolState`
6. `logfilename`（路径权限）
7. `sceneinfofile`
8. `zoneInfoFile`
9. `dbAccessServerListFile`
10. `loginServerListFile`
11. `billServerList`
12. `cmdswitch`
13. `jpeg_passport`
14. `merge_version`
15. `SERVERLIST` 表内网/外网端口映射

## RAG 入库优先级
- S: `01/02/03/05/06/07`
- A: `00/04/09/10`
- B: `08`（manifest 元数据索引）

## 适合解决的问题
- 某服务启动后无法注册到 Super 的根因定位。
- 同服多机部署时内外网端口错配定位。
- 数据库连错库/读错表的快速排查。

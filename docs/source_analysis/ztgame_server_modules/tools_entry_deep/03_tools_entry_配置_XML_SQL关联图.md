# tools 配置 / XML / SQL 关联图（静态）

## 总体结论
- tools 主要是“命令行参数 + `Zebra::global` 全局变量 + 工具私有 XML/地图文件”驱动。
- 本目录入口未发现直接执行 SQL 字符串（SELECT/INSERT/UPDATE/DELETE）的代码路径。
- 但客户端类工具会通过网络协议触达服务端，服务端可能进一步写库（间接 DB 风险）。

## 关联图（文字版）
1. `zebraclientService.cpp` / `skill_test_client.cpp`
   - `zArg::parse` -> 写入 `Zebra::global[...]`
   - `ArrayTable::load` -> `zXMLParser.initFile(Zebra::global["skilltable"])` -> 读取 `skilltable.xml`
   - `LoadMap` -> 读取 `mapfile.mps`（默认 `qingyuancun-main.mps`）
   - `loginClient/zebraclient` -> 发登录/角色/行为协议到 server:port
2. `zntop.cpp`
   - `zArg::parse` -> `outfile/interval`
   - `CollectServerInfo` -> 读取 `/proc/stat`、`/proc/net/dev` -> 组装 XML 字符串
   - 可选写 `outfile`（追加）
3. `base64.cpp` / `datetime.cpp`
   - 无 XML/SQL/DB 依赖链

## 文件级映射
- XML 读取：`skilltable.xml`（`ArrayTable.cpp`）。
- XML 生成：`CollectServerInfo.cpp`（构造 `<Server>` 节点并 dump 到内存字符串）。
- 地图资源读取：`LoadMap.cpp`（二进制 `fopen(...,"rb")` 读取 `.mps`）。
- SQL：tools 入口代码未见直接 SQL 语句。

## 是否触达数据库
- 直接触达：未发现 `mysql query`/SQL 字符串执行。
- 间接触达：`zebraclient*` 通过登录/角色协议调用服务端，服务端可能落库（高风险行为链）。

## 是否生成/修改配置
- 未见回写 `skilltable.xml` / `ztgame/Config/*.xml` 的代码路径。
- 但可写监控输出文件（`zntop outfile`），并可能在日志系统中产生日志文件。

## 待确认
- `ro.txt` 与 `testGame` 的生产链关系（源码未定位直接引用）。

# tools 深挖总览（函数级/链路级）

## 1) tools 真实职责边界
- `ztgame/tools/` 在当前仓库中不是“表转换/数据库迁移”中台，而是**测试客户端 + 运维观测小工具 + 若干独立实验程序**集合。
- 其主产物由 `Makefile` 明确为 `zntop`、`zebraclientService`、`datetime` 三个可执行程序；`base64` 规则存在但不在 `ALL_TARGETS` 默认编译目标中。
- 未发现 `*.sql/*.pl/*.py/*.sh`，说明当前 `tools` 不承担典型 SQL 迁移脚本和 shell 运维脚本职责。

## 2) 目录结构与文件类型
- 文件总数：34（`find ... -type f` 结果）。
- 类型统计：`cpp(12)`, `h(9)`, `d(3)`, `xml(2)`, `mps(1)`, `txt(1)`, `md(1)`, 以及 4 个无扩展名可执行/样例文件。
- 关键资产：
  - 源码：`zntop.cpp`、`zebraclientService.cpp`、`zebraclient.cpp`、`skill_test_client.cpp`、`CollectServerInfo.cpp` 等。
  - 样例资源：`skilltable.xml`、`qingyuancun-main.xml`、`qingyuancun-main.mps`。
  - 历史构建依赖文件：`.zntop.d`、`.CollectServerInfo.d`、`.zebraclientService.d`（编译机路径泄露风险）。

## 3) 工具清单（按可执行/核心模块）
- `zntop`：终端监控 + 服务器信息采集，可写出统计文件（`outfile`）。
- `zebraclientService`：批量登录压力/行为客户端服务，读取地图和技能配置，连接登录/网关。
- `datetime`：时间戳转换/结构体尺寸小工具。
- `base64`：命令行 base64 编码工具（默认未纳入 all）。
- `skill_test_client.cpp` / `testGame`：技能/地图场景联调客户端（偏手工测试）。
- `CollectServerInfo.*`：采集 `/proc/stat` `/proc/net/dev` / `sysinfo` 并组装 XML。
- `LoadMap.*`：读取 `.mps` 地图二进制到内存 tile 阵列。
- `ArrayTable.*`：解析 `skilltable.xml`，抽取 Skill/Timer/Global 节点供客户端行为控制。

## 4) 输入输出关系（跨目录）
- 输入：
  - `tools/skilltable.xml`（默认技能表）
  - `tools/qingyuancun-main.mps`（默认地图）
  - 运行参数/`Zebra::global`（server/port/user/passwd/zone/count/mapfile 等）
  - Linux `/proc` 与 `sysinfo` 系统信息。
- 输出：
  - `zntop` 可附加写文件（`fopen(..., "a+")`）。
  - 其余以网络消息发包、终端输出、日志输出为主；未见本目录下持久化 SQL 写库逻辑。

## 5) 与主启动链路关系
- `tools` 不在服务主链（Gateway/Scenes/FL/Super/Bill）启动依赖内。
- 其主要作用是“联调/压测/观测”外围工具链；运行失败不阻塞主服务启动。

## 6) 最高风险工具 Top15
1. `zebraclientService.cpp`（批量账号登录+并发连接，易触发封禁/压垮测试服）
2. `zebraclient.cpp`（自动行为与协议发包，可能对在线环境造成副作用）
3. `skill_test_client.cpp`（含 GM 指令发送与地图行为）
4. `loginClient.cpp`（真实登录握手、账号密码直连）
5. `zntop.cpp`（可能写出文件，路径配置不当会覆盖/膨胀日志）
6. `CollectServerInfo.cpp`（系统信息暴露，隐私/资产信息泄漏）
7. `LoadMap.cpp`（不校验 magic/version 的代码段被注释，脏输入风险）
8. `ArrayTable.cpp`（强依赖 XML 结构，异常处理偏弱）
9. `Makefile`（依赖旧 GCC/mysql_config/xml2-config/log4cxx 环境）
10. `.CollectServerInfo.d`（泄露编译机 include 路径）
11. `.zebraclientService.d`（同上）
12. `.zntop.d`（同上）
13. `testGame`（二进制来源不明，历史残留风险）
14. `zebraclientService`（二进制来源不明）
15. `zntop`（二进制来源不明）

## 7) RAG 入库优先级
- S：`zebraclientService.cpp`, `zebraclient.cpp`, `loginClient.cpp`, `zntop.cpp`, `CollectServerInfo.cpp`, `Makefile`
- A：`ArrayTable.cpp`, `LoadMap.cpp`, `skill_test_client.cpp`, `README.md`
- B：`*.d`, `ro.txt`, 无扩展名二进制

## 8) 适合解决的问题
- “如何做登录链路压测、并发虚拟用户行为模拟？”
- “如何在老服环境快速采集 CPU/网卡信息并终端可视化？”
- “tools 是否会直接改库/改配置/删文件？”
- “哪些工具只能在隔离测试网运行？”

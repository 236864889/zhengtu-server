# 附录C_Codex_Claw任务模板

## 1) 编译失败排查任务
- 当前状态：某模块编译/链接失败，需只读排查。
- 禁止事项：禁止改源码、禁止编译执行、禁止启动服务。
- 允许事项：读取日志、Makefile、历史修复文档。
- 要读取的文件：`ztgame/Makefile`、`docs/rag_sources/02_编译顺序与产物.md`、`docs/rag_sources/10_已修复编译问题记录.md`。
- 要执行的只读命令：`cat`、`rg`、`find`（仅查询）。
- 输出要求：给出失败阶段、首个错误点、候选根因、人工复核项。

## 2) base 编译失败任务
- 当前状态：`base` 相关报错。
- 禁止事项：禁止直接打补丁到源码。
- 允许事项：仅做证据归档与修复建议。
- 要读取的文件：`docs/rag_sources/10_已修复编译问题记录.md`、`ztgame/base` 下相关文件（只读）。
- 要执行的只读命令：`cat`、`rg "zMisc|zSocket|zLogger|zMysqlDBConnPool" ztgame/base`。
- 输出要求：映射到已知修复项，标记是否复发。

## 3) ScenesServer 编译失败任务
- 当前状态：`ScenesServer` 编译或链接失败。
- 禁止事项：禁止重构构建系统。
- 允许事项：核对静态库依赖链与路径。
- 要读取的文件：`docs/rag_sources/02_编译顺序与产物.md`、`ztgame/Makefile`。
- 要执行的只读命令：`cat`、`rg "liblua|luabind|ssl|zebra" ztgame/ScenesServer`。
- 输出要求：指出缺失库/错误符号/路径不一致点。

## 4) 服务启动失败任务
- 当前状态：服务按脚本启动失败或异常退出。
- 禁止事项：禁止实际启动、禁止 kill 进程。
- 允许事项：比对启动脚本与日志线索。
- 要读取的文件：`ztgame/start.sh`、`ztgame/start1.sh`、`ztgame/stop.sh`、`docs/rag_sources/03_服务端启动顺序.md`。
- 要执行的只读命令：`cat`、`rg "Super|FL|Gateway|kill -9" ztgame/*.sh`。
- 输出要求：给出基线A/B差异、风险点、建议启动基线。

## 5) 登录失败任务
- 当前状态：`login failed/timeout`。
- 禁止事项：禁止改协议与数据库结构。
- 允许事项：读取账号链路分析材料。
- 要读取的文件：`docs/rag_sources/06_数据库与账号体系.md`、`docs/source_analysis/ztgame_dat/runtime_validation/`、`docs/source_analysis/launcher_reanalysis/`。
- 要执行的只读命令：`cat`、`rg "login|timeout|DBAccess|Session" docs/source_analysis`。
- 输出要求：给出链路断点假设和最小验证顺序。

## 6) 配置漂移任务
- 当前状态：环境配置与文档不一致。
- 禁止事项：禁止直接覆盖线上配置。
- 允许事项：做差异盘点与风险分类。
- 要读取的文件：`docs/rag_sources/05_配置文件总览.md`、相关 XML 清单。
- 要执行的只读命令：`find ztgame -maxdepth 2 -name "*.xml"`、`cat`。
- 输出要求：漂移项列表（缺失/新增/命名变更）。

## 7) 客户端登录器联调任务
- 当前状态：客户端或登录器联调受阻。
- 禁止事项：禁止运行未知二进制。
- 允许事项：读取 runtime_validation 与 launcher_reanalysis 证据。
- 要读取的文件：`docs/source_analysis/ztgame_dat/runtime_validation/`、`docs/source_analysis/launcher_reanalysis/`。
- 要执行的只读命令：`cat`、`rg "launcher|runtime|P0_" docs/source_analysis`。
- 输出要求：联调前置条件、链路核对表、人工确认项。

## 8) 教程整理/手册修订任务
- 当前状态：需更新 SOP 文档。
- 禁止事项：禁止把 C/D 教程写入主流程。
- 允许事项：仅更新 `docs/operations/`。
- 要读取的文件：`docs/source_analysis/tutorials_inventory/04_教程质量分级与整理建议.md`、`05_最终教程手册结构建议.md`、`docs/operations/review/*.md`。
- 要执行的只读命令：`cat`、`rg`。
- 输出要求：修订清单、变更理由、仍待确认项。

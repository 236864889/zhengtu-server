# 附录C：Codex/Claw 任务模板

## 模板1：编译失败排查任务
- 当前状态：`make` 失败，需定位首个报错。
- 禁止事项：不改协议/数据库/登录器，不执行危险写操作。
- 允许事项：读取 `Makefile`、日志、依赖检查命令。
- 要读取的文件：`ztgame/Makefile`、对应模块 `Makefile`、`docs/rag_sources/10_已修复编译问题记录.md`。
- 要执行的只读命令：`make 2>&1 | tee build.log`、`tail -n 200 build.log`。
- 输出要求：首错位置、根因判断、最小修复建议、回滚点。

## 模板2：base 编译失败任务
- 当前状态：`make -C base` 失败。
- 禁止事项：不跳过错误、不直接替换第三方库。
- 允许事项：只读检查头文件、编译器、已修复记录。
- 要读取的文件：`ztgame/base/*`、`docs/rag_sources/10_已修复编译问题记录.md`。
- 要执行的只读命令：`make -C base 2>&1 | tee base_build.log`。
- 输出要求：是否命中历史问题、修复优先级、影响范围。

## 模板3：ScenesServer 编译失败任务
- 当前状态：ScenesServer 链接失败。
- 禁止事项：不删除 Lua 功能、不改业务逻辑规避。
- 允许事项：检查 `script` 静态库与 `ScenesServer/Makefile`。
- 要读取的文件：`ztgame/ScenesServer/Makefile`、`ztgame/script/*`、`docs/rag_sources/02_编译顺序与产物.md`。
- 要执行的只读命令：`ls ztgame/script/*.a`、`grep -n 'llua\|lluabind' ztgame/ScenesServer/Makefile`。
- 输出要求：缺失库/顺序/ABI 三类结论与建议。

## 模板4：服务启动失败任务
- 当前状态：启动链路中断。
- 禁止事项：不直接全量 kill -9 线上进程。
- 允许事项：读取启动脚本、日志、进程/端口状态。
- 要读取的文件：`ztgame/start.sh`、`ztgame/start1.sh`、`ztgame/stop.sh`、配置 XML。
- 要执行的只读命令：`ps -ef | egrep 'Server'`、`ss -lntp`、`tail -n 200 /tmp/*.log`（若存在）。
- 输出要求：首个失败服务、前置依赖、修复与回滚步骤。

## 模板5：登录失败任务
- 当前状态：客户端无法登录。
- 禁止事项：不做协议注入/抓包伪造。
- 允许事项：核对配置与服务状态。
- 要读取的文件：`config.ini`、`servers.json`、runtime_validation 文档。
- 要执行的只读命令：服务进程与端口检查、日志关键字检索。
- 输出要求：配置差异点、服务链路断点、最小修复路径。

## 模板6：配置漂移任务
- 当前状态：环境间配置不一致。
- 禁止事项：不覆盖生产配置。
- 允许事项：只读 diff 与备份核对。
- 要读取的文件：`ztgame/Config/*.xml`、顶层 XML、备份目录。
- 要执行的只读命令：`diff -ruN <envA_config> <envB_config>`（只读）。
- 输出要求：漂移清单、风险等级、回滚建议。

## 模板7：客户端登录器联调任务
- 当前状态：登录器与客户端联调异常。
- 禁止事项：不改 `ztgame.dat` 源码、不改协议算法实现。
- 允许事项：读取 runtime_validation 与 launcher_reanalysis 文档，核对配置。
- 要读取的文件：`docs/source_analysis/ztgame_dat/runtime_validation/*.md`、`docs/source_analysis/launcher_reanalysis/*.md`。
- 要执行的只读命令：配置文件检查、日志检查、端口连通检查。
- 输出要求：联调路径图、断点、建议验证顺序、人工复核项。

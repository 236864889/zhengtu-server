# 附录B_常见错误速查表

| 错误关键词 | 所属阶段 | 优先检查文件/命令 | 可能原因 | 建议处理 | 关联手册章节 | 是否需要人工复核 |
|---|---|---|---|---|---|---|
| mysql_config: command not found | 依赖检查 | `which mysql_config` | MySQL 开发包未安装或 PATH 缺失 | 补齐开发包与 PATH，再复核依赖清单 | 主手册 §6 | 是 |
| xml2-config: command not found | 依赖检查 | `which xml2-config` | libxml2-devel 缺失 | 安装开发包并复核命令可用性 | 主手册 §6 | 是 |
| cannot find -llog4cxx | 链接阶段 | `ldconfig -p \| grep log4cxx` | log4cxx 库缺失或版本不兼容 | 对齐 log4cxx 版本，复核 zLogger 兼容补丁前提 | 主手册 §7 | 是 |
| undefined reference to lua/luabind | ScenesServer 链接 | 检查 `script/liblua.a` `libluabind.a` | script 静态库缺失/路径错误 | 先核实 script 库存在，再核对链接参数 | 主手册 §2/§7 | 是 |
| No such file *.xml | 启动/加载配置 | 检查 `Config/*.xml`、根目录 XML | 配置文件缺失/路径错误/权限不足 | 回到配置清单逐项补齐并校验可读性 | 主手册 §6 | 是 |
| bind: Address already in use | 启动阶段 | `ss -lntp` | 端口被占用或残留进程 | 查占用进程并按变更流程释放端口 | 主手册 §3/§9 | 是 |
| Segmentation fault | 运行阶段 | 服务日志、核心转储开关 | 空指针/配置异常/链接不一致 | 先定位首个报错模块，回溯最近改动 | 主手册 §7 | 是 |
| connect mysql failed | 运行阶段 | DB 配置文件、网络连通性检查 | 地址/账号/权限/网络策略错误 | 对齐 DB 权威来源，逐项核查参数 | 主手册 §6/§9 | 是 |
| login failed / timeout | 联调阶段 | Gateway/Session/FL 日志 | 链路顺序错误、账号链路异常 | 按双基线确认顺序并核查账号链路 | 主手册 §3/§9 | 是 |
| base 编译失败 | 编译阶段 | `base/` 相关报错、修复记录 | 历史兼容问题未收敛 | 参考已修复记录逐项对照（zMisc/zSocket/zLogger 等） | 主手册 §2/§7 | 是 |
| ScenesServer 编译失败 | 编译阶段 | `ScenesServer/Makefile` 与链接报错 | Lua/luabind 或 SSL/基础库链异常 | 优先校验 script 静态库与基础库完整性 | 主手册 §2/§7 | 是 |
| 服务启动即退出 | 启动阶段 | 启动脚本与首屏日志 | 缺配置、缺库、顺序不满足 | 先做路径/配置/库存在性三检查，再按基线重排 | 主手册 §3/§6/§7 | 是 |

# 入口总表（server + test + tools + launcher + ztgame.dat）

| 入口类型 | 路径 | 入口函数/脚本 | 参数来源 | 输入 | 输出 | 副作用 | 风险等级 | 对应 deep 文档 | 后续动作 |
|---|---|---|---|---|---|---|---|---|---|
| server 启动脚本 | `ztgame/start.sh` | bash 顺序拉起 | 脚本硬编码 | 本地二进制、配置文件 | 多进程启动 | 清理 `/tmp/*.log*`，直接启动进程 | 高 | `tools_entry_deep/01/04` | 仅在沙箱做 dry-run 语义改造 |
| server 启动脚本 | `ztgame/start1.sh` | bash 顺序拉起（当前目录） | 脚本硬编码 | 同上 | 同上 | 同上 | 高 | `tools_entry_deep/01/04` | 建立生产基线差异表 |
| server 停服脚本 | `ztgame/stop.sh` | `ps|grep|kill -9` | 脚本硬编码 | 进程名匹配 | 强杀进程 | 误杀/数据一致性风险 | 高 | `tools_entry_deep/04` | 设计替代停服策略（TERM+wait） |
| server 主构建入口 | `ztgame/Makefile` | `all/debug/test` | make target | 子目录源码 | 二进制 | 默认含 `tools`，不含完整 test 运行保护 | 中高 | `test_deep/01` `tools_entry_deep/02` | 保持静态分析，不执行 make |
| test 入口 | `ztgame/test/*` | 各样例 main | Makefile/test 目录 | 本地配置/DB/网络 | 样例行为 | 可能写 MySQL、网络通信 | 高 | `test_deep/00~06` | 增加 dry-run 包装层 |
| tools 入口 | `ztgame/tools/*` | Makefile targets 与脚本 | 参数/配置/SQL/XML | 文件、库、表 | 转换/写入/发布动作 | 高副作用工具混合 | 高 | `tools_deep/*` `tools_entry_deep/*` | 分级白名单、只读探测子集 |
| tools 默认目标 | `ztgame/tools/Makefile` | `zntop zebraclientService datetime` | make 默认 | 源码与依赖 | 工具二进制 | `zebraclientService` 风险高 | 高 | `tools_entry_deep/02/04` | 默认目标拆分 |
| tools 非默认目标 | `ztgame/tools/Makefile` | `base64`/`skill_test_client` | 手动 target | 源码/网络参数 | 工具行为 | `skill_test_client` 高风险 | 高 | `tools_entry_deep/01/08` | 加 guard + dry-run |
| 登录器入口 | `docs/source_analysis/launcher_reanalysis/*` 对应源码路径 | 登录器主程序/配置驱动 | ini/xml/patch 配置 | 账号服地址、版本信息 | 登录/更新流程 | 联调失败会阻断登录链 | 中高 | `launcher_reanalysis/08~10` | 仅做只读配置核对 |
| ztgame.dat 启动入口 | `docs/source_analysis/ztgame_dat/01/07` 提及路径 | 客户端 exe 启动链 | config.ini/资源包 | 客户端资源与服务端地址 | 客户端登录与渲染 | 资源/协议不一致风险 | 中高 | `ztgame_dat/01~10` | 走 runtime_validation 方案但暂不执行 |

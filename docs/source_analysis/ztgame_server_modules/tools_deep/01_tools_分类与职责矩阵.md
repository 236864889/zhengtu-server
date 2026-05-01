# tools 分类与职责矩阵

| 工具路径 | 类型 | 输入 | 输出 | 是否危险 | 是否可运行 | 依赖环境 | 关联服务/目录 |
|---|---|---|---|---|---|---|---|
| ztgame/tools/zntop.cpp | 运维观测 | `/proc/*`、`Zebra::global` | 终端监控、可选 outfile 写文件 | 中高 | 可编译运行（旧环境） | Linux + ncurses + libxml2 + log4cxx | 运维侧；不进主服务链 |
| ztgame/tools/CollectServerInfo.cpp | 运维采集库 | `sysinfo/uname/proc/net` | XML 字符串 | 中 | 被 zntop 调用 | Linux procfs | zntop 上游 |
| ztgame/tools/zebraclientService.cpp | 压测/联调 | server/port/user/passwd/zone/count/mapfile/skilltable | 网络发包、登录行为 | 高 | 可运行（网络依赖强） | 登录服/网关可达、旧协议 | Login/Gateway（外部交互） |
| ztgame/tools/zebraclient.cpp | 协议客户端核心 | 登录返回、地图数据、技能表 | 自动行为发包 | 高 | 可运行（依赖服务） | RC5/协议兼容 | Login/Gateway/Scene 协议链 |
| ztgame/tools/loginClient.cpp | 登录握手 | 账号口令 | 网关地址/临时令牌/密钥 | 高 | 可运行（依赖服务） | 网络 + 协议 | LoginServer |
| ztgame/tools/skill_test_client.cpp | 联调客户端 | skilltable/mapfile | 技能/移动/GM 行为流 | 高 | 可运行（依赖服务） | 地图+协议 | Scene/Gateway |
| ztgame/tools/ArrayTable.cpp | 配置解析 | `skilltable.xml` | Skill/Timer/Global 内存表 | 中 | 可运行 | XML 结构稳定性 | tools 内部配置层 |
| ztgame/tools/LoadMap.cpp | 资源读取 | `.mps` | tile 内存网格 | 中 | 可运行 | 地图格式一致性 | tools 客户端地图层 |
| ztgame/tools/base64.cpp | 构建辅助/小工具 | argv 文本 | base64 编码输出 | 低 | 可运行 | base 库 | 独立 |
| ztgame/tools/datetime.cpp | 构建辅助/诊断 | 时间戳输入 | 日期格式输出 | 低 | 可运行 | C++ runtime | 独立 |
| ztgame/tools/Makefile | 构建辅助 | cpp/h + base 库 | 二进制 | 中 | 依赖旧编译链 | gcc/mysql/xml2/log4cxx/gd/jpeg | tools 全体 |
| ztgame/tools/*.d | 历史残留 | 编译依赖信息 | 无 | 中（信息泄露） | 不建议运行 | 历史构建机路径 | 构建历史 |
| ztgame/tools/testGame 等无扩展文件 | 历史残留二进制 | 未知 | 未知 | 高（来源不明） | 待确认 | 对应运行时库 | 待确认 |

## 分类结论
- 构建辅助：`Makefile`, `base64.cpp`, `datetime.cpp`
- 表转换：仅 `ArrayTable.cpp`（XML→内存数组，不落地转换文件）
- 数据库脚本：**未发现**
- 配置生成/检查：`ArrayTable.cpp`（解析检查型）
- 资源处理：`LoadMap.cpp`（`.mps` 读取）
- 日志分析：未见离线日志解析器
- 运维脚本：无 shell 脚本；以 C++ 程序 `zntop` 实现
- 历史残留：`.d` + 无扩展二进制

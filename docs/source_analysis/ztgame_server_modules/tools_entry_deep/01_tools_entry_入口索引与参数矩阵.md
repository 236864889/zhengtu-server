# tools 入口索引与参数矩阵（静态）

| 工具/目标名 | 入口文件 | main/函数 | 参数来源 | 输入文件 | 输出文件 | 是否读库 | 是否写库 | 是否写文件 | 是否建议直接运行 | 风险等级 |
|---|---|---|---|---|---|---|---|---|---|---|
| zntop | `ztgame/tools/zntop.cpp` | `main(argc, argv)` | `zArg + argp` (`-o/--outfile`,`-i/--interval`等) | `/proc/stat`, `/proc/net/dev`（经 `CollectServerInfo`） | 可选 `outfile` 追加写 | 否 | 否 | 是（`fopen(...,"a+")`） | 不建议直接运行（先加 dry-run 和输出沙箱） | 中 |
| zebraclientService | `ztgame/tools/zebraclientService.cpp` | `main(argc, argv)` | `zArg + argp`（server/port/user/passwd/count/cacc/change/role 等） | `skilltable.xml`, `*.mps`，网络服务端 | 日志/网络状态副作用 | 否（代码中未见 SQL） | 间接高风险（通过协议触发服务端写库） | 可能（日志） | 不建议直接运行 | 高 |
| skill_test_client | `ztgame/tools/skill_test_client.cpp` | `main(argc, argv)` | `zArg + argp`（与 zebraclientService 类似） | `skilltable.xml`, `*.mps`，网络服务端 | 日志/网络状态副作用 | 否（本地） | 间接高风险（服务端侧） | 可能（日志） | 不建议直接运行 | 高 |
| base64 | `ztgame/tools/base64.cpp` | `main(argc, argv)` | `zArg + argp`（method/key/source） | 命令行文本参数 | stdout | 否 | 否 | 否 | 可在隔离环境运行 | 低 |
| datetime | `ztgame/tools/datetime.cpp` | `main()` | 无命令行解析 | 无 | stdout | 否 | 否 | 否 | 可在隔离环境运行 | 低 |

## 参数备注
- `zebraclientService`/`skill_test_client` 默认内置了历史内网地址与账号相关默认值（如 `192.168.*`、`user/passwd/count`），存在“未显式指定参数即误连目标环境”的风险。
- 两者均支持账号创建/改密/角色流程切换参数，属于“强副作用入口”。

## 适合 Claw / 服务器 dry-run 的候选
- 最高优先：`base64`、`datetime`（副作用低，适合先打通 dry-run 规范）。
- 次优先：`zntop`（可做只读监控 dry-run，禁写文件）。
- 最后：`zebraclientService`、`skill_test_client`（需先做网络隔离、禁操作子命令、参数白名单）。

## 待确认
- `zebraclient.cpp` 内部 `zebraClient::main()` 属业务流程函数，不是进程入口；需后续在服务端协议文档中对齐命令行为语义。

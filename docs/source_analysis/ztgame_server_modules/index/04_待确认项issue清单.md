# 待确认项 Issue 清单

| Issue ID | 问题 | 涉及路径 | 为什么不确定 | 下一步查哪个文件 | 是否需要服务器实测 | 优先级 |
|---|---|---|---|---|---|---|
| IDX-001 | UserServer 是否部署必需 | `ztgame/` 与手册 | 仓库缺目录但历史材料提及 | `docs/operations/*`、外部部署包清单 | 是 | P0 |
| IDX-002 | testGame 来源 | `test_deep` 相关结论 | 当前仅看到引用，未见完整来源链 | `ztgame/test` + 历史教程映射 | 否 | P1 |
| IDX-003 | ro.txt 消费链 | tools/test | 被提及但消费入口未闭环 | `ztgame/tools`、`test_deep/09` | 否 | P1 |
| IDX-004 | tools 协议只读探测命令是否存在 | `ztgame/tools` | 尚未形成白名单命令清单 | `tools_entry_deep/01/05` + tools 源码 | 否 | P0 |
| IDX-005 | start.sh/start1.sh 生产基线 | `ztgame/start*.sh` | 两脚本路径与调用方式不同 | 手册与发布脚本 | 是 | P0 |
| IDX-006 | stop.sh 替代停服策略 | `ztgame/stop.sh` | 当前仅 `kill -9`，缺优雅停服 | 运维脚本目录与手册 SOP | 是 | P0 |
| IDX-007 | 配套官网安全外置是否完成 | `tutorials_inventory/review/*` | 静态复核有建议但未见闭环状态 | `review/01/02/03` 后续记录 | 否 | P0 |
| IDX-008 | tutorials_inventory/07~50 是否落地 | `tutorials_inventory/07~50` | 模板存在，执行落地状态未知 | `docs/operations/review/*` | 否 | P1 |

# 下一步建议：进入“源码分析总索引与查漏补缺”

## 建议结论
- 建议下一轮进入：**源码分析总索引与查漏补缺**。

## 原因
1. `core service deep`、`test_deep`、`tools_entry_deep` 三个视角已形成。
2. 当前缺口不再是“局部模块”，而是“跨目录可检索总索引与一致性校验”。
3. 后续若做 Claw 自动化，需要统一索引支持：
   - 入口 -> 参数 -> 输入输出 -> 风险 -> 回滚 的闭环映射；
   - 避免同一风险在 test/tools/server 文档中结论不一致。

## 建议输出物（下一轮）
- `docs/source_analysis/ztgame_server_modules/index/00_源码分析全局索引.md`
- `docs/source_analysis/ztgame_server_modules/index/01_入口总表（server+test+tools）.md`
- `docs/source_analysis/ztgame_server_modules/index/02_风险总表与优先级.md`
- `docs/source_analysis/ztgame_server_modules/index/03_dry_run改造全局路线图.md`

## 本轮到下一轮的衔接点
- 直接复用本轮 `01/04/05/06/07`：入口矩阵、风险矩阵、dry-run 建议、RAG manifest、关键词词库。
- 对 `待确认` 项建立 issue 清单：`testGame` 来源、`ro.txt` 消费链、协议只读探测命令可用性。

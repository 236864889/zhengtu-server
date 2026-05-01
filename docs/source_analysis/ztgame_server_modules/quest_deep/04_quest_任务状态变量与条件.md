# 任务状态、变量与条件
- 状态主键：`Quest::FINISHED_NAME = "state"`。
- 常见变量：`kill_*`、`visit_*`（在 XML 中通过 `<var>` 累加/比较）。
- 常见操作：`<var type="Add|Less|Great" .../>`、`<refresh name="..."/>`。
- 击杀计数：`on_kill/*.xml` 中对 `kill_xxx` 加1并刷新，达到阈值触发后续动作。
- 访问变量：`on_visit/*.xml` 常与任务阶段状态推进绑定。

## 条件类型
- 等级：`on_kill_by_level/*.xml`。
- 道具：`on_use/*.xml` 与 `on_get/*.xml`。
- 职业/国家：在 source 条件分支中（需专题提取）。

## 失败提示
- 依赖 source 中 message/notice 节点（建议后续批量抽取字段词频）。

RAG标签：`quest-state`, `vars`, `kill-counter`, `refresh`。
适合问题：任务为什么不推进。

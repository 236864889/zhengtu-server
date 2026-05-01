# source XML 与事件映射
## 目录到事件映射
- `on_visit/*.xml` ↔ `event type="on_visit"`
- `on_kill/*.xml` ↔ `event type="on_kill"`
- `on_use/*.xml` ↔ `event type="on_use"`
- `on_get/*.xml` ↔ `event type="on_get"`
- `on_kill_by_level/*.xml` ↔ `event type="on_kill_by_level"`
- `on_enter/227.xml` ↔ `event type="on_enter"`
- `on_quit/1.xml` ↔ `event type="on_quit"`

## 示例链路
- quest 214：`on_visit 5159.xml` + `on_kill 10001.xml`，描述中变量 `kill_*` 与 source 内 `<var type="Add" .../>` / `<refresh .../>` 联动。
- quest 211：`on_visit 5156/5157.xml` + `on_enter 227.xml`，体现“任务访问 + 全局进入”混合。

## target 函数映射说明
- 旧任务 XML 侧主要是动作节点（var/refresh/reward 等），不大量出现 `target=`。
- `request->target` 来自客户端交互包，在 `Quest::execute` 内传递给 `execute_script_event`，用于 Lua 函数调用（新任务桥接/特定 NPC 菜单）。

## NPC/怪物/场景条件
- source XML 中常见 `<if>` 条件 + `var` + `task`，怪物击杀通过 `on_kill/怪物ID.xml` 组织。
- 地图/NPC 条件需逐条 source 解析（建议下轮按国家/活动专题切片）。

RAG标签：`source-xml-map`, `on_visit`, `on_kill`, `quest214`。
适合问题：某个 source 文件影响哪些任务。

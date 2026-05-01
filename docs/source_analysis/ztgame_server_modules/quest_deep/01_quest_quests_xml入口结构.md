# quests.xml 入口结构
- 文件：`ztgame/quest/quests.xml`
- 声明：`<?xml version="1.0" encoding="GB2312"?>`
- 根节点：`<quests>`，子节点 `<quest id name>`。
- 事件节点：`<events><event type="..." source="..."/></events>`。

## 事件类型统计（从 quests.xml 抽样统计）
- on_visit: 1212
- on_use: 485
- on_kill: 482
- on_kill_by_level: 177
- on_get: 170
- on_other: 48
- on_quit: 13
- on_enter: 4

## source 字段与加载顺序
- `source` 仅写文件名（如 `5156.xml`），目录由 `type` 决定（如 `on_visit/5156.xml`）。
- 加载顺序：按 quests.xml 逐任务读取 + 运行时按事件触发查对应 EventManager（待结合 QuestTable 细节确认）。

## 缺失表现
- source 缺失：事件执行时无法命中动作（通常表现为任务点击无响应或流程中断）。
- type 与目录不匹配：配置存在但触发链断裂。
- 编码异常：GB2312 与 UTF-8 混用可能造成中文字段乱码/解析异常。

RAG标签：`quest-entry`, `quests.xml`, `event-source-binding`, `gb2312`。
适合问题：任务入口排错、source 丢失排错。

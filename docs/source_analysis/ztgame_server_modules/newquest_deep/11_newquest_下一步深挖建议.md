# 下一步深挖建议

优先目录：`ztgame/quest/`。

原因：
1. newquest 仅覆盖新任务脚本入口，旧任务大量逻辑在 `quest/quests.xml` + source xml。
2. 需要建立“旧 quest XML 事件 -> Lua/对话函数 -> C++ 调度”完整映射。
3. 可统一新旧任务系统边界，降低二开误改风险。

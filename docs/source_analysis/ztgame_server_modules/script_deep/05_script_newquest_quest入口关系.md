# newquest / quest 入口关系
- Lua 主入口：`newquest/quest.lua`。
- 入口内容：设置 `LUA_PATH`，`require "./newquest/npc/npc_848"`，定义 `die/enter/quit`。
- C++ 启动与热重载均固定加载 `newquest/quest.lua`（`ScenesServer.cpp` 与 `script.cpp::reloadVM`）。
- 任务配置：`quest/quests.xml`（声明 `encoding="GB2312"`，事件如 `on_enter/on_visit/on_kill` + source xml）。

## 事件链路
登录 enter -> NPC 对话/访问 -> 接任务/击杀/完成 -> 奖励 -> 退出 quit。
其中函数路由由 `execute_script_event` 根据 func 名与参数分发。

## 编码风险
- `quests.xml` 明示 GB2312；`script.cpp` 中函数名存在中文乱码痕迹，提示历史 GBK/UTF-8 混用。
- Lua require 路径含相对前缀 `./newquest/...`，部署目录变化会触发找不到脚本。

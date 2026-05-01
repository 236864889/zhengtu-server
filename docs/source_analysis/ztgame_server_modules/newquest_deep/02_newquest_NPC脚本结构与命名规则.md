# NPC 脚本结构与命名规则

- 主命名：`npc_<id>.lua`，函数命名遵循 `visit_<id>`/`state_<id>`/`kill_<id>`。
- 例外文件：`kill_boos.lua`（批量 boss kill 函数）、`tian_jiang_hong_fu.lua`（活动脚本名非 npc_xxx）。
- 多数 NPC 文件仅做路由：`require defineXXX` + 调用 `visit_npc/state_npc/kill_npc`。
- NPC ID 与文件名一一映射（如 `npc_825.lua` 对应 `visit_825/state_825`）。

## 异常命名
- `kill_boos.lua` 内函数如 `kill_3010`，不依赖 `npc_3010.lua` 文件存在。
- `npc_5021.lua` 依赖 `define5021`，但仓库内未见该文件（待确认）。

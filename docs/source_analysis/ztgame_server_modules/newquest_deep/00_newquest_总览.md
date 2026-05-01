# newquest 总览

- 真实职责：承接 ScenesServer Lua VM 启动入口，并提供 `visit_* / state_* / kill_*` 事件函数供 C++ `execute_script_event` 调用。
- 入口：`ztgame/newquest/quest.lua` 设置 `LUA_PATH`，并显式 require `npc_848`，同时导出 `enter/quit/die`。
- 目录规模：167 个 Lua 脚本（含 define*、npc 子目录）。

## 关键脚本 Top30（按行数）
1. `ztgame/newquest/npc/npc_825.lua` (1299 lines)
2. `ztgame/newquest/npc/npc_497.lua` (1298 lines)
3. `ztgame/newquest/npc/npc_822.lua` (1293 lines)
4. `ztgame/newquest/npc/npc_823.lua` (1292 lines)
5. `ztgame/newquest/npc/npc_821.lua` (1292 lines)
6. `ztgame/newquest/npc/npc_820.lua` (1291 lines)
7. `ztgame/newquest/npc/npc_5021.lua` (1122 lines)
8. `ztgame/newquest/define823.lua` (564 lines)
9. `ztgame/newquest/define822.lua` (563 lines)
10. `ztgame/newquest/define821.lua` (563 lines)
11. `ztgame/newquest/define820.lua` (563 lines)
12. `ztgame/newquest/define825.lua` (493 lines)
13. `ztgame/newquest/define497.lua` (493 lines)
14. `ztgame/newquest/npc/npc_247.lua` (349 lines)
15. `ztgame/newquest/define.lua` (319 lines)
16. `ztgame/newquest/npc/npc_848.lua` (298 lines)
17. `ztgame/newquest/npc/kill_boos.lua` (291 lines)
18. `ztgame/newquest/npc/npc_850.lua` (120 lines)
19. `ztgame/newquest/npc/npc_215.lua` (98 lines)
20. `ztgame/newquest/npc/tian_jiang_hong_fu.lua` (59 lines)
21. `ztgame/newquest/npc/npc_10032.lua` (30 lines)
22. `ztgame/newquest/npc/npc_10002.lua` (30 lines)
23. `ztgame/newquest/npc/npc_50021.lua` (29 lines)
24. `ztgame/newquest/npc/npc_16259.lua` (29 lines)
25. `ztgame/newquest/npc/npc_16257.lua` (29 lines)
26. `ztgame/newquest/npc/npc_16255.lua` (29 lines)
27. `ztgame/newquest/npc/npc_16251.lua` (29 lines)
28. `ztgame/newquest/npc/npc_16250.lua` (29 lines)
29. `ztgame/newquest/npc/npc_16249.lua` (29 lines)
30. `ztgame/newquest/npc/npc_16248.lua` (29 lines)

## 最高风险脚本 Top15
- `ztgame/newquest/npc/npc_820.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_821.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_822.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_823.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_825.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_497.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_5021.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/define820.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/define821.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/define822.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/define823.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/define825.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/define497.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/kill_boos.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。
- `ztgame/newquest/npc/npc_848.lua`：包含随机任务分配/奖励分支/refresh_npc 调用，改动回归面大。

# script 目录总览（函数级）

## 1. 真实职责边界
- `ztgame/script/` 本身主要是 **Lua 5.1 头文件 + luabind 头文件快照**，用于编译期接口；当前目录未发现 `.c/.cpp` 实现文件与 `.a` 静态库实物。
- 运行时脚本桥接逻辑并不在 `ztgame/script/`，而在 `ztgame/ScenesServer/script.cpp` + `script.h`（VM 初始化、绑定、事件调用）。
- 任务脚本入口在 `ztgame/newquest/quest.lua`，任务结构化配置在 `ztgame/quest/quests.xml`。

## 2. 静态库清单（本目录实扫）
- `ls ztgame/script/*.a` 无结果；当前仓库快照未包含 `liblua.a/libluabind.a/liblualib.a` 实物。
- 但 `ScenesServer/Makefile` 明确链接 `-L../script -llua -lluabind`，说明部署或历史构建环境中预计存在对应库。

## 3. 源码/头文件/第三方目录清单
- 文件总数：147（含 `.svn` 元数据）。
- 类型：`*.hpp` 74，`*.h` 4，`README.md` 1，其余为 SVN 快照。
- 关键目录：
  - `ztgame/script/lua.h|lualib.h|lauxlib.h|luaconf.h`
  - `ztgame/script/luabind/*`（含 `class.hpp/function.hpp/scope.hpp/detail/*`）

## 4. 被哪些服务链接
- 已确认：`ztgame/ScenesServer` 通过 `-I../script` 与 `-L../script -llua -lluabind` 直接依赖 script 目录产物。
- 待确认：其他服务是否也链接 `-llua/-lluabind`（建议后续扫描各服务 Makefile）。

## 5. 关键文件 Top30（本轮关联）
1. `ztgame/ScenesServer/script.cpp`
2. `ztgame/ScenesServer/script.h`
3. `ztgame/ScenesServer/ScenesServer.cpp`
4. `ztgame/ScenesServer/Quest.cpp`
5. `ztgame/ScenesServer/SceneNpc.cpp`
6. `ztgame/ScenesServer/SceneUser.cpp`
7. `ztgame/ScenesServer/SceneTask.cpp`
8. `ztgame/ScenesServer/SessionClient.cpp`
9. `ztgame/ScenesServer/SceneUserManager.cpp`
10. `ztgame/ScenesServer/Property.cpp`
11. `ztgame/ScenesServer/Trade.cpp`
12. `ztgame/ScenesServer/ScenePk.cpp`
13. `ztgame/ScenesServer/Makefile`
14. `ztgame/newquest/quest.lua`
15. `ztgame/quest/quests.xml`
16. `ztgame/script/lua.h`
17. `ztgame/script/lualib.h`
18. `ztgame/script/lauxlib.h`
19. `ztgame/script/luaconf.h`
20. `ztgame/script/luabind/luabind.hpp`
21. `ztgame/script/luabind/class.hpp`
22. `ztgame/script/luabind/function.hpp`
23. `ztgame/script/luabind/scope.hpp`
24. `ztgame/script/luabind/open.hpp`
25. `ztgame/script/luabind/error.hpp`
26. `ztgame/script/luabind/detail/call_function.hpp`
27. `ztgame/script/luabind/detail/call_member.hpp`
28. `ztgame/script/luabind/detail/pcall.hpp`
29. `ztgame/script/luabind/detail/policy.hpp`
30. `ztgame/script/README.md`

## 6. 最高风险文件 Top15
- `ScenesServer/script.h`：模板调用 + 异常吞并返回0，易掩盖业务故障。
- `ScenesServer/script.cpp`：VM 初始化与 dofile 失败处理只写日志。
- `ScenesServer/ScenesServer.cpp`：启动时硬依赖 `newquest/quest.lua`。
- `ScenesServer/Quest.cpp`：任务触发函数名/参数全走运行时绑定。
- `ScenesServer/SceneNpc.cpp`：NPC 多触发点，高频调用脚本函数。
- `ScenesServer/SceneUser.cpp`：enter/物品等多事件回调。
- `newquest/quest.lua`：入口 require 路径、编码、函数签名风险。
- `quest/quests.xml`：GB2312 声明与 Lua 文件编码不一致风险。
- `script/lua.h`：Lua 5.1 ABI 约束。
- `script/lualib.h`：openlib 集与 VM 初始化依赖。
- `script/luabind/class.hpp`
- `script/luabind/function.hpp`
- `script/luabind/detail/call_member.hpp`
- `script/luabind/detail/policy.hpp`
- `ScenesServer/Makefile`：链接库漂移风险。

## 7. RAG 入库优先级
- S级：`script.cpp`、`script.h`、`ScenesServer.cpp(加载段)`、`Quest.cpp`、`newquest/quest.lua`
- A级：`SceneNpc.cpp`/`SceneUser.cpp`触发点、`Makefile` 链接信息、`quests.xml`
- B级：`lua*.h`/`luabind/*.hpp`（主要做 ABI 与机制参考）

## 8. 适合解决的问题
- “Lua VM 在哪里初始化/加载哪些库？”
- “脚本事件如何从 C++ 进入 Lua？”
- “任务/NPC 事件链路中哪个点最可能报签名错误？”
- “二开新增绑定函数时最小改动路径是什么？”

## 9. 待确认
- `liblualib.a` 在当前仓库未发现；需到历史构建机或制品仓确认。
- 其他服务是否直接复用 `ScriptingSystemLua` 仍待全仓 Makefile 扫描。

# execute_script_event 调用链
- 定义位置：`ScenesServer/script.h`。
- 重载：0/1/2/3 参数模板版本。
- 核心流程：
  1) 校验 `user` 非空并设置 `current_user`；
  2) `ScriptingSystemLua::instance().getVM(0)` 获取唯一 VM；
  3) `luabind::call_function<int>(L, func, ...)` 调 Lua；
  4) 捕获 `luabind::error` / `const char*`，记录日志并返回0。

## 触发点（实扫）
- quit：`SceneTask.cpp` / `SessionClient.cpp` / `SceneUserManager.cpp`
- enter：`SceneUser.cpp`
- NPC：`SceneNpc.cpp` 多处
- 任务：`Quest.cpp`（target + id/offset 或 npc+id+offset）
- 交易/属性/PK：`Trade.cpp` / `Property.cpp` / `ScenePk.cpp`

## 风险
- 所有异常统一回0，调用侧若未强校验，可能静默失败。
- 模板参数签名与 Lua 函数不一致时，仅运行时暴露。

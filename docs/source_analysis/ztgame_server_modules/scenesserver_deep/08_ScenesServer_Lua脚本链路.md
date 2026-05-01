# Lua / newquest / quest 脚本链路
- 初始化：`script.cpp` 创建 Lua VM，打开 base/table/io/string/math/debug/package，`luabind::open`。
- 加载：`ScenesServer.cpp` 与 `script.cpp` 均会加载 `newquest/quest.lua`。
- 调用：`script.h` 模板 `execute_script_event` 封装 `luabind::call_function<int>`，支持 0~3 参数。
- 触发点：用户 quit、NPC 对话/事件、任务流程等。
- 风险：
  1) luabind 异常捕获后仅日志，业务补偿弱；
  2) Lua panic/脚本语法错误可能影响主循环；
  3) 脚本函数签名不匹配仅运行时暴露。

待确认：`newquest` 与 `quest` 双系统边界（建议后续扫 `ztgame/newquest` 与 `ztgame/quest` 调用入口）。

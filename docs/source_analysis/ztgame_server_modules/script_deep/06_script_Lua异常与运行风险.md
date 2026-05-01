# Lua 异常与运行风险
- Lua panic：未见显式 `lua_atpanic` 自定义处理，panic 行为依赖默认机制。
- luabind 异常：`execute_script_event` 捕获后记录 `CATCHED Luabind EXCEPTION` 并返回0。
- 签名不匹配：`call_function<int>` 固定 int 返回，Lua 返回值/参数不符会在运行时报错。
- 语法错误/路径缺失：`luaL_dofile` 失败记录 `FATAL ERROR:%s`，但不一定阻断服务流程。
- userdata/index 错误：典型来自 luabind cast 与对象生命周期不一致。
- 业务补偿不足：大部分触发点无重试/降级，表现为任务、NPC行为失效。

适合排障关键词：`FATAL ERROR`, `CATCHED Luabind EXCEPTION`, `call_function`, `quest.lua`.

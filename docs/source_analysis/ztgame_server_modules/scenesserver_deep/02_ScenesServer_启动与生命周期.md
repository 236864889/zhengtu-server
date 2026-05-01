# 启动与生命周期
- `main`：参数解析（含 mysql）-> 初始化全局配置 -> `ScenesService::main()`。
- `init`：初始化 taskPool/sessionClient/recordClient；调用 `zSubNetService::init`；再初始化 `SceneNpcManager/SceneManager/FuBenMgr/NpcTrade/MallTrade/QuestTable/GameConfigMgr/MagicRange` 与 Lua `newquest/quest.lua`。
- `validate`：继承 base 骨架（明确覆写点待确认，下一步查 `ScenesServer.h/.cpp` 全函数表）。
- `serviceCallback`：循环调度网络消息、用户状态轮询、超时回收、GM惩罚/踢人等。
- `final`：按逆序释放 `SceneTimeTick/taskPool/sessionClient/recordClient`，再 `zSubNetService::final`。
- 启动失败点：任一 manager init false、地图/脚本加载失败、Session/Record 连接失败。

适合问题：启动即退、半初始化、热重载不生效。

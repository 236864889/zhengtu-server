# ScenesServer 源码深度分析

## 1) 入口与生命周期
- `ScenesServer.cpp` 含 `main`。
- `ScenesService::init/final` 为生命周期主轴。
- 初始化步骤多且长，是全项目最复杂服务之一。

## 2) 模块构成（核心）
- 用户/对象：
  - `SceneUser.cpp/h`、`SceneUserManager.cpp/h`
  - `SceneNpc.cpp/h`、`SceneNpcManager.cpp/h`
  - `zObject.cpp/h`、`zObjectB.cpp/h`、`zObjectC.cpp/h`
- 任务与命令：
  - `SceneTask.cpp/h`、`SceneTaskManager.cpp/h`
- 地图与场景：
  - `Scene.cpp/h`、`Zone.cpp/h`、`WayPoint.cpp/h`
- 任务系统：
  - `Quest.cpp/h`、`QuestAction.cpp`、`QuestCondition.h`、`QuestEvent.cpp/h`、`QuestTable.cpp/h`
- 脚本系统：
  - `script.cpp/h`、`script_func.cpp/h`
- 配置系统：
  - `fjconfig.cpp/h`、`GameConfigMgrX.cpp/h`

## 3) 初始化链关键事实
- `ScenesService::init` 包含大量 `fjconfig::init*` 调用。
- 显式初始化点包括：`NpcTrade::init`、`MallTrade::init`、`MessageSystem::init`、`MagicRangeInit::init` 等。
- 配置失败容易导致整个场景服启动中断。

## 4) 协议与任务分发
- `SceneTask.cpp` 中存在 `verifyLogin/verifyConn` 验证链。
- 大量 `Cmd::` 分支处理玩家行为与系统消息。
- `SceneTask` 是“网络事件 -> 玩法逻辑”的核心桥梁。

## 5) Lua 与任务耦合
- Makefile 链接 `-L../script -llua -lluabind`。
- `Quest.cpp` include `luabind/luabind.hpp`，任务链直接接 Lua。
- 存在 `quest/` + `newquest/` 并行脚本资源，需警惕版本漂移。

## 6) 数据与持久化关系
- 场景服通过 client 与 Session/Record/Bill 等外部服务协作。
- 本地更多承担“实时状态与逻辑演算”，持久化链路跨服务完成。

## 7) 高风险改动点
1. `SceneTask.cpp` 命令分发改动（影响面极广）。
2. `fjconfig.cpp` 配置解析改动（启动风险高）。
3. `Quest*` + `script*` 改动（玩法与脚本耦合风险）。
4. `zObject*` 改动（对象生命周期与战斗稳定性风险）。

## 8) 建议的二开顺序
1. 先补配置与命令映射文档。
2. 小范围灰度单玩法，再扩展全图。
3. 对脚本变更做回滚预案（版本号 + 校验）。

## 9) 待确认
- Lua panic 保护与错误传播边界。
- Quest 新旧脚本冲突检测机制。
- 大场景高并发下任务池背压阈值。

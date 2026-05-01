# MiniServer 源码分析（v0.2）

## 入口与生命周期
- `MiniServer.cpp`：`main` 入口。
- `MiniService::init/final` 生命周期。

## 模块结构
- `MiniTask.cpp/h`、`MiniTaskManager.cpp/h`
- `MiniUser.cpp/h`
- `MiniUserDataParser.cpp/h`
- `SceneClient.cpp/h`（与场景侧交互）

## 关键模式
- 小游戏服务独立任务分发 + 用户对象管理。
- 与场景服务存在联动客户端关系。

## 风险点
- 小游戏状态与主角色状态同步一致性。
- 跨服消息延迟导致状态错位。

## 待确认
- Mini 与 Scene 的协议映射表。

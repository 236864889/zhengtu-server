# SessionServer 用户会话与状态管理

## 核心对象
- `UserSession` / `SceneSession`（定义在 `Session.h`）。
- `UserSessionManager : zUserManager`。
- `SceneSessionManager : zSceneManager`。

## 管理能力
- 按 name/id/tempid 查询用户会话。
- 按条件群发、按 task 批量清理用户。
- 场景对象按 name/file/id/tempid 查询与移除。

## 登录后状态流转（源码可见部分）
- Gateway 命令进入 `msgParse_Gate` 后驱动用户会话建立/更新。
- Scene 命令进入 `msgParse_Scene` 后维护用户在场景中的在线态。
- 各业务模块通过 `UserSessionManager::getUserByID` 按需读写在线态并向 scene 发包。

## 踢人/断线/重连
- 大量逻辑位于 `SessionTask.cpp` 分支；当前确认存在按 task 清理用户能力。
- 重连细节需继续定位 `CMD_GATE` 子 para（待确认）。

## 跨服务同步
- 通过 `SessionTaskManager::broadcastScene/broadcastGateway/sendCmdToCountry` 完成分服广播。

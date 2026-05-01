# MiniServer 主链路通信关系

## 协议骨架
- `Cmd::Mini` 定义 4 类 cmd：`CMD_LOGIN/CMD_GATE/CMD_FORWARD/CMD_SCENE`。
- `MiniTask::cmdMsgParse` 按上述四类分发到 `parseGateMsg/parseForwardMsg/parseSceneMsg`。

## Gateway -> Mini
- Gateway 启动时连 Mini 并发送 `t_LoginMini`。
- 玩家首次 Mini 指令会触发 `t_UserLogin_Gateway`；之后用户包封装为 `t_Mini_UserForwardMini`（`PARA_USER_FORWARD_MINI`）发 Mini。
- Mini 侧登录后 `MiniHall::userEnter` 发登录成功、用户分数与房间列表。

## Mini -> Gateway
- Mini 使用 `t_Mini_ForwardUser`（`PARA_MINI_FORWARD_USER`）回推用户包，由 Gateway MiniClient 解包再投递给在线用户。

## Session/Scenes -> Mini
- Session：当前未见专属 MiniClient 或 msgParse 分支，暂不在直连主链路。
- Scenes：存在 `ScenesServer/MiniClient.*`，并使用 `PARA_SCENE_SET_SCENE/DEPOSIT/CHECK_DRAW/DRAW_RET` 与 Mini 协作资金与场景绑定。

## Super 服务发现
- MiniTask 验证登录时会用 `MiniService::getServerEntry(wdServerID)` + IP + serverType 校验，意味着接入方必须在 Super 服务表中可查。

## sendCmd/cmdMsgParse 关键链
- 入站：`MiniTask::verifyConn -> verifyLogin`。
- 业务分发：`cmdMsgParse -> parseGateMsg/parseForwardMsg/parseSceneMsg`。
- 出站到用户：`MiniTask::sendCmdToUser -> t_Mini_ForwardUser`。

## 是否影响登录后功能入口
- 是。若 Mini 链路异常，登录后小游戏入口、迷你资金流（充值/提现）与相关提示会失败，但主战斗链路（Scene核心玩法）理论上可存活。

## RAG 标签
- `CMD_GATE`, `CMD_FORWARD`, `CMD_SCENE`, `MiniClient`, `forwardMini`, `verifyLogin`

## 适合解决的问题
- 小游戏按钮点了没反应。
- Mini 金币提现状态与场景不一致。

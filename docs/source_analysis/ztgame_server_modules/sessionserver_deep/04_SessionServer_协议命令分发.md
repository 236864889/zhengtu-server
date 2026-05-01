# SessionServer 协议命令分发

## Command 头引用
- `Command.h`、`SessionCommand.h`、`RecordCommand.h`、`GmToolCommand.h`（散见于核心文件）。

## 分发入口
- `SessionTask::cmdMsgParse`：按 `cmd` 分发到 `msgParse_Scene/msgParse_Gate/msgParse_Forward`，并处理 shutdown/boss 等。
- `SessionService::msgParse_SuperService`：处理来自 Super 的会话/GM 相关指令。
- `RecordClient::cmdMsgParse`：处理 Record 回包。

## 重点命令域
- 登录/用户关联：`CMD_GATE` 路径（细分子 para 分支多，建议继续拆表）。
- 场景同步：`CMD_SCENE` + `*_SceneSession` 结构。
- 跨服转发：`CMD_FORWARD`。

## 高风险命令
- 关服类：`CMD_SCENE_SHUTDOWN/PARA_SHUTDOWN`。
- GM 广播类：`GmTool PARA_BROADCAST_GMTOOL`。
- BOSS 持久化类：`PARA_NPC_BOSSDIE_INFO`（直接 DB 写）。

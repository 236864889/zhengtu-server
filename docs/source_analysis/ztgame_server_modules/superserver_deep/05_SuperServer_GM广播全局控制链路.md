# SuperServer GM/广播/全局控制链路

## GMTool 入口
- 在 `ServerTask::msgParse` 中分发 `Cmd::GmTool::CMD_GMTOOL` 到 `msgParse_GmTool`。

## 处理动作
- `PARA_LOG_GMTOOL`：补 `zone` 后转发 Info。
- `PARA_CHAT_GMTOOL`：补 `zone + server(zoneName)` 后转发 Info。
- `PARA_MSG_GMTOOL` / `PARA_NEW_MSG_GMTOOL`：补 zone 后转发 Info。
- `PARA_PUNISH_GMTOOL`：补 zone/server 后转发 Info（封禁/处罚链路入口之一）。

## 全局控制
- `CMD_SESSION + PARA_SHUTDOWN`：Super 接收到 Session 下发后调用 `Terminate()`，触发服务停止。
- `TimeTick` 每秒广播 `t_GameTime` 给全部已注册服务，形成全局时钟同步。

## 风险
- GM 参数字段由 Super 原地改写（复用原包内存），若结构体版本不一致可能越界/误写。
- 全局关服命令来源只按 cmd/para 匹配，缺少额外鉴权字段。

## 适合解决的问题
- GM消息“发出但后台平台看不到”。
- 关服命令是谁触发、经过哪条链路。

## 待确认
- InfoServer 对 GM 指令的最终落地（查 `ztgame/InfoServer/`）。

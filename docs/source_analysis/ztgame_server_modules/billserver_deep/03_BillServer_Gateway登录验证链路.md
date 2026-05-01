# Bill 与 Gateway 登录验证链路
1. Gateway 连接 Bill 后先发 `t_LoginBill`（`CMD_LOGIN/PARA_LOGIN`），`BillTask::verifyLogin` 校验 `wdServerID/wdServerType/IP`。
2. Super 下发 `PARA_BILL_NEWSESSION` 给 Bill，Bill 创建 `BillUser` 并转发 `t_NewSession_Gateway` 给对应 Gateway task。
3. Gateway 发 `PARA_GATE_LOGINVERIFY`，Bill 在 `BillTask::cmdMsgParse` -> `BillUser::login(loginTempID)`：
   - 从 `BALANCE` 表按 `accid` 查询余额/金币/月卡等字段。
   - 成功则回 `t_LoginVerify_Gateway_Return.retcode=1`；失败/不匹配回 0。
4. Gateway 收到回包后放行或拒绝。

检查项：
- 账号状态由 Bill 内存态（`WAIT_LOGIN/CONF_LOGIN`）+ DB 余额记录共同决定。
- 封禁/权限检查在 Bill 侧不明显，更多在 Gateway/Session；Bill 主要做账务可用性与会话匹配。

风险：
- 登录放行依赖 `accid + loginTempID`，若会话竞争或复用异常，可能误放行/误拒绝。

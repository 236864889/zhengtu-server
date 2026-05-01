# Bill 与 Scene 充值消费链路
- Scene 侧命令经 `CMD_FORWARD/PARA_SCENE_FORWARD_BILL` 到 BillTask，路由到 `BillUser::usermsgParseScene`。
- 典型路径：
  - 存钱：更新 `BALANCE.money/gold` 并通知网关/场景。
  - 取钱：先校验本地余额，再 `exeUpdate` 扣减，随后回包 `t_Stock_Fetch` 等。
  - 点卡兑换：Gateway 请求 -> `Bill_action` 发外部统一计费 -> 回调后 `BillUser::redeem_*` 落地并回包。
- 失败补偿：
  - 外部扣点成功但游戏内发奖失败时，写 `RESTORECARD/RESTOREGOLD`，用户下次登录 `restorecard/restoregold` 补发。
- 重复扣费风险：
  - 通过 `tid` + `begin_tid/check_tid/end_tid` 避免重放，但边界场景（超时、进程重启）仍需审计。

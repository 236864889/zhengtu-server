# 账号点数与资金一致性边界
- POINT/BONUS/FILLPOINT/CONSUMEPOINT 主要通过 `BillData.at`（如 `AT_CONSUME`）和 `Bill_action -> BillClient` 执行外部计费。
- Bill 内部保存：本地余额(`BALANCE`)、补单(`RESTORE*`)、委托交易与历史。
- Web/上游计费系统保存：真正点卡账户与外部扣点流水（由 `BillClient` 对接）。
- Scene 保存：角色资产展示与玩法态，最终账务准实时依赖 Bill 回包。
- 幂等机制：`tid` 缓存 + `BillCache` 超时回收；存在重启丢缓存的天然缺口。
- 回滚策略：失败不做数据库事务回滚，而是记录补偿数据并在后续登录补发。
- 审计：`tradelog` 与 `ClientTrade` 双日志通道，适合做离线对账。

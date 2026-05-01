# MiniServer 高风险点与二开建议

## 是否影响主链路
- 中等影响：不阻断“角色进场景主链路”，但会影响登录后小游戏入口与资金子链路体验。

## 风险清单
1. **会话一致性风险**：MiniUser 同时持有 gate/scene task 指针，场景切换依赖 `PARA_SCENE_SET_SCENE`，时序错会导致回包丢目标。
2. **数据丢失风险**：draw 失败时有在线/离线两套补偿逻辑，错误分支中存在 `u` 空指针仍调用 `u->sendCmdToMe` 风险。
3. **协议错位风险**：`CMD_FORWARD` 里大量裸内存拷贝，size 未做充分上界校验。
4. **连接回收风险**：`recycle_state` 两阶段删除依赖轮询，异常中断可能残留在线态。
5. **玩法状态机风险**：DDZ 超时自动叫分/出牌与手动包竞态可能触发分歧。
6. **配置风险**：Mini.xml 与线程池参数缺校验，非法配置会启动失败或运行抖动。

## 推荐安全改造点
- 为 `MiniTask::parseSceneMsg` 加空指针保护与统一回包路径。
- 为 `t_Mini_UserForwardMini/t_Mini_ForwardUser` 增加长度阈值校验与审计日志。
- 给资金变更路径加“幂等流水号”（至少在日志中记录 request-id）。
- 增加用户态一致性巡检（gate/scene task 是否可达）。

## 暂不建议改造点
- 不建议先动 `DDZCardGame` 出牌规则核心（历史行为兼容性高风险）。
- 不建议先改协议编号（跨 Gateway/Scene/base 三端联动大）。

## RAG 标签
- `risk`, `session consistency`, `money compensation`, `protocol size`, `recycle`

## 适合解决的问题
- 线上“偶现提现失败后金币不对”。
- Mini 用户幽灵在线/无法再次进入。

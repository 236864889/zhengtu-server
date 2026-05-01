# 下一步深挖建议

## 优先目录
- `ztgame/GatewayServer/`（优先级 P0）

## 原因
1. Gateway 是玩家入口第一跳，与 Session 的 `CMD_GATE` 分发链路最紧。
2. 登录态/重连/踢人问题通常横跨 Gateway+Session。
3. Session 已确认大量逻辑依赖 Gateway 上送字段，需双边对齐协议。

## 建议输出
- Gateway 启动与监听模型
- 登录请求到 Session 的完整命令链
- Gateway 断线与 Session 清理协作机制
- 网关限流/防刷/封禁点

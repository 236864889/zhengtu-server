# GatewayServer 下一步深挖建议

## 建议目录
- `ztgame/ScenesServer/`

## 原因
1. Gateway/Session 已完成后，ScenesServer 是玩家入场后业务执行主战场。
2. 当前 Gateway 文档里多处调用最终落在 Scene（切图、可见广播、场景命令转发）。
3. 风险链条闭环需要 Scene 侧验证：Gateway 发包 -> Session 协调 -> Scene 执行。

## 下一轮优先主题
- Scene 服务生命周期与线程模型。
- Scene 与 Session/Gateway 的命令分发对齐。
- 玩家入场、切图、AOI 广播路径。
- 反外挂/非法包处理与性能热点。

## 待确认问题清单（承接本轮）
- GateUser 的 checkPassport 与 checkUserCmd 精确策略阈值。
- SceneClient 索引与屏广播在高并发下的一致性。
- Session 下发广播在 Gateway 与 Scene 的压测瓶颈点。

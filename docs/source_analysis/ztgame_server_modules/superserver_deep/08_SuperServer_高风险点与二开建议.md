# SuperServer 高风险点与二开建议

## 风险清单
1. 服务发现单点：`SERVERLIST + SuperServer` 任一异常导致全服发现失败。
2. 注册表一致性：`container` 与 `taskUniqueContainer` 分离维护，异常路径可能短暂不一致。
3. 全局广播阻塞：`broadcast` 持锁 send，慢连接可能拖慢全局。
4. GM 命令风险：Super 对 GM 包体直接改写字段，缺鉴权增强。
5. 断线级联：Gateway 下线会直接影响 FL 可见区服，可能触发大范围“区服消失”。

## 建议安全改造点（优先）
- 为 `broadcast*` 引入无锁快照发送/分桶发送，降低阻塞。
- 为 `RoleregCache` 增加长度上限与丢弃告警。
- 为 `CMD_SESSION/PARA_SHUTDOWN` 增加签名或来源白名单。
- 为 `SERVERLIST` 增加启动前一致性检查工具（离线脚本）。

## 暂不建议改造
- 不建议先动 `SuperCommand.h` 协议结构（跨服务影响面太大）。
- 不建议先改 `serverSequence` 逻辑，先补链路观测与告警。

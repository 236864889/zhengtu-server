# FLServer 高风险点与二开建议

## 高风险点
1. 区服列表错误：Super-网关链路短断会让 FL 仅持旧缓存。
2. NewSession 丢失：tempid 映射若超时回收过快，会造成“网关已建会话但客户端拿不到成功包”。
3. 重复登录：依赖 Bill 判重；若 Bill 短暂异常可能放大误判。
4. 维护态同步：`disableAll` + 延迟上报窗口期间可能全区不可见。
5. 版本过滤：`zoneGameVersion` 由网关上报，配置不一致会大面积拒绝。

## 推荐安全改造点
- 引入 tempid 生命周期指标与可观测日志（创建/下发/回包/超时）。
- GYList 增加时间戳与“陈旧度”报警。
- 登录关键路径加幂等标识，避免重复 NewSession 投递。
- 统一维护态与版本门槛配置源，减少网关节点漂移。

## 暂不建议改造点
- 不建议直接改协议结构（影响 Gateway/Super/Bill 全链路）。
- 不建议切换加密握手方式（`_ENCDEC_MSG`）而不同时升级客户端。

## RAG 标签
`risk`, `session-loss`, `idinuse`, `gylist-stale`, `version-gate`

# 下一步深挖建议

## 建议目录
- `ztgame/FLServer/`（优先级 P0）

## 原因
1. SuperServer 已确认与 FL 之间存在 GYList/NewSession/IDInUse 关键登录链路。
2. 客户端可见区服列表、登录入口调度主要受 FL/前置服控制。
3. 要闭环“服务发现 -> 区服可见性 -> 登录分流”，必须联读 FLServer。

## 下一轮建议重点
- FLServer 与 Super 的握手协议、tempid 映射、故障重连。
- 区服列表生成与过滤逻辑（维护态、版本门槛、网络类型）。
- FL 到 Gateway/Bill 回路的超时与重试策略。
- GM 或后台平台是否直接触达 FL。

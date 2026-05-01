# 下一步深挖建议
建议下一轮优先：`ztgame/SuperServer/`。

原因：
1. 当前已覆盖 base/Gateway/Session/Scenes/Record/Bill，SuperServer 是服务发现与注册中枢。
2. Bill 登录会话分发依赖 Super 命令（如 `PARA_BILL_NEWSESSION`），需要从源头确认全局状态机。
3. GM/广播/跨服调度通常在 Super 侧统一管控，属于全局链路关键缺口。

建议重点：服务注册表、跨服路由策略、故障转移、全局踢人/封禁广播、与各子服务握手协议一致性。

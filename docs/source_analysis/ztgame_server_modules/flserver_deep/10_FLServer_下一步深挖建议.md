# 下一步深挖建议

## 建议目录
- **优先：`ztgame/MiniServer/`**

## 原因
1. 主登录链路核心服（Base/Super/Gateway/Bill/FL）已覆盖，MiniServer 是剩余关键服务之一。
2. 需确认 Mini 是否参与主启动链路、是否承载独立玩法/小游戏/辅助服务。
3. Gateway 与 Mini 的客户端转发关系可能影响登录后首帧与功能入口可用性。

## 下一轮建议问题
- MiniServer 是否需要 Super 注册与回调。
- 是否有独立会话/鉴权复用。
- 是否影响主链路 SLO（延迟/可用性）。

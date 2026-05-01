# GatewayServer 防刷限流与连接保护

## IP/连接数限制
- 连接总量由 `zTCPTaskPool` 容量与状态控制。
- `GatewayTaskManager` 做 accid 唯一，抑制同号并发登录。

## 非法包处理
- 握手阶段仅接受固定登录子命令；其他包在未完成验证时直接失败。
- 选角、创建角色、删角命令对参数边界/国家合法性做检查。

## 超时与频率
- `GatewayTask` 内有 `sampleInterval/maxSamplePPS/chatInterval` 等节流字段；用于包频检测与聊天节奏控制。
- `LoginSessionManager::update` 定期清理超时登录票据（20s）。

## 黑名单/封禁
- Session 下发 `t_Session_HandleBlackList`，Gateway 在 `SessionClient::msgParse` 调用户黑名单增删。

## 安全日志
- 使用 `logger->alarm/error/warn/iffy` 记录异常角色、非法命令、验证失败。

## 待确认
- `GateUser::checkCmd`/`checkPassport` 的具体阈值与策略分布在 `GateUser.cpp`，需下一轮继续函数级拆解。

# FLServer 区服列表与网关选择链路

- 数据源：Gateway 上报 -> Super 转发 -> FL `GYListManager`。
- 结构字段：`wdServerID/pstrIP/wdPort/wdNumOnline/state/networkType/zoneGameVersion`。
- 可见列表：
  - ping 端口请求由 `PingTask::msgParse` 调 `GYListManager::full_ping_list` 返回。
  - `ServerACL` 从 `zoneInfo.xml` 维护区服基础信息。
- 版本过滤：`GYListManager::verifyVer` 检查 `zoneGameVersion`，不匹配则返回 `LOGIN_RETURN_VERSIONERROR` 或 `LOGIN_RETURN_GAMEMAINTAIN`。
- 维护态：`state_maintain` 相关状态在 `getAvl`/`verifyVer` 中参与可用性选择。
- 网关选择：`GYListManager::getAvl` 选择 online 最小且可用节点（同区多网关）。
- 登录容量门槛：`LoginManager::verifyReturn` 若 `wdNumOnline >= maxGatewayUser - 10` 返回 `LOGIN_RETURN_USERMAX`。

## 风险
- 列表仅内存态，断链后 `disableAll` 可能导致短时全不可用。
- `networkType` 当前 FL 链路使用痕迹弱，跨网筛选可能在客户端侧完成，需联查客户端协议消费。

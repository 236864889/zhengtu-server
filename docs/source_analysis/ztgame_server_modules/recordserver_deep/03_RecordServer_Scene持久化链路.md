# Record 与 Scene 持久化链路
## 主链路
1. Scene 发 `PARA_SCENE_USER_READ` -> Record `readCharBase` 读 `CHARBASE` 并回 `t_UserInfo_SceneRecord`。  
2. Scene 发 `PARA_SCENE_USER_WRITE` -> Record 先 `RecordSessionManager::verify` 再 `saveCharBase` 更新 `CHARBASE`，部分场景 remove session，成功回 `t_WriteUser_SceneRecord_Ok`。  
3. Scene 下线/切图/定时写回由 `writeback_type` 区分（`LOGOUT/CHANGE_SCENE/TIMETICK/OPERATION`）。

## 数据内容
- 结构化字段：`CharBase`（大量列）。  
- 压缩二进制档：`dataSize + data[]`，通常对应对象、技能、任务、宠物状态等（由 Scene 打包）。

## 补偿与重试
- 当前看不到 Record 端持久化失败重试队列；失败多为即时返回 false/日志。  
- `RecordSessionManager` 防并发但不保证事务补偿，宕机场景可能丢最近写回。

## 风险
- 异步 ack 乱序：写回请求与 Scene 内部状态推进依赖时序。  
- `remove` 触发时机耦合写回类型，逻辑改动易引入“会话未清/过早清理”。

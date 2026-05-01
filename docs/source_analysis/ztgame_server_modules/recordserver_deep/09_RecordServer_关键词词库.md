# RecordServer 关键词词库
- 启动生命周期：`RecordService::init`, `zSubNetService`, `taskPool`, `reloadConfig`, `daemon`
- Scene持久化：`PARA_SCENE_USER_READ`, `PARA_SCENE_USER_WRITE`, `t_WriteUser_SceneRecord_Ok`, `writeback_type`
- Session/Gateway查询：`PARA_CHK_USER_EXIST`, `PARA_GATE_CREATECHAR`, `PARA_GATE_DELCHAR`, `getSelectInfo`
- 数据库SQL：`CHARBASE`, `ACCPRIV`, `exeSelectLimit`, `exeUpdate`, `escapeString`
- 角色数据：`CharBase`, `ALLBINARY`, `dataSize`, `compressSaveData`
- 事务一致性：`no explicit transaction`, `race window`, `idempotent`
- 配置：`mysql`, `ifname`, `threadPoolCapacity`, `initThreadPoolState`, `cmdswitch`
- 风险排查：`out-of-order ack`, `save lost`, `slow query`, `mapping drift`

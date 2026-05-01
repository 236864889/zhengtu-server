# base 数据库基础层
## 1. 范围
`zDBConnPool.h zMysqlDBConnPool.cpp zDatabase* zDatabaseManager*`
## 2. 核心链路
- 初始化：URL 解析 -> `mysql_init/mysql_real_connect` -> 连接句柄池。
- 执行：`execSql/execSelectSql` 为统一入口；上层封装 select/insert/update/delete SQL 拼装。
- 事务：`SET AUTOCOMMIT=0/1` + `COMMIT/ROLLBACK`。
## 3. 职责
- `zMysqlDBConnPool`: 连接生命周期、SQL 执行、结果集提取。
- `zDatabaseManager`: 全局表管理、`loadAllBM/unloadAllBM`。
- `zDatabase.h`: 元数据结构与字段映射逻辑。
## 4. 风险
- 字符串拼接 SQL 注入/转义风险。
- 失败重连与错误码处理分散，易漏。
- 字符集/编码未统一可能导致脏数据。
## 5. RAG
- level: S
- keywords: zMysqlDBConnPool,execSql,execSelectSql,commit,rollback,loadAllBM

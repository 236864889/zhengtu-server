# BillServer 数据库表与 SQL 映射
主要表（来自 `metaData->getFields()`）：
- `BALANCE`：accid, money, gold, monthcard, list计数等。
- `RESTORECARD`：补发道具卡。
- `RESTOREGOLD`：补发金币。
- `CONSIGNGOLD` / `CONSIGNMONEY`：钱庄挂单。
- `CONSIGNGOLDHISTORY` / `CONSIGNMONEYHISTORY`：成交历史。

SQL形态：
- `exeSelect`：登录拉余额、查询委托、查询历史。
- `exeInsert`：补单写入、挂单写入、历史落库。
- `exeUpdate`：余额增减。
- `exeDelete`：补单消费后删除、撤单删除。

事务边界：
- 代码中多为单条语句，无显式 begin/commit/rollback；跨多表操作非原子，需靠业务补偿。

风险：
- `where.put("accid", oss.str())` 等字符串拼接，转义与注入防护依赖底层封装，需核验 `zDBConnPool`。
- 编码/注释混杂，日志与字段名可能出现乱码，影响审计检索。

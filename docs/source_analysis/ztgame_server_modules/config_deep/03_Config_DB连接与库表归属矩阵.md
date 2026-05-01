# DB 连接与库表归属矩阵

| 服务 | mysql 来源 | 逻辑库 | 核心表/域 | 读取点 |
|---|---|---|---|---|
| RecordServer | `Zebra::global[mysql]` | RecordServer | `CHARBASE`,`ACCPRIV` | `RecordServer.cpp` + Record 文档 |
| BillServer | 同上 | BillServer | `BALANCE`,`RESTORE*`,`CONSIGN*` | `BillServer.cpp` + Bill 文档 |
| FLServer | 同上 | FLServer/账号域 | `ACCSTORE0000`（来自既有文档线索） | `FLServer.cpp` |
| MiniServer | 同上 | MiniServer | `MINIGAME`（既有文档线索） | `MiniServer.cpp` |
| SessionServer | 同上 | Session/综合 | `ALLY`,`SEPT`,`BOSSINFO`（既有线索） | Session 数据管理模块 |
| SuperServer | 同上 | SuperServer | `SERVERLIST` 及服控表 | `SuperServer` |

## 统一规则
- URL 格式：`mysql://user:pass@host:port/dbName`
- 可 `encode=yes`。
- 多数服务启动期 `dbConnPool->putURL(0, global[mysql])`。

## 风险
- 默认 URL 指向 192.168.* 内网，迁移云环境高风险。
- 多服务共库但表前缀不隔离时，误连可能“可启动但逻辑错”。

# MySQL 与数据库样例（静态）

## 覆盖文件
- `ztgame/test/MySqlTest.cpp`
- `ztgame/test/NewMySqlTest.cpp`

## 使用的 MySQL 封装
- 两个样例均继承 `zService`，在 `init()` 中初始化数据库能力。
- 共同依赖：`zDBConnPool`（URL 注入）、`zMetaData`（元数据初始化）等 `base` 封装。

## 连接参数来源
- 非配置中心动态注入，而是代码中写死 URL：
  - `mysql://Zebra:Zebra@192.168.2.11:3306/Zebra`
  - `mysql://zebra1:zebra1@192.168.2.16:3306/zebra1`
- 说明：这是历史联机样例，不具备凭据隔离与环境切换。

## 查询/事务行为
- 样例内包含大量 SQL 操作路径（含写操作分支）；并在 `serviceCallback()` 持续执行。
- 目的偏性能/稳定性验证而非只读巡检。

## 为什么不建议直接运行
- 明确写库风险：可能对目标库建表/改写/删除。
- 凭据明文 + 内网地址，存在安全与不可达双重风险。
- 持续循环执行，若连上真实库可能形成持续写压。

## 可复用建议
- 仅复用“数据库封装调用顺序”：`init service -> init pool/meta -> callback 执行 SQL -> final`。
- 实测前必须改为：
  1) 本地沙箱库；
  2) 只读账号；
  3) 显式 dry-run SQL 白名单；
  4) 限次退出条件。

## RAG 标签 & 适合问题
- 标签：`mysql-test` `db-conn-pool` `write-risk` `legacy-credential`。
- 适合问题：
  - “旧项目里数据库连接池如何初始化？”
  - “为什么历史 MySQL test 不能直接在现网跑？”

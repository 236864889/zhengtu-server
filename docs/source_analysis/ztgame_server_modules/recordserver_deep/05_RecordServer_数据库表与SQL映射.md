# RecordServer 数据库表与 SQL 映射
## 主要表
- `CHARBASE`：核心角色基础+大量扩展字段+二进制列（如 `ALLBINARY`）。
- `ACCPRIV`：建角时读取账号权限。
- `CHARTEST`：仅 `_TEST_DATA_LOG` 场景。

## SQL 类型
- SELECT：角色读取、存在校验、国家排序、角色列表、查重名、账号权限。  
- INSERT：建角、测试数据插入。  
- UPDATE：角色写回、测试更新。  
- DELETE：删角（`CHARBASE`）。

## 事务边界
- 未看到显式 `BEGIN/COMMIT/ROLLBACK`；多语句流程（如建角前查后插）非事务，存在竞争窗口。  
- 句柄级执行为单条 SQL 原子，不等于业务事务原子。

## 字段/MetaData/编码风险
- `charbase_define` 超大列映射，字段顺序和 `CharBase` 结构高度耦合。  
- 大量 `char[]`/`DB_BIN` 列，编码和长度越界风险高。  
- 多数 where 采用 `escapeString`，但仍有 SQL 拼接逻辑，需持续审计。

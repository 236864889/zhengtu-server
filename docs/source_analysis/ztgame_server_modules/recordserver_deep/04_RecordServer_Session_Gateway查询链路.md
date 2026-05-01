# Record 与 Session/Gateway 查询链路
- Session -> Record：`PARA_CHK_USER_EXIST`，按角色名查询 `CHARBASE`，回填 `user_id/user_level` 返回。  
- Gateway -> Record：
  - 角色列表：`PARA_GATE_GET_SELECTINFO` (`getSelectInfo`)；
  - 查重名：`PARA_GATE_CHECKNAME`；
  - 建角：`PARA_GATE_CREATECHAR`（含 `ACCPRIV` 查询+插入 `CHARBASE`）；
  - 删角：`PARA_GATE_DELCHAR`；
  - 国家推荐排序：`REQUEST_GATE_COUNTRY_ORDER`（group by country）。

## 关系总结
- Gateway 关心“账号入口数据与建删角”。  
- Session 关心“角色存在性与社交推荐补全”。  
- Record 是入口查询真源，但并不维护完整在线状态。

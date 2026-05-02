# 顶层脚本 / XML / SQL 补充分析候选

## 顶层文件扫描结论
- 仓库根目录 `-maxdepth 1` 下未发现 `*.sh`、`*.xml`、`*.sql`。
- 因此 `start.sh`、`start1.sh`、`stop.sh` 在仓库根 **未发现**。

## 配置入口与替代扫描建议
- 启动/停服脚本候选应转向：`ztgame/tools`（及其子目录）与 `docs/operations` 的脚本引用。
- SQL 主入口位于 `mysql/`（`ZtGameDB.sql`、`FLServerDB.sql`），不在仓库根。
- XML 配置入口主要在 `ztgame/Config` 与 `ztgame/test`（测试样例 XML）。

## 是否需要单独 deep
1. **`ztgame/tools` 脚本入口链**：需要单独 deep（补充运维入口与发布回滚链）。
2. **`mysql/*.sql`**：可作为“数据库结构补缺专题”轻量 deep。
3. **根目录脚本专题**：不需要（因未发现目标文件）。

## 待确认项（含下一步路径）
- 待确认：历史文档提到的 `start.sh/start1.sh/stop.sh` 是否在外部部署包。下一步检查路径：`docs/operations/*.md` 与 `教程系列_上传版/服务端说明/*` 的脚本引用。

# tools 数据库与 SQL 工具分析

## 扫描结论
- `ztgame/tools` 下未发现 `*.sql` 文件。
- 未发现显式 SQL 语句（`SELECT/INSERT/UPDATE/DELETE`）出现在 tools 源码。
- `Makefile` 链接 `mysql_config --cflags/--libs_r`，说明工具在构建时引入 MySQL 客户端库，但本轮可见源码未体现直接 CRUD。

## 可能原因
1. 复用 `../base/libzebra.a` 的通用 DB 能力，但 tools 当前业务路径未调用。
2. 历史版本可能有 DB 功能后被删除或迁移。

## 风险评估
- 误写库风险：低（从当前源码可见路径）。
- 连错库风险：中（运行参数含 server/port/user/passwd，主要是登录服而非 DB 直连）。
- 凭据泄露风险：中高（命令行/配置传入账号密码）。

## 建议
- 运维制度上仍将 `zebraclientService` 定义为高危网络工具，禁止对生产账号段执行。
- 继续在 `base` 层追踪 DB API 被 tools 调用情况（下一轮可做符号级 grep）。

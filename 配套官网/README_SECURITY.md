# 配套官网安全治理说明（P0 静态改造）

## 范围
- 本次仅做静态改造：配置外置、去敏、文档补齐。
- 不执行 PHP、不连 DB、不做真实支付回调。

## 环境变量规范
1. 复制 `.env.example` 为 `.env`（或由部署系统注入环境变量）。
2. 生产环境必须设置：`DB_PASS`、`SMTP_PASS`、`EPAY_KEY`、`EPAY_PID`。
3. 禁止将真实凭据写回仓库。

## 兼容性
- `db.php` 保留旧业务变量与常量兼容：`$DB_*` / `DB_DSN` / `DB_USER` / `DB_PWD` / `EPAY_*`。

## 排查建议
- 数据库连接失败：检查 `DB_HOST/DB_PORT/DB_USER/DB_PASS/DB_NAME`。
- 邮件失败：检查 `SMTP_HOST/SMTP_PORT/SMTP_USER/SMTP_PASS/SMTP_SECURE`。
- 支付验签失败：检查 `EPAY_KEY` 与签名串顺序。

# 配套官网目录说明（更新）

## 目录用途
`配套官网` 提供账号注册、密码找回、充值下单/回调、抽奖等 Web 侧业务能力。

## 核心文件
- `db.php`：统一数据库/邮件/支付配置（现已改为优先读取环境变量）。
- `register_compat.php`：注册与验证码流程。
- `reset_password_backend.php`：密码找回与重置。
- `recharge_compat.php`：充值下单。
- `epay_notify.php`：支付异步回调与幂等入账。
- `lottery_draw.php`：抽奖逻辑。

## 安全部署要点
1. 复制 `.env.example` 为环境配置（或在面板/容器注入同名变量）。
2. 生产环境必须填写并保密：`DB_PASS`、`SMTP_PASS`、`EPAY_KEY`、`EPAY_PID`。
3. 禁止把真实凭据写回仓库文件。

## 快速检查
- 若官网报数据库连接错误，先检查 `DB_HOST/DB_PORT/DB_USER/DB_PASS/DB_NAME`。
- 若邮件发送失败，检查 `SMTP_HOST/SMTP_PORT/SMTP_USER/SMTP_PASS/SMTP_SECURE`。
- 若充值回调失败，检查 `EPAY_*` 变量与回调 URL 是否公网可访问。

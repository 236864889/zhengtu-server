<?php
/**
 * db.php
 * 统一数据库 / 邮件 / 支付配置。
 *
 * 安全约定：
 * - 生产凭据必须通过环境变量注入；
 * - 仓库只保留安全默认值与 .env.example 模板；
 * - 禁止再次提交真实生产密码/密钥。
 */

/**
 * 读取环境变量（兼容 Apache/FPM/CLI）。
 */
function env_get(string $key, ?string $default = null): ?string {
    $v = getenv($key);
    if ($v !== false && $v !== '') return $v;
    if (isset($_ENV[$key]) && $_ENV[$key] !== '') return (string)$_ENV[$key];
    if (isset($_SERVER[$key]) && $_SERVER[$key] !== '') return (string)$_SERVER[$key];
    return $default;
}

/* ------------------------------
 * 【数据库连接配置】
 * 建议使用环境变量：DB_HOST/DB_PORT/DB_USER/DB_PASS/DB_NAME/DB_CHARSET
 * ------------------------------ */
$DB_HOST    = env_get('DB_HOST', '127.0.0.1');
$DB_PORT    = (int)env_get('DB_PORT', '3306');
$DB_USER    = env_get('DB_USER', 'root');
$DB_PASS    = env_get('DB_PASS', '');
$DB_NAME    = env_get('DB_NAME', 'FLServerDB');
$DB_CHARSET = env_get('DB_CHARSET', 'latin1');
$DB_DSN     = "mysql:host={$DB_HOST};port={$DB_PORT};dbname={$DB_NAME};charset={$DB_CHARSET}";

/* ------------------------------
 * 【注册限制配置】
 * ------------------------------ */
$REGISTER_IP_LIMIT_MIN = (int)env_get('REGISTER_IP_LIMIT_MIN', '0');

/* ------------------------------
 * 【邮件发送配置】
 * ------------------------------ */
$MAIL_FROM        = env_get('MAIL_FROM', 'no-reply@example.com');
$MAIL_SENDER_NAME = env_get('MAIL_SENDER_NAME', '征途客服中心');
$MAIL_SUBJECT     = env_get('MAIL_SUBJECT', '征途注册验证码');

$SMTP_HOST      = env_get('SMTP_HOST', 'smtp.qq.com');
$SMTP_PORT      = (int)env_get('SMTP_PORT', '465');
$SMTP_USER      = env_get('SMTP_USER', '');
$SMTP_PASS      = env_get('SMTP_PASS', '');
$SMTP_SECURE    = env_get('SMTP_SECURE', 'ssl');
$SMTP_FROM      = env_get('SMTP_FROM', $SMTP_USER ?: 'no-reply@example.com');
$SMTP_FROM_NAME = env_get('SMTP_FROM_NAME', '征途注册中心');
$MAIL_DEBUG     = strtolower((string)env_get('MAIL_DEBUG', 'false')) === 'true';

/* ------------------------------
 * 【XaPay / 易支付 配置】
 * ------------------------------ */
$EPAY_ENABLE_SIGN = strtolower((string)env_get('EPAY_ENABLE_SIGN', 'true')) !== 'false';
$EPAY_KEY         = env_get('EPAY_KEY', '');
$EPAY_PID         = env_get('EPAY_PID', '');
$EPAY_API         = env_get('EPAY_API', 'https://api.zxspro.cn/xpay/epay/');
$EPAY_SITENAME    = env_get('EPAY_SITENAME', '征途私服充值');
$EPAY_RETURN_URL  = env_get('EPAY_RETURN_URL', 'https://example.com/recharge.html');
$EPAY_NOTIFY_URL  = env_get('EPAY_NOTIFY_URL', 'https://example.com/epay_notify.php');

// 1元=多少点（10元→1,000,000点）
$RECHARGE_RATE = (int)env_get('RECHARGE_RATE', '100000');

// ========== 兼容充值模块的常量定义 ==========
if (!defined('DB_DSN')) {
    define('DB_DSN', $DB_DSN);
    define('DB_USER', $DB_USER);
    define('DB_PWD', $DB_PASS);
    define('RECHARGE_RATE', $RECHARGE_RATE);
    define('EPAY_PID', $EPAY_PID);
    define('EPAY_KEY', $EPAY_KEY);
    define('EPAY_NOTIFY_URL', $EPAY_NOTIFY_URL);
    define('EPAY_RETURN_URL', $EPAY_RETURN_URL);
    define('EPAY_API', $EPAY_API);
    define('SITE_NAME', $EPAY_SITENAME);
    define('EPAY_SIGN_STYLE', 'boss');
}

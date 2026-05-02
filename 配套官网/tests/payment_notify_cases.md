# epay_notify 回调验签回归用例

> 目标：覆盖 `success / sign_error / timeout / duplicate` 四类关键回调场景。

## 1. 前置准备

1. 确认 `.env` 已配置：`EPAY_KEY`、`DB_*`。
2. 在 `RECHARGEORDERS` 先准备一条待支付订单（`STATUS=1`）。
3. 确认回调地址可访问：`https://<your-domain>/epay_notify.php`。

## 2. 签名算法说明（与生产逻辑保持一致）

- 去掉 `sign` / `sign_type`
- 去掉空值
- 按键名字典序排序
- 组装 `urldecode(http_build_query(data)) . EPAY_KEY`
- `md5` 小写

可直接使用 `scripts/epay_sign.php` 生成签名。

## 3. 用例 A：成功回调（应返回 success）

```bash
php 配套官网/scripts/epay_sign.php \
  --key "$EPAY_KEY" \
  --data "money=10&name=征途私服充值&out_trade_no=ORD_TEST_001&pid=$EPAY_PID&timestamp=$(date +%s)&trade_no=TN_TEST_001&trade_status=TRADE_SUCCESS&type=alipay"
```

将输出中的 `sign` 带入：

```bash
curl -sS -X POST "https://<your-domain>/epay_notify.php" \
  -d "money=10" \
  -d "name=征途私服充值" \
  -d "out_trade_no=ORD_TEST_001" \
  -d "pid=$EPAY_PID" \
  -d "timestamp=$(date +%s)" \
  -d "trade_no=TN_TEST_001" \
  -d "trade_status=TRADE_SUCCESS" \
  -d "type=alipay" \
  -d "sign=<填入上一步签名>" \
  -d "sign_type=MD5"
```

预期：返回 `success`，且订单状态从 1 变为 2。

## 4. 用例 B：错签（应返回 sign_error）

把 `sign` 改成任意错误值：

```bash
curl -sS -X POST "https://<your-domain>/epay_notify.php" \
  -d "money=10" \
  -d "name=征途私服充值" \
  -d "out_trade_no=ORD_TEST_001" \
  -d "pid=$EPAY_PID" \
  -d "timestamp=$(date +%s)" \
  -d "trade_no=TN_TEST_001" \
  -d "trade_status=TRADE_SUCCESS" \
  -d "type=alipay" \
  -d "sign=bad-sign" \
  -d "sign_type=MD5"
```

预期：返回 `sign_error`。

## 5. 用例 C：超时（应返回 timeout）

`timestamp` 传 10 分钟前：

```bash
OLD_TS=$(( $(date +%s) - 600 ))
php 配套官网/scripts/epay_sign.php \
  --key "$EPAY_KEY" \
  --data "money=10&name=征途私服充值&out_trade_no=ORD_TEST_002&pid=$EPAY_PID&timestamp=${OLD_TS}&trade_no=TN_TEST_002&trade_status=TRADE_SUCCESS&type=alipay"
```

用生成的签名请求回调，预期返回 `timeout`。

## 6. 用例 D：重复回调（应返回 success 且不重复加点）

1. 先执行一次“成功回调”。
2. 用完全相同参数再次回调同一 `out_trade_no`。

预期：第二次仍返回 `success`，但 `POINTBONUS0000.POINT` 不会再次增加。

## 7. 数据库核对SQL（示例）

```sql
SELECT TID, STATUS, PAID_AT, TRADE_NO FROM RECHARGEORDERS WHERE TID IN ('ORD_TEST_001','ORD_TEST_002');
SELECT UID, ACCOUNT, POINT FROM POINTBONUS0000 WHERE ACCOUNT='<测试账号>';
SELECT TID, UID, ACCOUNT, FILLPOINT, THISPOINT FROM CONSUMELOG0000 WHERE TID='ORD_TEST_001';
```

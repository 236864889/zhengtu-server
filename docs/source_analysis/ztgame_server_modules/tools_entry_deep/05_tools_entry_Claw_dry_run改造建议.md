# tools -> Claw / 服务器 dry-run 改造建议

## 目标
将高副作用入口改造成“默认安全、显式放权”的执行模式，支持 Claw 自动化调用。

## 通用改造基线
1. 默认 `--dry-run=true`，仅做参数校验、配置加载、目标连通性预检查（不发业务命令）。
2. 增加 `--input-dir` / `--output-dir`，所有读写限制在沙箱路径。
3. 增加 `--max-steps` / `--max-seconds`，限制循环次数与运行时长。
4. 增加 `--no-db-sideeffect`（协议级开关），禁止建号/改密/创建角色等动作命令。
5. 所有输出加 execution-id（时间戳+随机后缀），便于审计和回滚。

## 分工具建议

### zebraclientService / skill_test_client（高优先）
- 参数层：新增 `--mode=probe|login|pressure`，默认 `probe`。
- 行为层：
  - `probe` 只做握手/登录前校验，不下发角色管理命令；
  - `pressure` 必须显式 `--allow-sideeffect yes` 才可执行。
- 安全层：
  - 默认拒绝私网白名单外地址；
  - 限制 `count` 上限（如 <= 10）。

### zntop
- 增加 `--dry-run`：仅展示一次采集样本，不落盘。
- `--outfile` 必须在 `--output-dir` 子路径，禁止绝对路径直写。
- 增加 `--once` 与 `--max-seconds`，防止无限运行。

### base64 / datetime
- 保持轻量，补齐 `--help` 示例与错误码约定，作为 Claw 接入模板。

## 日志与审计建议
- 固定日志格式：`ts, tool, mode, args_hash, target, effect_flags, result`。
- 记录“本次是否触发副作用命令”的布尔字段。
- 执行结束输出回滚提示（若触发副作用）。

## 适合解决的问题
- “如何在不改业务语义的前提下，把历史工具变成可控自动化任务？”
- “如何将工具执行纳入统一审计与回滚流水线？”

## 待确认
- 协议层是否已有‘只读探测’命令可复用；若无需在 `zebraclient` 协议分发层新增 no-op 探测命令。

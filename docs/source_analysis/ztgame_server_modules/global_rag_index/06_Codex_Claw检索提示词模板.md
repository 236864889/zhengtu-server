# Codex/Claw 检索提示词模板

## 排查启动失败模板
“请先检索 `config/base/super/<目标服务>` 的 S/A 级条目，按启动顺序列出失败断点、配置项和日志定位建议。”

## 排查登录失败模板
“请检索 `fl/gateway/session/bill` 中与 login/auth/tempid/token 相关 S 级条目，给出失败分层与最小验证步骤。”

## 排查任务脚本失败模板
“请检索 `script/newquest/quest/scene` 中 execute_script_event、source XML、NPC脚本映射条目，定位任务不推进原因。”

## 排查充值扣费异常模板
“请检索 `bill/scene/record` 的 charge/consume/point/account S 级条目，按请求路径输出一致性核对清单。”

## 新增二开功能前检索模板
“请先检索目标模块的高风险与二开建议文档，再检索相邻上游/下游模块 S/A 条目，输出变更风险矩阵。”

## 源码修改前风险检索模板
“请检索 `ztgame_server_risk` collection 中相关关键词条目，并列出必须回归的链路与配置。”

## RAG召回结果验证模板
“请按 `source_path/deep_doc/rag_level/module` 校验召回结果，去除无路径或低相关条目，输出待确认点。”

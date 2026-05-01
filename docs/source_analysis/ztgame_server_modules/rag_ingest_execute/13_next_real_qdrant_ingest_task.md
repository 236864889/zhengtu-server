# 下一轮任务：真实 Qdrant/Embedding 小批次导入

## 目标
接入真实 Qdrant 与 embedding 服务，并基于当前 dry-run 产物进行最小风险的小批次验证导入。

## 执行范围
1. 将 `RAG_DRY_RUN=false`。
2. 先导入 `ztgame_server_core` 的 Markdown chunk（小批次）。
3. 每批写入 `ingest_batch_id` 以便追踪。
4. 导入后运行 `07_rag_qa_eval_cases.md` 对应的 11 类评测。
5. 输出真实导入日志与召回结果。
6. 失败则按 `09_ingest_rollback_guide.md` 回滚。

## 交付物
- 真实导入执行日志（含 batch_id、collection、写入条数、失败条数）
- 11 类评测召回报告（top-k 命中、引用文档、失败样例）
- 回滚记录（若发生）

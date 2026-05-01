# RAG 入库回滚说明

> 原则：先导出再删除，先小范围验证再全量执行。

## 1) 按 batch_id 删除
- 条件：某次批次存在系统性错误。
- 操作：根据 `ingest_batch_id=<batch_id>` 过滤删除。
- 建议：先执行“导出待删清单”，确认数量后再删。

## 2) 按 collection 删除
- 条件：某 collection 导入策略错误。
- 操作：按 collection 全量删除后重导。
- 风险：影响该集合所有历史数据，必须先备份。

## 3) 按 ingest_time 删除
- 条件：某时间窗口误入库。
- 操作：按 `ingest_time` 范围过滤删除。
- 建议：用 UTC 时间，避免时区偏差。

## 4) 按 path 前缀删除
- 条件：某目录文档批量错误。
- 操作：按 `path` 前缀（如 `docs/source_analysis/ztgame_server_modules/newquest_deep/`）过滤删除。
- 建议：用于“局部纠偏”，避免清空整个 collection。

## 回滚前导出
- 导出过滤条件命中的 point id、path、chunk_id 到备份文件。
- 记录操作人、时间、过滤条件、命中数量。

## 回滚后抽检
- 用 `07_rag_qa_eval_cases.md` 至少抽检 3 题。
- 确认误数据消失且关键问题召回未退化。
- 回填回滚日志与后续重导计划。

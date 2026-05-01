# RAG 真实试导执行准备状态（第 08 轮）

## 1. 是否真实连接向量库
- 否。当前仅生成 dry-run 与执行模板，未连接任何 Qdrant 实例。

## 2. 是否真实导入
- 否。未执行真实 embedding 与 upsert。

## 3. 生成了哪些脚本
- `02_ingest_dry_run.py`
- `03_ingest_markdown_batch.py`
- `04_ingest_s_level_source_preview.py`
- `08_eval_retrieval_quality.py`

## 4. 生成了哪些说明文档
- `00_rag_env.example`
- `01_qdrant_collections_plan.md`
- `05_collection_batch_ingest_guide.md`
- `06_ingest_log_template.md`
- `07_rag_qa_eval_cases.md`
- `09_ingest_rollback_guide.md`
- `10_rag_ingest_execute_status.md`

## 5. dry-run 能检查哪些问题
- manifest 是否存在/是否可解析
- path 对应文件是否缺失
- collection 级文档分布是否异常
- markdown/source 预切片输出是否为空或过大
- 评测集格式是否可解析为结构化检查表

## 6. 下一步人工要执行哪些命令
1. `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/02_ingest_dry_run.py`
2. `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/03_ingest_markdown_batch.py`
3. `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/04_ingest_s_level_source_preview.py`
4. `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/08_eval_retrieval_quality.py`
5. 人工检查 `out/*.jsonl` 后，再切换 `RAG_DRY_RUN=false` 并接入真实向量库执行正式导入。

## 7. 成功后需要回传什么
- 每批导入日志（使用 `06_ingest_log_template.md`）
- 每批成功/失败数量与错误摘要
- 抽检问答结果（按 `07_rag_qa_eval_cases.md`）
- 如发生回滚：回滚条件、删除数量、回滚后抽检结果

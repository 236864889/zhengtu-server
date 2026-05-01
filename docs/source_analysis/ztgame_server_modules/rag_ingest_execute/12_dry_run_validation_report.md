# dry-run 校验报告

## 1. 执行环境
- 仓库根目录：`/workspace/zhengtu-server`
- 解释器：`python3`
- 时间：2026-05-01 (UTC)

## 2. 运行了哪些脚本
- `02_ingest_dry_run.py`
- `03_ingest_markdown_batch.py`
- `04_ingest_s_level_source_preview.py`
- `08_eval_retrieval_quality.py`

## 3. 02_ingest_dry_run.py 输出摘要
- manifest 行数：108
- collection 分布：
  - `ztgame_server_core`: 65
  - `ztgame_server_script_quest`: 21
  - `ztgame_server_config`: 9
  - `ztgame_server_tools_ops`: 9
  - `ztgame_server_risk`: 4
- level 分布：S=74, A=28, B=6
- missing path：0

## 4. 03_ingest_markdown_batch.py 输出摘要
- 生成文件：`out/markdown_chunks.preview.jsonl`
- 预览行数：224
- 元数据来源：`03_markdown_docs_ingest_list.md`（path -> recommended_collection/recommended_level）

## 5. 04_ingest_s_level_source_preview.py 输出摘要
- 生成文件：`out/s_level_source_chunks.preview.jsonl`
- 预览行数：40
- missing path：0
- 路径抽取正则：`` `(ztgame/[^`]+)` ``
- 元数据来源：`01_global_rag_ingest_manifest.dedup.jsonl`

## 6. 08_eval_retrieval_quality.py 输出摘要
- 生成文件：`out/retrieval_eval_table.jsonl`
- 评测分类行数：11
- 11 类问题解析结果：通过

## 7. preview JSONL 合法性
- `markdown_chunks.preview.jsonl`: rows=224, errors=0
- `s_level_source_chunks.preview.jsonl`: rows=40, errors=0
- `retrieval_eval_table.jsonl`: rows=11, errors=0

## 8. collection 分布是否正确
- dry-run 统计已优先使用 `recommended_collection`，与 dedup manifest 设计一致。
- preview 文件中不再全量写死 core，按清单/manifest 回填 collection。

## 9. missing path 是否存在
- 目前脚本运行结果为 `missing_paths=0`。

## 10. 是否仍保持 dry-run
- 是。仅生成 preview JSONL 与报告。
- 未连接真实 Qdrant。
- 未调用真实 embedding。
- 未伪造入库成功。

## 11. 是否可以进入真实 Qdrant/embedding 接入
- 可以进入“**小批次**真实接入”阶段（先 core markdown，保留 batch_id 与回滚路径）。

## 12. 下一步建议
1. 设置 `RAG_DRY_RUN=false` 前先固定环境变量与目标 collection。
2. 首批仅导入 `ztgame_server_core` 的 markdown 子集（建议 20~50 chunk）。
3. 导入后立即执行 11 类评测并记录 top-k 命中。
4. 若命中率或引用质量不达标，按回滚指南执行并调整切片与关键词。

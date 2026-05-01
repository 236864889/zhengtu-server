# dry-run 脚本修复记录

## 修复文件
- `02_ingest_dry_run.py`
- `03_ingest_markdown_batch.py`
- `04_ingest_s_level_source_preview.py`
- `08_eval_retrieval_quality.py`

## 修复前问题
1. `rag_ingest_execute` 目录及脚本缺失，无法执行 dry-run 校验。
2. collection 统计可能只读 `collection` 字段，未兼容 `recommended_collection`。
3. S 级源码路径若用宽泛正则，可能受 Markdown 表格尾部 `|` 干扰。
4. Markdown/source preview 元数据可能写死 core，未按清单回填。
5. 评测模板解析需要确认可稳定抽取 11 类问题。

## 修复后行为
1. 新增 dry-run 脚本并统一 ROOT 为仓库根目录（`Path(__file__).resolve().parents[4]`）。
2. `02_ingest_dry_run.py` 使用：
   - `collection = row.get("recommended_collection") or row.get("collection") or "unknown_collection"`
3. `04_ingest_s_level_source_preview.py` 使用严格正则：
   - ``re.search(r"`(ztgame/[^`]+)`", line)``
4. `03_ingest_markdown_batch.py` 从 `03_markdown_docs_ingest_list.md` 回填 `recommended_collection/recommended_level`。
5. `04_ingest_s_level_source_preview.py` 从 dedup manifest 回填 `module/domain/recommended_collection/keywords/reason`。
6. `08_eval_retrieval_quality.py` 解析评测模板二级标题并输出 JSONL 表。

## 验证命令
- `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/02_ingest_dry_run.py`
- `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/03_ingest_markdown_batch.py`
- `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/04_ingest_s_level_source_preview.py`
- `python3 docs/source_analysis/ztgame_server_modules/rag_ingest_execute/08_eval_retrieval_quality.py`
- JSONL 合法性检查（逐行 `json.loads`）

## 是否需要人工复核
- 需要：建议人工 spot-check 10~20 条 preview（尤其是超长源码与中文文档边界切片）后，再进入真实 Qdrant/embedding 小批次导入。

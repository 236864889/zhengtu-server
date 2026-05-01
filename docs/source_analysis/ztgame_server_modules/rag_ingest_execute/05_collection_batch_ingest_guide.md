# Collection 分批导入指南（执行版）

## 批次顺序
1. `ztgame_server_core`（小批次先行）
2. `ztgame_server_config`
3. `ztgame_server_script_quest`
4. `ztgame_server_tools_ops`
5. `ztgame_server_risk`

## 每批导入前检查
- 校验 manifest 存在且无 JSON 格式错误
- 执行 dry-run，确认缺失文件列表
- 确认 `RAG_DRY_RUN=true` 时仅生成 preview
- 记录 `batch_id` 和执行人

## 每批导入后检查
- 核对 input/chunk/success/fail 数量
- 抽检至少 3 个问题（对应本批主要场景）
- 保存导入日志到批次记录文件

## 失败回滚
- 任一批次 `fail_count > 0` 且影响召回时，停止下一批
- 依据 `batch_id` 执行删除回滚（见 `09_ingest_rollback_guide.md`）
- 回滚后再执行抽检，确认脏数据已清除

## 日志记录
- 使用 `06_ingest_log_template.md`
- 每批必须单独记录，不得覆盖历史

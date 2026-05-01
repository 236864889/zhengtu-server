# Qdrant Collections 规划建议（试导阶段）

> 状态：仅规划，不连接真实向量库。

## 通用建议
- 距离：`Cosine`
- 向量维度：`<EMBEDDING_DIM>`（由实际 embedding 模型确认后替换）
- 索引：默认 HNSW；数据稳定后再调 `m`、`ef_construct`
- payload 建议字段：
  - `path`
  - `collection`
  - `module`
  - `level`
  - `title`
  - `chunk_id`
  - `source_type`（markdown/source）
  - `line_start`
  - `line_end`
  - `ingest_batch_id`
  - `ingest_time`

## ztgame_server_core
- 用途：核心链路（启动、网关、场景、会话、角色主流程）
- 文档数量：以 `01_global_rag_ingest_manifest.dedup.jsonl` 中该 collection 统计为准
- 建议向量维度占位：`<EMBEDDING_DIM>`
- payload 字段：通用字段 + `server_role`
- 优先导入级别：P0（首批）
- 抽检问题：启动失败、登录失败、进场景失败

## ztgame_server_config
- 用途：配置加载、区服配置、参数漂移问题
- 文档数量：manifest 统计
- 建议向量维度占位：`<EMBEDDING_DIM>`
- payload 字段：通用字段 + `config_key`
- 优先导入级别：P1（第二批）
- 抽检问题：区服列表异常、配置漂移

## ztgame_server_script_quest
- 用途：Lua 任务、NPC 逻辑、任务推进问题
- 文档数量：manifest 统计
- 建议向量维度占位：`<EMBEDDING_DIM>`
- payload 字段：通用字段 + `quest_id` / `npc_name`
- 优先导入级别：P1（第三批）
- 抽检问题：Lua 脚本异常、任务不推进

## ztgame_server_tools_ops
- 用途：工具链、运维脚本、发布排障 SOP
- 文档数量：manifest 统计
- 建议向量维度占位：`<EMBEDDING_DIM>`
- payload 字段：通用字段 + `tool_name`
- 优先导入级别：P2（第四批）
- 抽检问题：工具误用风险、日志提取失败

## ztgame_server_risk
- 用途：高风险清单、风险案例、资金一致性与安全边界
- 文档数量：manifest 统计
- 建议向量维度占位：`<EMBEDDING_DIM>`
- payload 字段：通用字段 + `risk_type` / `severity`
- 优先导入级别：P2（最后批次）
- 抽检问题：充值扣费异常、越权调用风险

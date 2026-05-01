# RAG 入库准备状态报告

1. 是否成功读取原始 global manifest：是
2. 原始行数：108
3. 去重后行数：108
4. 重复行数：0
5. collection 拆分结果：
- ztgame_server_core: 65
- ztgame_server_config: 9
- ztgame_server_script_quest: 21
- ztgame_server_tools_ops: 9
- ztgame_server_risk: 4
6. Markdown 文档数量：157
7. S 级源码文件数量：40
8. 是否发现 missing path：否
9. 是否可以进入真实向量库导入：是（前提：使用 dedup manifest 且按 collection 分批）
10. 下一步建议：先以 Markdown collection 进行小批次试导与抽检，再执行源码批次导入。

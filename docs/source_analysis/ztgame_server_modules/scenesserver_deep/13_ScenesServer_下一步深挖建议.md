# 下一步深挖建议
- 优先目录：`ztgame/RecordServer/`
- 原因：ScenesServer 的角色保存、离线恢复、日志回写、邮件/交易异步结果都经 Record 链路回流；若不先解 Record，Scene 下线/切图一致性只能看到半链路。
- 建议重点：
  1) Scene->Record 命令矩阵；
  2) 角色持久化事务边界；
  3) 失败重试与补偿策略；
  4) 与 Session 的回执顺序一致性。

# 可复用调试模板与风险点（面向 Claw/服务器）

## 可复用（建议）
1. 网络骨架模板：`TCPClient.cpp` / `TCPServer.cpp`
   - 抽取连接、收发、task 分发框架；替换为本地 loopback 与假命令。
2. 线程池模板：`ThreadPoolTest.cpp`
   - 抽取 `zTCPTaskPool` 初始化与任务投递流程，禁真实 listen。
3. 配置解析模板：`confiletest.cpp` + `includefiletest.cpp`
   - 作为只读解析检查与 XML include 完整性检查。

## 需脱敏/改 dry-run
- MySQL 样例（`MySqlTest.cpp`、`NewMySqlTest.cpp`）：
  - 必须替换连接串、禁写 SQL、限次执行。
- 网络样例：
  - 必须替换内网地址/端口，增加超时、退出条件、节流。

## 高风险点
- 网络：阻塞 accept、无限发送循环。
- 数据库：明文凭据、潜在写库。
- 线程：资源回收依赖手工 final/join，异常路径不健壮。

## 交给 Claw/服务器执行建议
- Codex 本轮仅静态分析，不执行任何二进制。
- Claw 实测建议顺序：
  1) 先配置/XML只读样例；
  2) 再线程 dry-run；
  3) 最后网络 loopback；
  4) DB 样例仅在隔离库且只读模式。

## 适合解决的问题
- “如何把历史 test 变成安全的诊断脚手架？”
- “哪些样例可最快转成最小可复现实验？”

## 待确认
- `entrytest.cpp`、`fsmtest.cpp` 的真实业务上下文。
- 下一步建议追 `base` 命令结构头文件与历史操作文档映射。

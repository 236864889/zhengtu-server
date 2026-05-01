# base 服务生命周期
## 1. 范围
`zService zNetService zSubNetService Zebra`
## 2. 主流程
`zService::main`:
1) `init()`：读取环境变量、注册 SIGINT/SIGTERM/SIGHUP、忽略 SIGPIPE、初始化随机种子。
2) `validate()`：服务自检（可重载）。
3) `while(!terminate) serviceCallback()`。
4) `final()`：释放资源。
## 3. 派生差异
- `zNetService`：在 `init(port)` 创建 `zTCPServer` 并 bind，`serviceCallback` 执行 accept/newTCPTask。
- `zSubNetService`：增加 `SuperClient` 外连 super server、注册与 serverList 管理、`sendCmdToSuperServer`。
## 4. 上下游
- 上游：各服务 main/daemon 启动入口。
- 下游：网络 task pool、logger、全局配置。
## 5. 风险
- `final()` 为纯虚，子类遗漏清理会泄漏。
- SIGHUP `reloadConfig` 默认仅日志，热更可能“看似成功实则未应用”。
## 6. RAG
- level: S
- keywords: zService::main,init,validate,serviceCallback,final,zSubNetService

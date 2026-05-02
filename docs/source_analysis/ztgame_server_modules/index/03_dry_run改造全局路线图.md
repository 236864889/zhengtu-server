# dry-run 改造全局路线图

## 目标
在“不执行真实副作用”的前提下，为 test/tools/server/config/launcher/payment/RAG 建立统一 dry-run 能力。

## 路线
1. test dry-run（P0）
   - 对 `ztgame/test` 样例增加 `DRY_RUN=1` 分支，禁止写库/发包。
2. tools dry-run（P0）
   - 对高风险入口（含 `zebraclientService`、`skill_test_client`）增加参数校验与 no-op。
3. 服务器启动 dry-run（P1）
   - `start.sh/start1.sh` 增加 `--plan` 模式，仅输出将启动的进程序列。
4. 配置检查 dry-run（P1）
   - 校验 `ServerList/XML/DB` 文件存在性、字段完整性、跨文件引用。
5. 登录器 dry-run（P1）
   - 仅做配置解析与地址可达性模拟，不发起真实登录。
6. 支付/官网 dry-run（P0）
   - 所有账变接口采用 mock endpoint；敏感配置外置并去敏。
7. RAG 入库 dry-run（P2）
   - 复用 `rag_ingest_execute` 现有 dry-run 链，仅离线切片与清单校验。

## 依赖关系
- P0：安全与副作用隔离（tools/test/payment）。
- P1：启动与配置一致性。
- P2：RAG 入库自动化与检索质量验证。

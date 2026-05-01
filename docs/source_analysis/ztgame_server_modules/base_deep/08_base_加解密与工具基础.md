# base 加解密与工具基础
## 1. 范围
`EncDec/ CDes zBase64 zMisc zArg zTime zTimer zThread`
## 2. 关键点
- `EncDec` 提供 blowfish/md5/des/idea/rc5 及统一 `EncDec` 接口，构建为 `libencdec.a`。
- `zBase64` 是“异或 + l64a/a64l”定制编码，不是标准 base64。
- `zThread` 在线程入口调用 `mysql_thread_init/end`，说明线程与 DB runtime 强耦合。
- `zTimer` 提供 delay/rate 两种调度语义；`zTime` 提供耗时统计工具。
- `zArg` 封装 argp，统一服务命令行参数收集。
## 3. 风险
- 加解密算法老旧，涉及登录/支付链路前严禁随意替换。
- `zBase64` 对二进制与长度边界敏感。
## 4. RAG
- level: A
- keywords: EncDec,CDes,zBase64,zThread,mysql_thread_init,zTimer,zTime,zArg

# base 日志系统
## 1. 分析范围
- 源码目录：`ztgame/base/`
- 重点文件：`zLogger.h/.cpp`、`Zebra.h/.cpp`
- 本文不覆盖：业务层日志埋点

## 2. 核心结论
- 全局日志入口是 `Zebra::logger`，生命周期由全局构造/析构管理。
- `zLogger` 封装 log4cxx，支持等级映射、文件滚动与控制台输出。

## 3. 风险点
- 编译风险：log4cxx 版本 ABI 差异。
- 运行风险：高频路径打印过多导致 I/O 抖动。
- 二开风险：日志中泄漏帐号、IP、SQL 明文。

## 4. RAG 入库建议
- level：A
- chunk_hint：按“入口/等级/appenders/二开安全”分块
- tags：base,logger,log4cxx,risk

## 5. 适合解决的问题
- 如何统一日志等级并保留历史格式
- 如何在网络/DB高频函数安全加日志

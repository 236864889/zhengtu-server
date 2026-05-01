# base 公共库重点确认清单（面向上线前核查）

## A. 构建与产物
- [x] `base/Makefile` 产出 `libzebra.a`。
- [x] 根 `Makefile` 先编译 `base/EncDec`，再编译 `base`。
- [ ] `libcrypto.a` 的生成脚本与产物路径逐行确认（待补证据）。

## B. 网络服务稳定性
- [x] `zSocket` 提供 `recvToCmd/sendCmd/checkIOForRead/checkIOForWrite` 等核心 IO 方法。
- [x] `zSubNetService` 继承 `zNetService`，作为业务服常见父类。
- [ ] `zSubNetService::validate` 的失败分支（断线、重连、错误码）需补充函数级路径图。

## C. 并发与任务池
- [x] `zTCPClientTaskPool` 具备 `zThreadGroup` + `tasks` 容器架构。
- [ ] 不同服务的线程池大小、队列阈值、背压策略对照表待补。
- [ ] 高峰压测下 `zTCPTaskPool` 丢包/延迟指标待补。

## D. 配置与热更新
- [x] `zConfile`/`zProperties`/`zXMLParser` 组成配置基础链。
- [x] `BillService::reloadConfig()` 存在热加载入口。
- [ ] 其它服务是否具备同等级热加载入口需逐项核验。

## E. 数据库与连接池
- [x] `zDatabaseManager` + `zDatabase` + `zMysqlDBConnPool` 主链存在。
- [ ] 连接池异常（断网/超时）下自动恢复策略与日志级别需补证据。

## F. 日志与可观测性
- [x] `zLogger` 支持 `fatal/error/warn/info/debug/trace`。
- [ ] 各服务日志级别默认值与切换策略待统一清单。

## G. 二开高风险文件（建议 code owner）
1. `base/zSocket.*`
2. `base/zSubNetService.*`
3. `base/zTCPTaskPool.*`
4. `base/zMysqlDBConnPool.*`
5. `base/zLogger.*`

## H. 建议补齐的自动化检查
- 编译后校验：`libzebra.a/libencdec.a/libcrypto.a` 是否存在。
- 启动自检：服务端口绑定、数据库连接可达、配置文件完整性。
- 压测自检：任务池队列长度、平均延迟、错误码分布。

## I. 结论
- base 层是全服务共享“地基模块”，任何小改动都可能放大全局影响。
- 二开时应先补证据（调用图/压测/日志）再改核心路径。

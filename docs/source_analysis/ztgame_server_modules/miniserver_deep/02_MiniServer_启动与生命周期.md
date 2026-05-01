# MiniServer 启动与生命周期

## 启动入口
- `main` 初始化 logger/default global，解析 `MiniServer` 配置后进入 `MiniService::main()`。

## 继承关系
- `MiniService : zSubNetService`，复用 base 生命周期骨架 `init -> validate -> serviceCallback loop -> final`。

## init 细链路
1. 初始化 DB 连接池并注入 mysql URL。
2. 初始化 MetaData（用于 `MINIGAME` 字段映射）。
3. 初始化 `zTCPTaskPool(threadPoolCapacity, state)`。
4. 根据 ifname 推导本机 IP。
5. 启动 `MiniTimeTick` 线程。
6. `MiniHall::init()` 读取 Mini.xml 并建房。
7. `zSubNetService::init()` 完成监听与 Super 侧链路。

## validate/serviceCallback/final
- `validate/serviceCallback` 未在 MiniService 显式覆写，走基类默认逻辑。
- `newTCPTask` 为每个接入连接创建 `MiniTask` 并入 verify 队列。
- `final` 顺序：停止 TimeTick -> 销毁 taskPool -> 删除 Task/User manager -> `zSubNetService::final` -> 释放 DB。

## 启动失败点
- mysql URL 无效、MetaData init 失败。
- threadPoolCapacity 配置异常导致 taskPool init 失败。
- `MiniHall::init` 失败（Mini.xml 缺失或格式错误）。
- `zSubNetService::init` 失败（监听/Super 接入失败）。

## 是否注册 Super
- 是（框架语义上是）。证据：继承 `zSubNetService`，并接受 `--server --port` 参数，且调用 `zSubNetService::init/final`。

## RAG 标签
- `main`, `zSubNetService`, `init`, `final`, `newTCPTask`, `MiniTimeTick`

## 适合解决的问题
- MiniServer 启动即退出。
- 启动后不接连接/未出现在 Super serverlist。

## 待确认
- validate 实际行为是否由 base 配置改变（下一步看 `base/zSubNetService.cpp` 当前分支版本）。

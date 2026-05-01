# SessionServer 配置与数据库关系

## 配置读取
- 读取 `Zebra::global` 键：`mysql`、`ifname`、`threadPoolCapacity`、`initThreadPoolState`、以及 dare/quiz 参数等。
- `zSubNetService` 层负责服务注册/ServerList 相关基础能力（Session 本层调用 `getServerEntryByType(RECORDSERVER)`）。

## 端口/IP/服务名
- Session 自身 IP 从网卡名解析：`zSocket::getIPByIfName(global["ifname"])`。
- Record 目标地址来自 Super 服务表：`serverEntry->pstrExtIP/wdExtPort`。

## DB 使用
- `zDBConnPool` + `MetaData` 是统一入口。
- 本目录多个模块直接执行 `exeSelect/exeInsert/exeUpdate/exeDelete`。
- 明确表线索：`ALLY/SEPT/SEPTMEMBER/RECOMMENDSUB/BOSSINFO` 等。

## 配置缺失表现
- mysql 或 metadata 初始化失败直接 `init=false`。
- 找不到 RECORDSERVER 或连接失败直接启动失败。
- 数值配置缺失会 fallback 默认值（dare/quiz）。

# MiniServer 配置与数据库关系

## 配置读取
- 启动参数支持：`daemon/log/logfilename/mysql/ifname/server/port`。
- `MiniConfile::parse("MiniServer")` 从统一配置体系加载全局键。
- `MiniHall::init()` 读取 `Mini.xml` 根节点 `Mini`，构建 room/game 数量与门槛。

## 监听端口/服务类型
- 监听端口由 zSubNetService 体系读取 global `port`。
- 服务类型由 Super 注册时的 serverType 决定；接入连接校验依赖 `wdServerType + wdServerID + IP`。

## 数据库表与字段
- 逻辑表名：`MINIGAME`（通过 `MetaData::getFields("MINIGAME")` 获取字段定义）。
- 关键字段可见：`CHARID`, `money`，以及 `MiniGameScore` 展开字段（win/lose/draw/score 等，待结合 metadata 表确认）。

## SQL/DB操作模式
- 读：登录 `exeSelect(where CHARID)`。
- 插：首次用户 `exeInsert`。
- 更：`exeUpdate`（save、addDBMoney、draw回滚）。
- 删：`PARA_GATE_USER_DELETE -> exeDelete`。

## 配置缺失表现
- mysql 缺失：init 直接失败退出。
- Mini.xml 缺失或 root 不匹配：MiniHall init 失败，服务不可用。
- server/port 错误：可能导致无法接入 Super 或无法被 Gateway/Scene 校验通过。

## RAG 标签
- `MiniConfile`, `Mini.xml`, `MINIGAME`, `exeSelect`, `exeUpdate`, `serverEntry`

## 适合解决的问题
- Mini 金币数据落库失败。
- 配置迁移后 Mini 无法对接 Gateway/Scene。

## 待确认
- `MINIGAME` 完整字段清单（下一步查 metadata 初始化源或建表 SQL）。

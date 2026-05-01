# 征途服务端源码深度分析：跨服务关系/协议/配置/数据库/Lua

## 1) Client / Manager / Task 调用关系（已证实）
- Gateway 模块包含 `SceneClient/SceneClientManager`、`SessionClient`、`RecordClient`、`BillClient`、`GatewayTask/GatewayTaskManager`、`GateUser/GateUserManager`，可确认网关承担“接入 + 多下游连接 + 任务分发”。
- Scenes 模块包含 `SessionClient`、`RecordClient`、`MiniClient`、`SceneTask/SceneTaskManager`、`SceneUser/SceneUserManager`、`SceneNpcManager`。
- Super 模块包含 `FLClientManager`、`RoleregClientManager`、`InfoClientManager` 与 `ServerTask`。
- Session 模块 `SessionTask.cpp` 内部将 `stNullUserCmd` 分发到多个子系统管理器：`Gift/CNpcDare/CQuiz/CDare/CSchool/CUnion/CRelation/CSept/CCountry/CArmy/Recommend/CAlly/CGem/CVote`。

## 2) 协议命令头与 Cmd 结构关系
- base 协议头清单：`Command.h`、`SceneCommand.h`、`SessionCommand.h`、`RecordCommand.h`、`SuperCommand.h`、`FLCommand.h`、`BillCommand.h`、`InfoCommand.h`、`RoleregCommand.h`、`GmToolCommand.h`。
- `SuperServer/ServerTask.cpp` 同时 include 多个命令头，并使用 `Cmd::Super / Cmd::RoleReg / Cmd::GmTool` 命名空间，证明其是跨协议汇聚点。
- `SessionTask.cpp` include `SessionCommand.h`，并显式使用 `Cmd::Session`；大量分支处理 `Cmd::stNullUserCmd`。
- `SceneTask.cpp`、`RecordTask.cpp`、`BillTask.cpp`、`GatewayTask.cpp` 均存在 `verifyLogin`/`verifyConn` 结构，体现连接鉴权 + 命令分发范式。

## 3) 配置文件读取入口
- `SessionServer.cpp`/`SuperServer.cpp` 定义 `*Confile : public zConfile`，并在 main 前后调用 `parse("SessionServer")` / `parse("SuperServer")`。
- `GameConfigMgrX::init()` 使用 `zXMLParser` 加载配置。
- `ScenesServer/fjconfig.cpp` 有大量 `init*()`，每个函数都通过 `zXMLParser` 读取对应 XML。
- `ScenesServer/Scene.cpp`、`Rush.cpp`、`QuestEvent.cpp`、`WayPoint.cpp` 等直接调用 `zXMLParser` 解析 map/scene/quest 相关 XML。

## 4) 数据库访问链路
- 基础层：`base/zDatabase*`、`zMysqlDBConnPool*` 提供连接池与数据库抽象。
- 业务层：`SessionServer` 中 `CUnion.cpp`、`CSept.cpp` 可见 `SessionService::dbConnPool->exeSelectLimit(...)` 的直接查询调用。
- 由此可确认“base 连接池 -> Session 业务管理器 SQL 调用”主链已打通；其他服务 SQL 细节待确认。

## 5) Lua / luabind / quest / newquest 绑定关系
- `script/lua.h` 明确 Lua 5.1。
- `ScenesServer/Makefile` 链接 `-L../script -llua -lluabind`，可确认场景服直接依赖 Lua+luabind。
- `ScenesServer/script_func.cpp` include `Command.h`，说明脚本导出函数与命令体系存在桥接。
- 仓库有 `quest/` 与 `newquest/`（含多个 `define*.lua`），说明脚本数据存在“旧任务+新任务”并行结构；行为冲突风险待确认。

## 6) 二开常用入口与高风险文件
- 常用入口：
  - 网关入口：`GatewayServer.cpp` + `GatewayTask.cpp`
  - 场景核心：`ScenesServer.cpp` + `SceneTask.cpp` + `SceneUser.cpp`
  - 会话路由：`SessionTask.cpp`
  - 配置系统：`fjconfig.cpp`、`GameConfigMgrX.cpp`
- 高风险文件：
  - `SessionTask.cpp`（多分支命令分发，改动容易引发全局联动）
  - `ScenesServer.cpp` / `fjconfig.cpp`（初始化链超长，配置加载失败易阻塞启动）
  - `SuperServer/ServerTask.cpp`（跨协议汇聚）

## 7) 待确认
- `UserServer` 组件在当前源码树未见目录，可能来自部署目录或历史拆分产物。
- 各服务端口号来源（Confile具体键）需进一步逐项抽取。

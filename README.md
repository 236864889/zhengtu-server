# 征途服务端源码结构总览

## 项目总体结构

- `mysql`：目录级说明见 `mysql/README.md`（如存在）。
- `ztgame`：目录级说明见 `ztgame/README.md`（如存在）。
- `ztgame.dat`：目录级说明见 `ztgame.dat/README.md`（如存在）。
- `配套官网`：目录级说明见 `配套官网/README.md`（如存在）。

## 各服务端模块关系（基于目录命名与接口文件，未推断业务细节）
- `ztgame/base` 提供基础网络、线程、数据库、协议定义，被各服务端目录复用。
- `ztgame/ScenesServer`、`ztgame/SessionServer`、`ztgame/GatewayServer`、`ztgame/FLServer`、`ztgame/SuperServer`、`ztgame/BillServer`、`ztgame/MiniServer`、`ztgame/RecordServer` 组成核心服务集群，互联关系需结合配置文件进一步核验（待确认）。
- `ztgame/Config`、`ztgame/quest`、`ztgame/newquest`、`ztgame/script` 等提供配置/脚本/表数据输入。

## 编译顺序（建议）
1. `ztgame/base`（公共库/基础组件）
2. 各独立服务：`BillServer`、`FLServer`、`GatewayServer`、`MiniServer`、`RecordServer`、`SessionServer`、`SuperServer`、`ScenesServer`
3. `ztgame/tools`、`ztgame/test`（辅助工具与测试程序）
4. 根 `ztgame/Makefile` 进行总控构建（待确认）

## 启动顺序（建议，待确认）
1. 数据库与基础依赖（MySQL等）
2. `FLServer` / `SuperServer`（全局路由与服列表）
3. `SessionServer` / `RecordServer` / `BillServer`（会话、日志、计费）
4. `ScenesServer`（场景逻辑）
5. `GatewayServer`（玩家入口）
6. `MiniServer`（小游戏，可选）

## 依赖库（从源码文件名可见）
- MySQL 相关封装：`zDatabase*`、`zMysqlDBConnPool.cpp`
- Lua 相关头文件：`ztgame/script/lua*.h`、`luabind/`
- XML 解析：`zXMLParser.*`、多处 `*.xml` 配置
- 网络/线程：`zTCP*`、`zThread*`、`zNetService*`

## 已知历史兼容点（待确认）
- 同时存在 `start.sh` 与 `start1.sh`，可能对应不同部署形态。
- `myflies` 下存在 Visual Studio 工程文件，可能用于历史 Windows 构建链。
- 存在多个版本化 Lua 定义文件（如 `define497.lua`、`define820.lua` 等），疑似跨版本兼容。

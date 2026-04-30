# ztgame/GatewayServer 目录说明

## 目录用途
- `GatewayServer`：根据命名推断用途如下，实际行为需结合代码确认（待确认）。
- 推断：网关接入层。

## 核心文件说明
- `BillClient.cpp`：用途待确认。
- `BillClient.h`：用途待确认。
- `CmdChecker.cpp`：用途待确认。
- `CmdChecker.h`：用途待确认。
- `CountryInfo.cpp`：用途待确认。
- `CountryInfo.h`：用途待确认。
- `GateUser.cpp`：用途待确认。
- `GateUser.h`：用途待确认。
- `GateUserManager.cpp`：用途待确认。
- `GateUserManager.h`：用途待确认。

## 主要模块职责
- 主要由当前目录下源码文件直接构成。

## 与其他目录/服务的关系
- 关系依据包含 `*Client*`/`*Manager*`/命令头文件命名推断，详细调用链待确认。

## 编译或运行时是否参与
- 判断：是。
- 若包含 `Makefile`，通常会直接参与构建。

## 常见风险点
- 协议/结构体变更引发跨服务兼容问题。
- 配置热更新或重启策略不明导致发布风险。
- 历史脚本与新逻辑并存导致行为分叉。

## 后续二开修改建议
- 对外协议先做版本化，再推进功能扩展。
- 先在 `test/` 或 `tools/` 补充验证程序。
- 为每次改造建立“配置-代码-启动参数”三方核对清单。

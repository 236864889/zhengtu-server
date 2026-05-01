# 全局 RAG 总览

## 1. 已覆盖模块
- base、SessionServer、GatewayServer、ScenesServer、RecordServer、BillServer、SuperServer、FLServer、MiniServer、Config、script、newquest、quest、tools。
- 对应 deep 目录：
  - `docs/source_analysis/ztgame_server_modules/base_deep/`
  - `docs/source_analysis/ztgame_server_modules/sessionserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/gatewayserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/scenesserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/recordserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/billserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/superserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/flserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/miniserver_deep/`
  - `docs/source_analysis/ztgame_server_modules/config_deep/`
  - `docs/source_analysis/ztgame_server_modules/script_deep/`
  - `docs/source_analysis/ztgame_server_modules/newquest_deep/`
  - `docs/source_analysis/ztgame_server_modules/quest_deep/`
  - `docs/source_analysis/ztgame_server_modules/tools_deep/`

## 2. 未覆盖模块（待补齐）
- `ztgame/test/`（待确认）
- `ztgame/myflies/`（待确认）
- `ztgame/Config` 子专题函数级与变更影响图（待确认）

## 3. 每个模块一句话职责
- base：公共运行时能力（网络/服务/日志/DB/XML/协议/加解密）基座。
- SessionServer：账号会话与协议分发中心。
- GatewayServer：外网接入、连接管理与前置限流。
- ScenesServer：核心玩法、AOI、战斗、任务事件执行主场。
- RecordServer：角色持久化、查询与保存链路。
- BillServer：充值/扣费/点数账务处理。
- SuperServer：服务注册发现、全局控制与广播。
- FLServer：区服列表与登录入口编排。
- MiniServer：小游戏相关服务逻辑与交互。
- Config：全局配置、端口、数据库、ServerList映射。
- script：Lua VM、luabind、Cpp-Lua桥接执行层。
- newquest：新任务 Lua 脚本体系。
- quest：任务 XML 数据与引擎事件配置体系。
- tools：SQL/资源转换/运维发布回滚工具链。

## 4. RAG 入库总体策略
1) 先入 deep 文档与全局 manifest；2) 再补 S 级源码片段；3) 最后补 A/B级背景文档。
- collection 建议：
  - `ztgame_server_core`
  - `ztgame_server_config`
  - `ztgame_server_script_quest`
  - `ztgame_server_tools_ops`
  - `ztgame_server_risk`

## 5. S/A/B/C 等级说明
- S：主链路关键节点、上线风险高、排障优先级最高。
- A：高频排障与联调核心文档。
- B：背景机制、次要分支链路。
- C：低频参考、历史说明或补充材料。

## 6. 主链路总览
登录入口（FL/Gateway）→ Session 鉴权与会话 → Scene 入场与玩法执行 → Record 持久化 → Bill 经济链路 → Super 全局注册发现与调度。

## 7. 二开风险总览
- 配置漂移（Config/ServerList/端口矩阵不一致）。
- 协议改动未同步（Gateway/Session/Scene）。
- 脚本行为与 XML 事件不一致（script/newquest/quest）。
- 工具误用导致资源或 DB 污染（tools）。

# RAG 问答评测集（正式样例）

## 1) 启动失败
- 问题：GameServer 启动后立即退出，如何定位初始化失败点？
- 期望召回 collection：`ztgame_server_core`
- 期望召回模块：启动生命周期、配置初始化、数据库初始化
- 期望关键词：`main`, `init`, `load config`, `db connect`
- 期望引用文档：`01_服务入口与生命周期.md`
- 验收标准：Top5 至少 2 条命中启动流程与失败日志关键词。

## 2) 登录失败
- 问题：玩家账号验证通过后仍提示登录失败，可能卡在哪个链路？
- 期望召回 collection：`ztgame_server_core`
- 期望召回模块：Gateway、Session、认证转发
- 期望关键词：`Gateway`, `Session`, `auth`, `login`
- 期望引用文档：`BillServer_Gateway登录验证链路`
- 验收标准：Top5 至少 1 条包含 Gateway->Session 调用路径。

## 3) 网关转发失败
- 问题：网关收到包但 Scene 未收到，如何判断是路由还是协议问题？
- 期望召回 collection：`ztgame_server_core`
- 期望召回模块：网关路由、协议解析、Scene 通信
- 期望关键词：`route`, `packet`, `command`, `dispatch`
- 期望引用文档：`10_协议与命令关键词分析.md`
- 验收标准：返回内容包含至少 1 个协议/命令排障关键词。

## 4) 进场景失败
- 问题：登录后角色无法进入场景，常见阻断点有哪些？
- 期望召回 collection：`ztgame_server_core`
- 期望召回模块：Scene 入口、角色状态校验
- 期望关键词：`enter scene`, `player state`, `map`, `transfer`
- 期望引用文档：`04_关键词索引矩阵.md`
- 验收标准：Top5 命中至少 2 个场景入场相关关键词。

## 5) 角色保存失败
- 问题：角色下线保存偶发失败，需要优先排查哪些数据链路？
- 期望召回 collection：`ztgame_server_core`
- 期望召回模块：RecordServer 保存链路、DB SQL 映射
- 期望关键词：`save`, `persist`, `flush`, `sql`
- 期望引用文档：`RecordServer_Scene持久化链路`
- 验收标准：至少命中 1 条持久化流程文档 + 1 条 SQL 相关文档。

## 6) 充值扣费异常
- 问题：充值到账与扣费流水不一致，如何快速定位边界？
- 期望召回 collection：`ztgame_server_risk`
- 期望召回模块：BillServer 资金一致性
- 期望关键词：`bill`, `charge`, `consume`, `consistency`
- 期望引用文档：`账号点数与资金一致性边界`
- 验收标准：Top5 中至少 2 条指向资金一致性与风险边界。

## 7) 区服列表异常
- 问题：客户端拉取区服列表为空，先检查哪些配置链路？
- 期望召回 collection：`ztgame_server_config`
- 期望召回模块：配置读取、区服配置、发布配置
- 期望关键词：`server list`, `config`, `load`, `region`
- 期望引用文档：`05_配置文件读取链路分析.md`
- 验收标准：返回文档必须包含配置读取入口与路径提示。

## 8) Lua脚本异常
- 问题：任务 Lua 脚本报错后 NPC 对话中断，如何追踪绑定链路？
- 期望召回 collection：`ztgame_server_script_quest`
- 期望召回模块：Lua 绑定、Quest 事件
- 期望关键词：`lua`, `quest`, `npc`, `bind`
- 期望引用文档：`07_Lua脚本绑定与任务系统分析.md`
- 验收标准：Top5 至少 2 条涉及 Lua 绑定函数或任务事件。

## 9) 任务不推进
- 问题：玩家完成条件后任务状态不更新，可能是哪些节点未触发？
- 期望召回 collection：`ztgame_server_script_quest`
- 期望召回模块：任务状态机、事件触发、奖励流程
- 期望关键词：`quest state`, `trigger`, `event`, `reward`
- 期望引用文档：`newquest_任务XML与Lua函数映射`
- 验收标准：召回结果需覆盖任务条件与状态推进两个层面。

## 10) 工具误用风险
- 问题：运维脚本批量执行后出现异常变更，如何核对工具风险边界？
- 期望召回 collection：`ztgame_server_tools_ops`
- 期望召回模块：运维工具、脚本执行规范
- 期望关键词：`tool`, `ops`, `batch`, `risk`
- 期望引用文档：`高风险点与二开建议`
- 验收标准：至少返回 1 条操作风险与 1 条回退建议。

## 11) 配置漂移
- 问题：同版本多区配置表现不一致，如何识别配置漂移？
- 期望召回 collection：`ztgame_server_config`
- 期望召回模块：配置版本、加载顺序、覆盖关系
- 期望关键词：`config drift`, `override`, `version`, `load order`
- 期望引用文档：`09_base配置与DB初始化时序图.md`
- 验收标准：答案需指出“对比基线 + 加载顺序核查”两个动作。

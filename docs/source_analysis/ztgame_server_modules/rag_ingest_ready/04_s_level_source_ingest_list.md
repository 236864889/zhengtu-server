# S 级源码文件入库清单

| path | module | domain | recommended_collection | chunk_hint | keywords | reason | 是否存在 |
|---|---|---|---|---|---|---|---|
| `ztgame/BillServer/BillServer.cpp` | `BillServer` | `billing` | `ztgame_server_core` | `init/final/super-msg` | `BillService, init, final, msgParse_SuperService, dbConnPool` | `服务生命周期与总装配主入口` | `是` |
| `ztgame/BillServer/BillTask.cpp` | `BillServer` | `billing` | `ztgame_server_core` | `verify+cmd-dispatch` | `verifyLogin, cmdMsgParse, PARA_GATE_LOGINVERIFY, PARA_SCENE_FORWARD_BILL` | `Gateway/Scene到Bill分发核心` | `是` |
| `ztgame/BillServer/BillUser.cpp` | `BillServer` | `billing` | `ztgame_server_core` | `balance+redeem+restore` | `login, usermsgParseScene, redeem_gold, RESTOREGOLD, BALANCE` | `资金变更与补偿主逻辑` | `是` |
| `ztgame/BillServer/StockConsign.cpp` | `BillServer` | `billing` | `ztgame_server_core` | `consign trade` | `CONSIGNGOLD, CONSIGNMONEY, history, cancelListAll` | `大额资金相关委托交易逻辑` | `是` |
| `ztgame/base/BillCommand.h` | `base` | `billing` | `ztgame_server_core` | `protocol enums` | `CMD_GATE, CMD_REDEEM, t_LoginVerify_Gateway` | `跨服务协议语义锚点` | `是` |
| `ztgame/GatewayServer/GatewayServer.cpp` | `gateway_lifecycle` | `gateway` | `ztgame_server_core` | `init/newTCPTask/notifyLoginServer/msgParse_SuperService/main/final` | `GatewayService, zSubNetService, init, validate, final, newTCPTask` | `服务生命周期与外连拓扑总入口` | `是` |
| `ztgame/GatewayServer/GatewayTask.cpp` | `gateway_conn_state_machine` | `gateway` | `ztgame_server_core` | `verifyConn/waitSync/msgParse/Terminate/forward*` | `verifyVersion, verifyACCID, waitSync, forwardSession, Terminate, anti-flood` | `客户端接入、鉴权、转发、防刷核心` | `是` |
| `ztgame/GatewayServer/SessionClient.cpp` | `gateway_session_link` | `gateway` | `ztgame_server_core` | `connectToSessionServer/run/msgParse` | `CMD_GATE, CMD_FORWARD, PARA_GATE_UNREGUSER, SessionClient` | `Gateway<->Session 双向协议链核心` | `是` |
| `ztgame/GatewayServer/GateUser.cpp` | `gateway_user_state` | `gateway` | `ztgame_server_core` | `reg/unreg/state transitions/passport/checkCmd` | `GateUser, select, login, scene, blacklist` | `玩家态与会话态衔接核心` | `是` |
| `ztgame/GatewayServer/LoginSessionManager.cpp` | `gateway_login_ticket` | `gateway` | `ztgame_server_core` | `put/verify/update` | `loginTempID, DES, sessionData, timeout` | `账号临时票据与密钥校验关键` | `是` |
| `ztgame/MiniServer/MiniServer.cpp` | `lifecycle` | `mini` | `ztgame_server_core` | `init/final/main/reloadConfig/newTCPTask` | `MiniService, zSubNetService, mysql, threadPool, MiniHall` | `服务启动与生命周期总入口，决定可用性` | `是` |
| `ztgame/MiniServer/MiniTask.cpp` | `protocol` | `mini` | `ztgame_server_core` | `verifyConn/cmdMsgParse/parseGateMsg/parseForwardMsg/parseSceneMsg` | `CMD_GATE, CMD_FORWARD, CMD_SCENE, deposit, draw` | `跨 Gateway/Scene 的协议枢纽与资金逻辑` | `是` |
| `ztgame/MiniServer/MiniHall.cpp` | `gameplay_entry` | `mini` | `ztgame_server_core` | `init/parseUserCmd/parseCommonCmd/userEnter/userLeave` | `Mini.xml, room, game, chat, top100` | `小游戏业务入口与大厅状态机` | `是` |
| `ztgame/MiniServer/MiniUserManager.cpp` | `user_state` | `mini` | `ztgame_server_core` | `newUser/removeUserByGatewayID/update` | `MINIGAME, load/save, gateway disconnect` | `用户生命周期与落库主路径` | `是` |
| `ztgame/base/MiniCommand.h` | `protocol_def` | `mini` | `ztgame_server_core` | `CMD/PARA struct definitions` | `PARA_USER_FORWARD_MINI, PARA_SCENE_DEPOSIT, PARA_MINI_FORWARD_USER` | `跨服务协议定义源头` | `是` |
| `ztgame/newquest/quest.lua` | `newquest` | `quest` | `ztgame_server_script_quest` | `入口/生命周期` | `LUA_PATH, enter, quit, die, require` | `全局入口与生命周期函数` | `是` |
| `ztgame/newquest/npc/npc_825.lua` | `newquest` | `quest` | `ztgame_server_script_quest` | `高复杂度任务脚本` | `visit_825, accept_825, show_dialog, 随机任务` | `超大脚本，分支最多` | `是` |
| `ztgame/newquest/define825.lua` | `newquest` | `quest` | `ztgame_server_script_quest` | `公共任务定义` | `visit_npc, state_npc, kill_npc, refresh_npc` | `高复用公共逻辑` | `是` |
| `ztgame/ScenesServer/Quest.cpp` | `ScenesServer` | `quest` | `ztgame_server_script_quest` | `Lua事件桥接` | `execute_script_event, target, offset` | `quest->lua 调用主链` | `是` |
| `ztgame/ScenesServer/ScenesServer.cpp` | `lifecycle` | `scene` | `ztgame_server_core` | `init/final/main/service loop` | `main, init, final, zSubNetService, quest.lua` | `启动和全局依赖总入口` | `是` |
| `ztgame/ScenesServer/SceneTask.cpp` | `protocol_dispatch` | `scene` | `ztgame_server_core` | `cmdMsgParse and gateway/session forward` | `cmdMsgParse, sendCmd, LUA_USERCMD, logout` | `协议分发核心` | `是` |
| `ztgame/ScenesServer/Scene.cpp` | `scene_aoi` | `scene` | `ztgame_server_core` | `changeMap/loadMapFile/sendCmdToNine` | `changeMap, AOI, sendCmdToNine, LoadMap` | `地图与广播核心` | `是` |
| `ztgame/ScenesServer/SceneUser.cpp` | `user_flow` | `scene` | `ztgame_server_core` | `login-enter-map-quit-save` | `SceneUser, changeMap, sendCmdToMe, execute_script_event` | `玩家主状态机` | `是` |
| `ztgame/ScenesServer/SceneNpc.cpp` | `npc_combat` | `scene` | `ztgame_server_core` | `npc ai interact death` | `SceneNpc, summon, sendCmdToNine, script` | `NPC与怪物主行为` | `是` |
| `ztgame/ScenesServer/SkillStatusManager.cpp` | `combat_status` | `scene` | `ztgame_server_risk` | `SkillStatus_*` | `SkillStatus, attack, buff, debuff` | `战斗状态高风险高价值` | `是` |
| `ztgame/ScenesServer/script.h` | `lua_bridge` | `scene` | `ztgame_server_core` | `execute_script_event templates` | `luabind, execute_script_event, lua error` | `脚本桥接入口` | `是` |
| `ztgame/ScenesServer/script.cpp` | `script_bridge` | `script` | `ztgame_server_script_quest` | `LuaVM init + Binder::bind + reloadVM` | `lua_open, luaopen_base, luabind::open, luaL_dofile, newquest/quest.lua` | `脚本桥接核心实现` | `是` |
| `ztgame/ScenesServer/script.h` | `script_bridge` | `script` | `ztgame_server_script_quest` | `execute_script_event templates` | `execute_script_event, call_function, luabind::error, current_user` | `事件调度与异常处理核心` | `是` |
| `ztgame/ScenesServer/ScenesServer.cpp` | `bootstrap` | `script` | `ztgame_server_script_quest` | `createVM/createScriptFromFile/bind/execute` | `createVM, quest.lua, ScriptQuest::sort` | `服务启动脚本加载入口` | `是` |
| `ztgame/newquest/quest.lua` | `lua_entry` | `script` | `ztgame_server_script_quest` | `LUA_PATH + require + enter/quit/die` | `LUA_PATH, require, enter, quit, die` | `Lua总入口` | `是` |
| `ztgame/SessionServer/SessionServer.cpp` | `lifecycle` | `session` | `ztgame_server_core` | `init/final/main/msgParse_SuperService` | `zSubNetService, init, RecordClient, MetaData, DBConnPool, Super` | `启动链路与跨服务关系中枢` | `是` |
| `ztgame/SessionServer/SessionTask.cpp` | `protocol_dispatch` | `session` | `ztgame_server_core` | `cmdMsgParse/msgParse_Scene/msgParse_Gate/msgParse_Forward` | `CMD_SCENE, CMD_GATE, CMD_FORWARD, shutdown, login, user` | `协议分发主入口且风险最高` | `是` |
| `ztgame/SessionServer/SessionTaskManager.h` | `network_routing` | `session` | `ztgame_server_core` | `broadcast/sendCmdToCountry` | `broadcastScene, broadcastGateway, country` | `跨服务路由与广播能力` | `是` |
| `ztgame/SessionServer/SessionManager.cpp` | `session_state` | `session` | `ztgame_server_core` | `UserSessionManager/SceneSessionManager` | `getUserByID, removeAllUserByTask, scene` | `用户/场景会话索引核心` | `是` |
| `ztgame/tools/Makefile` | `tools_build` | `tools` | `ztgame_server_tools_ops` | `targets,deps,libs` | `Makefile, zntop, zebraclientService, mysql_config, xml2-config` | `定义工具链可执行产物和构建依赖，是职责边界核心证据` | `是` |
| `ztgame/tools/zebraclientService.cpp` | `tools_client_load` | `tools` | `ztgame_server_risk` | `main,init,login loop` | `login, gateway, count, mapfile, skilltable` | `批量登录与行为压测主入口，高风险` | `是` |
| `ztgame/tools/zebraclient.cpp` | `tools_client_protocol` | `tools` | `ztgame_server_tools_ops` | `msgParse,connect,operation` | `protocol, RC5, mapscreen, npc, usercmd` | `协议交互核心实现，定位行为副作用必读` | `是` |
| `ztgame/tools/loginClient.cpp` | `tools_login_handshake` | `tools` | `ztgame_server_tools_ops` | `loginLoginServer` | `login, zone, password, gateway` | `账号登录握手与网关信息获取入口` | `是` |
| `ztgame/tools/zntop.cpp` | `tools_ops_monitor` | `tools` | `ztgame_server_tools_ops` | `serviceCallback,update,outfile` | `monitor, ncurses, outfile, interval` | `运维观测工具主程序，含写文件行为` | `是` |
| `ztgame/tools/CollectServerInfo.cpp` | `tools_ops_collect` | `tools` | `ztgame_server_tools_ops` | `getServerInfo,getCpuInfo,getNetStat` | `proc, cpu, net, xml` | `系统信息采集细节来源` | `是` |

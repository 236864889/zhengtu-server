# RecordServer 总览

## 真实职责
- RecordServer 是 **角色档案持久化与角色入口数据服务**：处理 Scene 的读档/写档、Gateway 的建角删角查角名、Session 的用户存在查询。核心分发都在 `RecordTask::msgParse_*`。  
- 不是通用业务 DB 层：没有看到邮件/交易/任务的独立表写入逻辑，更多是由 Scene 打包到 `t_WriteUser_SceneRecord::data` 后统一回写 `CHARBASE + ALLBINARY`。  
- 会话防重入通过 `RecordSessionManager` 做 `accid->(charid,serverid)` 级互斥，约束“同账号同角色并发读写”。

## 编译产物
- 二进制：`RecordServer`。  
- 对象文件：`RecordServer.o RecordTask.o RecordSessionManager.o`。  
- 未进入主构建：`RecordEditor.cpp`（仅工具/离线编辑倾向）。

## 服务入口
- 入口：`RecordServer.cpp::main`，初始化 logger、默认 mysql/log 路径、解析 `RecordServer` 配置后进入 `RecordService::getInstance().main()`。  
- 生命周期核心：`RecordService::init -> zSubNetService::init -> newTCPTask/RecordTask -> final`。

## 继承 base 哪些类
- `RecordService : zSubNetService`。  
- `RecordTask : zTCPTask`。  
- `RecordSessionManager` 使用 `zMutex/zTime` 维护并发会话映射。

## 关键类/文件 Top30
1. `ztgame/RecordServer/RecordTask.cpp`
2. `ztgame/RecordServer/RecordTask.h`
3. `ztgame/RecordServer/RecordServer.cpp`
4. `ztgame/RecordServer/RecordServer.h`
5. `ztgame/RecordServer/RecordSessionManager.cpp`
6. `ztgame/RecordServer/RecordSessionManager.h`
7. `ztgame/base/RecordCommand.h`
8. `ztgame/base/CharBase.h`
9. `ztgame/base/zDBConnPool.h`
10. `ztgame/base/zSubNetService.h`
11. `ztgame/base/zTCPTask.h`
12. `ztgame/base/zService.cpp`
13. `ztgame/base/zDatabaseManager.*`
14. `ztgame/base/zMetaData.cpp`
15. `ztgame/base/Command.h`
16. `ztgame/ScenesServer/RecordClient.cpp`
17. `ztgame/ScenesServer/SceneUser.cpp`
18. `ztgame/ScenesServer/ScenesServer.cpp`
19. `ztgame/SessionServer/RecordClient.cpp`
20. `ztgame/SessionServer/RecommendManager.cpp`
21. `ztgame/GatewayServer/RecordClient.cpp`
22. `ztgame/GatewayServer/GatewayTask.cpp`
23. `ztgame/GatewayServer/CountryInfo.cpp`
24. `ztgame/base/zSocket.cpp`
25. `ztgame/base/zTCPTaskPool.cpp`
26. `ztgame/base/zConfile.cpp`
27. `ztgame/base/Zebra.cpp`
28. `ztgame/RecordServer/Makefile`
29. `ztgame/RecordServer/README.md`
30. `ztgame/Config/*Record*`（待确认实际文件）

## 最高风险文件 Top15
- `RecordTask.cpp`（超长 SQL/协议分发 + 角色核心写档路径）
- `RecordSessionManager.cpp`（并发会话互斥，错误会导致重复写）
- `RecordServer.cpp`（启动/连接池/线程池初始化失败即全挂）
- `base/RecordCommand.h`（跨服务协议一改全链路受影响）
- `ScenesServer/SceneUser.cpp`（写档包体打包与 Record 解耦弱）
- `GatewayServer/RecordClient.cpp`（建角删角回包一致性）
- `SessionServer/RecordClient.cpp`（推荐/存在校验返回）
- 其余 base 网络与 DB 基础层同 base_deep S 级风险。

## 与 Scene/Session/Gateway/Bill 的关系
- Scene：`t_ReadUser_SceneRecord` 读档；`t_WriteUser_SceneRecord` 写档；`t_WriteUser_SceneRecord_Ok` 回包 ack。  
- Gateway：`getSelectInfo/checkName/createChar/delChar/countryOrder` 等角色入口操作。  
- Session：`PARA_CHK_USER_EXIST` 走角色名存在/ID回填（推荐/社交常用）。  
- Bill：Record 内未见直接 Bill 协议处理，关系更多通过 Gateway/Scene 间接体现（待下一轮 BillServer 深挖）。

## RAG 入库优先级
- S：`02/03/04/05/06/07`
- A：`00/01/09/10`
- B：`08`（manifest 为索引文件）

## 适合解决的问题
- 场景服掉线/切图后角色数据是否写回成功。  
- 建角/删角失败的 SQL 与回包断点。  
- 推荐系统“角色是否存在”误判问题。  
- Record 连接池句柄耗尽或慢查询导致服务阻塞。

## 待确认
- `ztgame/Config` 中 Record 专属 XML 键位与默认值覆盖优先级（下一步查 `ztgame/Config` 与部署 conf）。

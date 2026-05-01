# SessionServer 关键词词库

## 启动生命周期
- SessionService, zSubNetService, init, final, main, newTCPTask, Terminate

## 网络连接
- SessionTask, zTCPTaskPool, broadcastScene, broadcastGateway, sendCmdToCountry

## 协议命令
- cmdMsgParse, msgParse_Scene, msgParse_Gate, msgParse_Forward, CMD_SCENE, CMD_GATE, CMD_FORWARD

## 用户会话
- UserSession, SceneSession, UserSessionManager, SceneSessionManager, tempid, removeAllUserByTask

## 跨服务通信
- SuperServer, RecordServer, GatewayServer, ScenesServer, RecordClient, getServerEntryByType

## 配置
- Zebra::global, mysql, ifname, threadPoolCapacity, initThreadPoolState

## 数据库
- zDBConnPool, MetaData, exeSelect, exeInsert, exeUpdate, exeDelete, ALLY, SEPT, BOSSINFO

## 风险排查
- disconnect terminate, shutdown command, GM broadcast, state consistency, concurrency

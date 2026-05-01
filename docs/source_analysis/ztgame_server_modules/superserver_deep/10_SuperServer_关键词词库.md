# SuperServer 关键词词库

## 启动生命周期
- SuperService::main/init/final/newTCPTask
- zNetService::init
- zDBConnPool::putURL

## 服务注册
- PARA_STARTUP_REQUEST
- t_Startup_Response
- uniqueAdd/uniqueRemove

## 服务发现
- ServerEntry
- notifyOther/notifyMe/processSequence
- getServerEntryByType

## ServerList
- SERVERLIST
- TYPE + IP
- EXTIP/EXTPORT

## 各服务通信
- PARA_GATEWAY_GYLIST
- PARA_GATEWAY_NEWSESSION
- PARA_BILL_IDINUSE
- PARA_SESSION_NEWSESSION

## GM广播
- CMD_GMTOOL
- PARA_LOG_GMTOOL
- PARA_PUNISH_GMTOOL

## 全局控制
- PARA_SHUTDOWN
- Terminate
- t_GameTime

## 故障清理
- removeFromContainer
- responseOther
- RoleregCache::timeAction

## 配置
- loginServerListFile
- SuperServer/loginServerList.xml
- ifname/mysql/threadPoolCapacity

## 风险排查
- broadcast 持锁发送
- serverSequence 依赖顺序
- Gateway 下线 FL 列表同步

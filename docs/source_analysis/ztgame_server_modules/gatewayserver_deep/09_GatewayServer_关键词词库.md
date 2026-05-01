# GatewayServer 关键词词库

## 启动生命周期
- GatewayService, zSubNetService, init, validate, final, notifyLoginServer, newTCPTask

## 客户端连接
- GatewayTask, verifyConn, waitSync, recycleConn, addToContainer, Terminate

## Session通信
- SessionClient, CMD_GATE, CMD_FORWARD, PARA_GATE_UNREGUSER, PARA_FORWARD_USER, msgParse_Gate

## 登录验证
- verifyVersion, verifyACCID, loginTempID, LoginSessionManager::verify, t_LoginVerify_Gateway

## 协议命令
- SessionCommand, BillCommand, SuperCommand, PARA_GATE_DELCHAR, PARA_GATE_COUNTRY_ORDER

## 断线清理
- unreg, Logout, removeFromContainer, removeAllUser, accountVerifyOK

## 防刷限流
- sampleInterval, maxSamplePPS, chatInterval, checkUserCmd, alarm, iffy

## 配置
- ifname, initThreadPoolState, verify_client_version, countryorder, rolereg_verify

## 风险排查
- fixed rc5 key, iterator race, half-startup, protocol mismatch, false positive throttling

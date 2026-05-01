# BillServer 关键词词库
- 启动生命周期：`BillService::init` `zSubNetService::init` `BillTimeTick::start` `final`
- Gateway登录验证：`PARA_GATE_LOGINVERIFY` `t_LoginVerify_Gateway_Return` `loginTempID`
- Scene充值消费：`PARA_SCENE_FORWARD_BILL` `usermsgParseScene` `t_Stock_Fetch` `t_Stock_Save`
- 数据库SQL：`BALANCE` `RESTORECARD` `RESTOREGOLD` `CONSIGNGOLD` `exeUpdate` `exeInsert`
- 账号点数：`BillData` `AT_CONSUME` `Bill_action` `BillCache` `tid`
- 事务一致性：`补单` `幂等` `双写` `对账`
- 配置：`mysql` `billServerList` `tradelogfilename` `threadPoolCapacity`
- 风险排查：`重复扣费` `充值未到账` `行锁` `超时回包` `缓存丢失`

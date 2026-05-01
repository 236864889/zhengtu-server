# FLServer 关键词词库

## 启动生命周期
`FLService::init` `zMNetService::bind` `newTCPTask` `FLTimeTick::run` `FLService::final`

## Super通信
`PARA_FL_RQGYLIST` `PARA_GATEWAY_GYLIST` `PARA_SESSION_NEWSESSION` `PARA_BILL_IDINUSE`

## 区服列表
`GYListManager::put` `GYListManager::disableAll` `GYListManager::full_ping_list`

## 网关选择
`GYListManager::getAvl` `wdNumOnline` `state_maintain` `maxGatewayUser`

## tempid会话
`LoginTask::uniqueID` `loginTempID` `LoginManager::add` `LoginManager::verifyReturn`

## 登录入口
`USER_VERIFY_VER` `USER_REQUEST_LOGIN` `jpeg_passport` `ACCSTORE0000`

## 配置
`login_port` `inside_port` `ping_port` `zoneInfoFile` `dbAccessServerListFile` `InfoServer`

## 风险排查
`LOGIN_RETURN_IDINUSE` `LOGIN_RETURN_USERMAX` `LOGIN_RETURN_VERSIONERROR` `disableAll`

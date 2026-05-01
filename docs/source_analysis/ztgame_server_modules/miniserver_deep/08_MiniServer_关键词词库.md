# MiniServer 关键词词库

## 启动生命周期
- MiniService::main
- MiniService::init
- MiniService::final
- MiniTimeTick::start/final
- MiniConfile::parse("MiniServer")
- threadPoolCapacity / initThreadPoolState

## Super注册
- zSubNetService
- server / port
- getServerEntry
- wdServerID / wdServerType
- verifyLogin

## Gateway通信
- CMD_GATE
- PARA_GATE_USER_LOGIN
- PARA_USER_FORWARD_MINI
- PARA_MINI_FORWARD_USER
- GatewayTask::forwardMini
- Gateway MiniClient::msgParse

## 功能玩法
- MiniHall / MiniRoom / MiniGame
- DDZCardGame
- seat / ready / play
- Top100
- stChatCommonMiniGameCmd

## 配置数据库
- Mini.xml
- MINIGAME
- MetaData::getFields
- exeSelect/exeInsert/exeUpdate/exeDelete
- mysql URL

## 风险排查
- recycle_state
- removeUserByGatewayID
- addDBMoney
- draw rollback
- size/data[0]
- scene switch setScene

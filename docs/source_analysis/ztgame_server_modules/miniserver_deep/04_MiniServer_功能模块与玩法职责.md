# MiniServer 功能模块与玩法职责

## 模块分层
- 大厅层：`MiniHall`（登录、房间列表、进出房/进出桌、聊天、请求排行、充值提现请求上送 Scene）。
- 房间层：`MiniRoom`（同房玩家广播、空闲玩家管理）。
- 牌局层：`MiniGame` 抽象 + `DDZCardGame` 实现（发牌、叫分、出牌、自动托管、结算）。
- 用户层：`MiniUser`（状态、积分、money、Scene/Gate task 指针）。
- 排行层：`Top100`（榜单维护与下发）。

## 是否承载小游戏/活动/排行/辅助
- 小游戏：明确承载（斗地主 DDZ）。
- 排行：Top100 明确存在。
- 活动：本目录未见复杂活动调度框架，更像“单模块小游戏服务”。
- 辅助功能：聊天、提示、自动操作（超时自动叫分/出牌）。

## 用户状态与临时数据
- 临时态：座位、ready、play、host、当前回合、倒计时等在内存。
- 持久态：`MiniGameScore` + `money` 存 `MINIGAME` 表。
- 断链清理：Gateway 连接回收时，会批量 `removeUserByGatewayID`。

## 是否写 DB
- 是，且写路径较多：登录读/建档，用户保存，删号，离线充值补写。

## 是否与 Scene 玩法联动
- 有直接联动：Scene 向 Mini 发充值/提现检查与结果回执，Mini 再更新用户 money 并反馈。

## RAG 标签
- `MiniHall`, `MiniGame`, `DDZCardGame`, `Top100`, `MiniUser`, `deposit/draw`

## 适合解决的问题
- 某用户小游戏积分异常。
- 手牌回合错乱/自动出牌争议。
- 榜单更新延迟或缺失。

## 待确认
- 除 DDZ 外是否还有关闭中的玩法（下一步查 Mini.xml 的 type 配置与 MiniRoom 工厂分支）。

# 奖励/道具/经济 API 分布

基于 `ztgame/newquest/**/*.lua` 统计：
- `show_dialog`: 252
- `refresh_npc`: 25
- `add_exp`: 16

结论：该目录偏“任务引导与状态跳转”，显式经济接口调用不多，更多可能封装在绑定函数或 define 脚本。

高风险接口白名单（建议重点审计）
- `add_item/remove_item`
- `add_money/add_bind_money`
- `add_exp`

待确认：`define*.lua` 是否通过间接函数包装了货币/道具发放。

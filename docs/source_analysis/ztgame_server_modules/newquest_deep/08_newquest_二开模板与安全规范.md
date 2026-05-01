# 二开模板与安全规范

## 新增 NPC 模板
1. 新建 `npc_<id>.lua`，定义 `visit_<id>/state_<id>/kill_<id>`。
2. `require` 对应 `define*.lua`，复用公共逻辑。
3. 在 ScriptQuest 注册该 NPC 事件（通过 quest 配置链路）。

## 新增任务模板
- 在 `quests.xml` 增加 event/source；确保 target 函数名与 Lua 一致。

## 新增奖励模板
- 优先通过受控封装函数发放；避免脚本直接散落经济接口。

## 新增传送模板
- 必做 map/x/y 白名单校验 + 等级/状态校验。

## 回滚策略
- 单脚本灰度：先新增文件不挂入口；
- 配置开关：通过 quest event/source 切换；
- 失败回滚：恢复 quests.xml + 移除新增 npc 文件。

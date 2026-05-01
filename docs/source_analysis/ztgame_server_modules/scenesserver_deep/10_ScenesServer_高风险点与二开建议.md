# 高风险点与二开建议
## 高风险
1. 战斗：`SkillStatusManager.cpp` 体量巨大、状态叠加复杂。
2. 地图阻挡：`LoadMap/Scene::loadMapFile` 与客户端地图一致性敏感。
3. Lua：`execute_script_event` 广泛使用，脚本异常可放大。
4. 状态一致性：SceneUser 与 Session/Gateway/Record 多方异步回执。
5. 性能：九屏广播 + 大规模 NPC AI tick。

## 推荐安全改造点
- 建立命令链路表（cmd/para->handler）。
- 给关键链路加审计日志（入场/切图/下线/保存）。
- 对脚本调用加熔断与错误码上送。
- SkillStatus 拆分注册表化。

## 暂不建议改造
- 直接重写战斗公式；
- 直接改协议结构体对齐；
- 未做基线压测前改 AOI 算法。

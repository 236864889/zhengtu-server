# 下一步深挖建议

## 建议下一轮目录
- **优先：`ztgame/Config/`**

## 原因
1. 核心服务目录（Gateway/Session/Scenes/Super/FL/Mini）已基本覆盖，当前缺“配置字段级统一映射”。
2. Mini 的端口、Super 地址、线程池、Mini.xml 路径、DB 连接都依赖配置层，问题根源常在配置而非代码。
3. 需要把各服务 `ServerList/端口/数据库/XML资源` 形成统一矩阵，便于运维与二开基线治理。

## 下一轮建议产物
- `Config_服务启动参数矩阵.md`
- `Config_ServerList_服务发现映射.md`
- `Config_DB连接与库表归属矩阵.md`
- `Config_XML资源引用图谱.md`
- `Config_高风险默认值清单.md`

## Mini 侧待补充验证项
- `MINIGAME` 完整字段定义来源。
- Mini.xml 的 type->玩法工厂映射完整表。
- Super 中 MINISERVER serverType 与实例数量约束。

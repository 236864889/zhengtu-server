# tools 资源与表转换工具分析

## 覆盖面
- XML：`ArrayTable.cpp` 读取 `skilltable.xml`，提取 Skill/Timer/Global。
- Map：`LoadMap.cpp` 读取 `.mps` 二进制头和 tile 数据。
- TBL：未发现。
- NPC/skill/item 显式资源转换器：未发现（仅 skill 行为消费）。
- pack/unpack/convert 命令型工具：未发现独立实现。

## 关键链路
1. `zebraclientService/skill_test_client` 设置默认 `skilltable.xml` + `mapfile`。
2. `ArrayTable` 将 XML 转为内存结构（不落地）。
3. `LoadMap` 将 `.mps` 读取到 `zTiles` 用于移动/寻路测试。

## 风险
- `LoadMap` 的版本校验代码被注释，格式漂移时可能读入脏数据。
- XML 结构变更时 `ArrayTable` 仅日志报错，缺乏强校验/失败策略。
- 编码历史包袱（注释乱码）提示源码可能 GBK/UTF8 混用，后续文本处理需防乱码。

## 适合解决的问题
- “技能压测客户端为何没有按预期施放技能？”
- “地图文件替换后客户端测试路径为何异常？”

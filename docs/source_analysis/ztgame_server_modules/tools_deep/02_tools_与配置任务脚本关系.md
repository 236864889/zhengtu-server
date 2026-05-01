# tools 与 Config/quest/newquest 输入输出关系

## 总结
- 未发现 tools 直接生成/改写 `ztgame/Config/*.xml`。
- 未发现 tools 直接生成/改写 `ztgame/quest/*.xml` 或 `ztgame/newquest/*.lua`。
- tools 与任务/脚本链路是**间接关系**：通过登录/地图/技能协议驱动场景行为，可能触发服务端任务逻辑，但不负责产出任务配置文件。

## 关系拆解
1. `ArrayTable.cpp` 读取 `Zebra::global["skilltable"]`，默认值由测试客户端设为 `skilltable.xml`（tools 本地样例），用于发技能相关包。
2. `LoadMap.cpp` 读取 `mapfile.mps`（默认 `qingyuancun-main.mps`），用于客户端移动/路径测试。
3. `zebraclientService.cpp` 组织批量用户登录并进入网关，属于“行为流量生成器”。
4. 对 `Config/quest/newquest` 的影响停留在“运行时触发服务端逻辑”，不落地修改配置资产。

## 是否依赖 TBL/XML
- 依赖 XML：是（`skilltable.xml`）。
- 依赖 TBL：未发现。
- 依赖 quest XML/newquest Lua：未发现直接文件级读写。

## 是否影响服务启动
- 不参与主启动链路；仅在手工运行时对在线服务形成外部压力/行为输入。

## 待确认
- `skilltable.xml` 是否与主目录 `ztgame/skilltable.xml` 同源版本：待确认（建议对比字段差异）。

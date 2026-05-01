# XML 资源引用图谱

## 资源 → 服务/函数
- `scenesinfo.xml` → Gateway `CountryInfo`（scene/country 展示）
- 地图 `*.mps` + 场景 XML → Scenes `SceneManager/Scene`
- NPC XML / trade XML (`npctrade.xml`) → Scenes NPC/交易模块
- `skilltable.xml` → 技能管理
- `Config/GameConfig.xml` 等 → `GameConfigMgr::init()`
- `Config/MessageSystem.xml` → `MessageSystem::init()`
- `newquest/quest.lua` → `ScriptingSystemLua::createScriptFromFile`
- `zoneInfo.xml` → FL `ServerACL`
- `loginServerList.xml` → Super 的 FL/Rolereg/Info 客户端管理
- `UserServer.xml` / `dbAccessServerList.xml` → FL/Bill 等外部接入配置
- `Mini.xml`：待确认（建议查 Mini 模块读取器）

## 缺失表现
- XML 读取失败通常直接 `init=false` 并退出。
- Lua 缺失：任务系统初始化失败或功能空转。

## 适合解决的问题
- 某玩法开关改了不生效（定位到实际读取文件/函数）。

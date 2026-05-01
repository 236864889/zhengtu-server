# quest 旧任务系统总览
- 真实职责：`ztgame/quest/quests.xml` 维护旧任务定义与事件入口；`on_visit/on_kill/on_use/...` 子目录承载 source XML 动作脚本。运行时由 `ScenesServer/Quest.cpp` 请求分发 + `QuestTable/EventTable` 执行，必要时桥接到 Lua `execute_script_event`。 
- 目录结构：1445 文件，主要是 XML（1443）。高频目录：`on_use`(506)、`on_visit`(379)、`on_kill`(329)、`on_get`(174)。
- quests.xml 总入口：声明为 `encoding="GB2312"`，每个 `<quest>` 的 `<events><event type=... source=.../></events>` 绑定事件。
- source XML 清单（按目录）：`quest/on_visit/*.xml`、`on_kill/*.xml`、`on_use/*.xml`、`on_get/*.xml`、`on_kill_by_level/*.xml`、`on_other/*.xml`、`on_enter/227.xml`、`on_quit/1.xml`、`on_drop/745.xml`、`kill/16610.xml`。
- 被读取的 C++ 事件：`OnVisit/OnUse`（Quest::execute 分支）；另由 QuestEvent/EventTable 体系消费 `on_kill/on_get/on_drop/on_enter/on_quit/on_other`。

## 关键 XML Top30（按链路关键度）
`quests.xml`、`on_enter/227.xml`、`on_visit/5156.xml`、`on_visit/5157.xml`、`on_visit/5158.xml`、`on_visit/5159.xml`、`on_kill/10001.xml`、`on_use/50000.xml`、`on_use/50001.xml`、`on_use/50003.xml`、`on_get/50000.xml`、`on_get/50100.xml`、`on_other/levelup.xml`、`on_quit/1.xml`、`on_drop/745.xml`、`on_kill_by_level/100.xml`、`on_kill/16610.xml`、`kill/16610.xml`、`on_visit/50000.xml`、`on_visit/56000.xml`、`on_visit/56001.xml`、`on_visit/56003.xml`、`on_visit/56004.xml`、`on_visit/56006.xml`、`on_use/202400.xml`、`on_use/202401.xml`、`on_use/202402.xml`、`on_use/202403.xml`、`on_visit/610001.xml`、`on_visit/8052340.xml`。

## 最高风险 XML Top15
1) `quests.xml`（入口单点）
2) `on_visit/5099 - #U9353#Ue21b#U6e70.xml`（异常命名）
3) `on_visit/102-#U9358#U71ba#U5897.xml`（异常命名）
4) `on_visit/5123y.xml`（非纯数字命名）
5) `on_visit/5140bf.xml`（非纯数字命名）
6) `on_kill/0.xml`（保留值风险）
7) `on_other/levelup.xml`（非数值 source）
8) `on_enter/227.xml`（全局进入事件）
9) `on_quit/1.xml`（全局退出事件）
10) `on_drop/745.xml`（唯一 drop 入口）
11) `on_kill_by_level/*.xml`（等级粒度事件漂移）
12) `on_use/2024xxx.xml`（大批活动脚本）
13) `on_visit/805234*.xml`（超长编号专题）
14) `kill/16610.xml`（目录语义与 on_kill 并存）
15) `on_use/100001~100020.xml`（批量道具触发）。

## RAG 入库优先级
- S级：`quests.xml`、`ScenesServer/Quest.cpp`、`ScenesServer/script.h`、`script_deep/04,05`、`scenesserver_deep/08`。
- A级：`on_visit/on_kill/on_use` 中被高频引用 source。
- B级：长尾单任务 XML。

## 适合解决的问题
- “任务按钮点击后到底执行了哪条 source XML？”
- “on_visit 与 Lua target 在 C++ 是怎么分流的？”
- “旧任务改动回滚最小集是什么？”

## 待确认
- 待确认：本仓库未发现 `newquest_deep` 目录，需确认是否在其他分支/挂载路径。

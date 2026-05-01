# RecordServer 角色数据结构与保存边界
- 角色基础属性：直接落 `CharBase -> CHARBASE`。  
- 背包/装备/技能/任务/宠物等：Scene 侧序列化压缩后放 `t_WriteUser_SceneRecord::data`，Record 透明存取。  
- 邮件/交易：本目录未见独立持久化入口，更多由 Session/Bill/Scene 侧其它模块处理（待确认具体表）。  
- 国家/家族/帮会：Record 保留角色字段快照，但关系系统主数据明显在 Session 业务模块中。

## 边界结论
- Record 更像“角色总档案存储服务”，不是所有系统的唯一真源。  
- 二开时要明确“字段型持久化 vs blob型持久化”两条线。

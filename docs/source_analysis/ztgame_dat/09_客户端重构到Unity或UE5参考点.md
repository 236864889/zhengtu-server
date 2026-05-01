# 客户端重构到 Unity 或 UE5 参考点

## 1. 重构目标说明
目标不是“还原旧客户端全部实现细节”，而是：
- 保留既有服务端联动能力（登录、区服、协议、角色/地图/技能ID体系）；
- 用 Unity/UE5 重写表现层（渲染、UI、资源加载、动画特效、音频）；
- 形成可渐进替换的客户端架构（新旧并行验证）。

## 2. 旧客户端可复用部分
1. **服务端协议模型**：`base/Command.h`、`base/MiniUserCommand.h` 及其命令号体系。  
2. **账号与网关链路模型**：`Login -> Gateway/Session/Scenes` 的分层交互逻辑。  
3. **ID 主键体系**：`ObjectBase/NpcBase/SkillBase/地图ID`，是新客户端与服务端对齐的核心。  
4. **业务流程语义**：登录、选角、进场景、移动/战斗/聊天、商城/任务等流程状态机。  
5. **部分数据结构**：TBL/XML 的字段语义（可转换后复用）。

## 3. 必须重写部分
1. **D3D8 渲染链路**：`RenderD3D8 + engine` 的设备/纹理/绘制代码需全部替换为 Unity/UE5 渲染栈。  
2. **旧 GUI 系统**：`Gui*.cpp + interfaces.gl/login.gl/icons.gl` 的控件体系需重建。  
3. **旧资源包读取接口**：`OpenPackFileForRead/PackAggregate/GL` 读法不建议直接搬入新引擎。  
4. **旧特效表现层**：`Magic` 模块的动画、粒子、光照、地图附加表现需重制。  
5. **音频系统**：`Media`（DirectSound/MP3）需用 Unity Audio / UE Audio 重建。  
6. **窗口消息循环**：`Appation/WinMain/MsgProc` 风格应替换为新引擎生命周期。

## 4. 服务端协议复用可能性
- **可以继续沿用**，且应优先沿用：
  - 命令头结构（`byCmd/byParam/subParam`）与长度规则；
  - 登录与重连时序（含 key 切换）；
  - MiniGame 命令子协议。  
- **前提**：新客户端严格按字节序、对齐、字段长度实现编码/解码，保留兼容分支。

## 5. 资源迁移路线
1. **先抽象“逻辑资源ID -> 引擎资源句柄”映射层**（不要直接复制旧 GL/PAK API）。  
2. **PAK/GL 阶段策略**：
   - 阶段A：离线转换工具导出为 PNG/TGA/WAV/JSON（推荐）；
   - 阶段B：新引擎原生资源系统接管（Addressables / AssetManager / PakChunk）。  
3. **UI 资源**：`interfaces.gl/icons.gl/login.gl` 先做图集拆分与命名规范化。  
4. **音频资源**：`sound.pak` 提取后转统一采样格式，重建事件表。

## 6. 表数据迁移路线
- 可迁移为数据资产：
  - `ObjectBase/NpcBase/SkillBase/SkillLevelBase`；
  - 地图配置、NPC 配置、任务文本、UI 文案；
  - 图标帧映射（旧 frame -> 新 atlas sprite）。
- 推荐流程：
  1) 定义中间格式（JSON/Protobuf/CSV）；
  2) 写校验器验证 ID 唯一性、字段完整性；
  3) 生成 Unity ScriptableObject 或 UE DataTable/DataAsset。

## 7. 地图系统迁移参考
- 旧客户端地图表现分散在 `GameScene + Magic/map.h + MapSurf`。
- 迁移建议：
  - 逻辑网格、阻挡、传送点先作为“服务器兼容层数据”；
  - 视觉地形与材质在 Unity Terrain/UE Landscape 重新制作；
  - 先做“逻辑正确 + 简化视觉”的灰盒地图验证。

## 8. 技能与特效迁移参考
- 旧链路关键点：`stAttackMagicUserCmd` -> 场景对象 -> `Magic` 表现系统。
- 迁移原则：
  - 服务端判定继续保留；
  - 客户端只重建表现（技能动画、弹道、命中特效、声音）；
  - 技能ID映射表是第一优先级资产（避免技能错绑）。

## 9. UI 系统迁移参考
- 旧系统可提供：界面功能分区、字段语义、交互流程。
- 新系统建议：
  - Unity：UGUI/UI Toolkit + Addressables；
  - UE5：UMG + DataTable；
  - 以“登录/选角/主HUD/背包/聊天”作为首批页面。

## 10. 推荐最低可行路线 MVP
1. **MVP-1 联机壳**：登录、选角、进场景、角色移动、基础聊天。  
2. **MVP-2 战斗可视**：技能ID映射 + 基础受击/伤害飘字 + 简化特效。  
3. **MVP-3 核心UI**：背包、装备、任务、小地图。  
4. **MVP-4 内容扩展**：商城、社交、活动、MiniGame。  
5. 每阶段都做“新旧客户端抓包对比 + 服务端行为一致性校验”。

## 11. Unity / UE5 分别适合的方向
- **Unity 更适合**：快速迭代、工具链轻、UI改版频繁、移动/多平台探索。  
- **UE5 更适合**：高质量3D表现、复杂场景渲染、后期视觉升级空间。  
- 共同点：都应先建立“协议兼容层 + 数据转换层”，再扩大表现重构。

## 12. 风险与待确认问题
1. 旧 GL/PAK 资源是否可完整无损提取（含特效依赖关系）。  
2. 技能特效脚本与参数是否存在隐式逻辑（仅看表无法还原）。  
3. 地图阻挡与寻路细节是否与服务端完全同构。  
4. 登录器写入字段在全渠道版本中的差异。  
5. MiniGame 协议在实际线上是否有额外扩展字段。

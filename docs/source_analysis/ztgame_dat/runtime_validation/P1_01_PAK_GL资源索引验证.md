# P1-01 PAK / GL 资源索引验证

## 1. 验证目标

在**只做静态源码分析**前提下，确认 ztgame.dat 客户端的运行时资源索引体系：

1. PAK 资源包扫描与索引建立方式；
2. GL 图集加载入口与用途分工；
3. `datas.pak / sound.pak / map.pak / *.gl` 的职责边界；
4. UI、图标、技能特效、地图、声音的读取入口；
5. `OpenPackFileForRead / FindResource / PackAggregate / PackSubStream` 调用关系；
6. 启动、登录、进场景三个阶段的资源依赖差异；
7. Unity / UE5 重构时资源导出优先级。

---

## 2. 搜索范围与方法

### 2.1 重点路径

- `ztgame.dat/engine/source/FilePackHelper.cpp`
- `ztgame.dat/engine/source/PackAggregate.cpp`
- `ztgame.dat/engine/source/PackSubStream.cpp`
- `ztgame.dat/engine/source/BmpTextout.cpp`
- `ztgame.dat/engine/source/PhraseFilter.cpp`
- `ztgame.dat/engine/source/Spell.cpp`
- `ztgame.dat/Client/GameAppation.cpp`
- `ztgame.dat/Client/GameScene.cpp`
- `ztgame.dat/Client/Gui*.cpp`
- `ztgame.dat/Client/table.cpp`
- `ztgame.dat/Media/`
- `ztgame.dat/Magic/`
- `ztgame.dat/PakTest/`
- `ztgame.dat/ViewGL/`
- `ztgame.dat/MergeTga/`

### 2.2 检索方法

- 关键词检索：`OpenPackFileForRead`、`FindResource`、`SetFilePack`、`PackAggregate`、`PackSubStream`、`datas.pak`、`sound.pak`、`map.pak`、`interfaces.gl`、`login.gl`、`items.gl`、`icons.gl`。
- 结合 `OpenPackFileForRead` 调用点反推数据来源（XML/TBL/HZX）。
- 结合 `FindResource` 调用点反推 GL 图集加载时机与用途。
- 核对 `PakTest/ViewGL/MergeTga` 工具源码用途。

### 2.3 结果边界

- 本仓库以源码为主，未包含完整 `data` 运行资源目录；
- 本文结论以源码调用关系为准，不做资源内容解包验证。

---

## 3. 资源体系总览

| 资源类型 | 文件/目录 | 加载入口 | 用途 | 是否启动必需 |
|---|---|---|---|---|
| 核心表与配置 | `data\datas.pak` | `OpenPackFileForRead("data\\datas.pak",...)` | 角色/技能/地图配置、XML、TBL、拼音映射、文本映射 | 是 |
| 声音包 | `data\sound.pak` | `GetSoundManager()->SetFilePack("data\\sound.pak")` | 背景音乐、场景声、环境声 | 否（可降级静音） |
| 地图包（推断） | `map*.pak` | `FilePackHelper` 自动扫描 + 排除规则 | 地图块/地形相关包索引（动态按文件名检索） | 场景阶段必需 |
| UI 主图集 | `data\interfaces.gl` (+ interfaces1~7/cz) | `GetDevice()->FindResource(...)` | 通用 UI 框架、面板元素 | 是 |
| 登录图集 | `data\login.gl`、`data\logo.gl` | `FindResource` / `GuiLogin` | 登录界面背景与控件纹理 | 登录阶段必需 |
| 图标图集 | `data\icons.gl` | `FindResource` + `LoadFaceInfo` | 图标、头像/表情映射 | 是 |
| 物品图集 | `data\items.gl` | `FindResource(...flag_AlwaysOpen...)` | 物品图标、背包/道具显示 | 进游戏后高频必需 |
| 小地图图集 | `data\Minimap.gl` | `FindResource` | 小地图绘制资源 | 场景阶段必需 |
| 其他界面图集 | `data\other.gl` | `FindResource(...flag_AlwaysOpen...)` | 数字/杂项界面图素 | 是 |
| 技能/特效模块 | `Magic/` + `Client/Magic.cpp` | `Magic_CreateMagic` 等 | 技能特效对象创建与渲染 | 场景战斗阶段必需 |
| 文本字形映射 | `datas\BmpText.xml` + `Interfaces.gl` | `CBmpTextout` 构造 | 位图字体字符映射 | 登录/界面文本关键 |

---

## 4. PAK 资源包扫描与读取链路

### 4.1 `data/*.pak` 扫描机制

`FilePackHelper::SearchDirFileList()` 会扫描 `sResBasePath` 下 `*.pak` 与 `*.zip`，并调用 `InitZipPackInfo()` 建立包索引。`isSearchInPack()` 会排除一批“无需全局检索”的包名（如 `interfaces`、`icons`、`items`、`login`、`magic`、`npc`、`terrain` 等），并对 `map` + 数字包也做排除，避免全局名字表过大。说明：客户端采用“**部分包全局索引 + 特定包按需读取**”的混合策略。 

### 4.2 `OpenPackFileForRead` 定位逻辑

核心流程（由 `getPackFileInfo` 与 `OpenResFile/OpenZipFileForRead` 组成）：

1. 先规范化资源路径并尝试在已索引 pack 中匹配；
2. 找不到则回落到物理文件路径（支持非打包文件）；
3. 找到 pack 条目后按 offset/size 打开子流读取。

这使得调用侧可以统一使用 `OpenPackFileForRead("data\\datas.pak", "datas\\xxx")` 方式，无需关心资源在包内还是散文件。

### 4.3 `PackAggregate` 职责

`PackAggregate::openAggregate()` 负责读取 pak 头、校验 `magic/version`、解析文件头区并构建文件条目；`getFileInfo()` 负责读取单文件 detail（包含未压缩大小、CRC、flags）。其实现明确支持 `flagCompress`、`flagEncrypt`、`flagEncryptHeaderOnly`。

### 4.4 `PackSubStream` 职责

`PackSubRStream` 负责包内单文件流读取时的**解压与解密**：

- 压缩路径走 zlib `inflateInit2`；
- 加密路径走 DES 数据解密（`EncDec`）；
- 支持 full-buffer 与 streaming 两种模式。

### 4.5 压缩/加密/CRC

- 压缩：`PackAggregate::flagCompress` + `PackSubStream` inflate。
- 加密：`flagEncrypt/flagEncryptHeaderOnly` + DES key。
- CRC：`PackAggregate` 中计算/保存 CRC；`FilePackHelper` 把 `dwCompressedCRC` 放入索引结构。

### 4.6 缺失资源表现

调用层普遍模式是 `if(!pStream){ Engine_WriteLog(...); return; }` 或 `S_FALSE` 失败返回。即：

- 核心配置缺失：对应子系统初始化失败；
- UI 图集缺失：界面纹理无法显示；
- 登录图集缺失：登录界面显示异常甚至流程中断。

---

## 5. GL 图集加载链路

### 5.1 统一加载入口

`CGameAppation::InitInstance()` 在较早阶段集中调用 `GetDevice()->FindResource("data\\*.gl",...)` 预加载图集，覆盖：

- `interfaces.gl`、`interfaces1~7.gl`、`interfaces_cz.gl`
- `items.gl`（`flag_AlwaysOpen`）
- `icons.gl`
- `login.gl`
- `logo.gl`
- `other.gl`（`flag_AlwaysOpen`）
- `Minimap.gl`

说明 GL 图集在启动阶段建立资源句柄，后续 GUI/场景代码主要按“图集 + 帧号”取图。

### 5.2 登录相关 GL

- `GuiLogin.cpp` 直接设置 `data\logo.gl`；
- 启动预加载含 `login.gl`、`logo.gl`，说明登录界面依赖显式图集文件。

### 5.3 图集帧号引用方式

代码使用 `stResourceLocation("data\\interfaces.gl", group, frame)` 或同类定位，属于“**文件名 + 组/帧索引**”模式。示例：`GameScene.cpp` 中 `FindBitmaps(stResourceLocation("data\\interfaces.gl",4,30))`。

### 5.4 GL 缺失后果

- `interfaces*.gl` 缺失：大量基础 UI 组件贴图缺失；
- `login.gl/logo.gl` 缺失：登录界面不完整或无法正常呈现；
- `icons/items.gl` 缺失：图标/物品展示空白；
- `Minimap.gl` 缺失：小地图显示问题。

---

## 6. 启动阶段资源清单

| 资源 | 调用位置 | 用途 | 缺失后果 | 优先级 |
|---|---|---|---|---|
| `data\sound.pak` | `GameAppation::InitInstance` `SetFilePack` | 声音资源包挂载 | 无声/音效失效 | P1 |
| `data\interfaces.gl` 系列 | `GameAppation::InitInstance` `FindResource` | GUI 核心图集 | UI 大范围缺图 | P0 |
| `data\items.gl` | `GameAppation::InitInstance` | 物品图集常驻 | 道具显示异常 | P0 |
| `data\icons.gl` | `GameAppation::InitInstance` + `LoadFaceInfo` | 图标与表情映射 | 图标/头像异常 | P0 |
| `data\login.gl` / `data\logo.gl` | `GameAppation::InitInstance` | 登录界面素材 | 登录界面异常 | P0 |
| `data\datas.pak` | `LoadFaceInfo`/表加载/文本加载 | 各类核心配置 | 多系统初始化失败 | P0 |

---

## 7. 登录界面资源清单

登录阶段静态可确认重点依赖：

1. `login.gl`：登录 UI 图集；
2. `interfaces.gl`：通用 UI 组件；
3. `datas.pak`：登录流程涉及的 XML/表配置基础；
4. `datas\face.xml`（来自 `datas.pak`）：头像/表情信息；
5. 字体/文本资源：`BmpTextout` 使用 `datas\BmpText.xml` + `Interfaces.gl` 映射位图字。

结论：登录界面不仅依赖 `login.gl`，还依赖共享 UI 图集与 `datas.pak` 中的文本/配置资源。

---

## 8. 游戏内资源清单

### 8.1 明确入口

- `items.gl`：物品图标资源；
- `icons.gl`：技能/状态/通用图标；
- `sound.pak`：场景/环境/音乐；
- `datas.pak`：地图信息（如 `mapsinfo.xml`）、各类 table（技能、NPC、对象等）；
- `Magic` 模块：`Magic_CreateMagic` 等运行时特效对象创建。

### 8.2 代码侧证据

- `table.cpp` 大量 `OpenPackFileForRead("data\\datas.pak", "datas\\tables\\*.tbl")`；
- `GameScene.cpp` 读取 `datas\mapsinfo.xml`；
- 场景中调用 `Magic_CreateMagic` 进行特效实例化；
- 声音系统在运行循环中由 `GetSoundManager()->Run` 驱动。

### 8.3 NPC/怪物/装备外观

源码关键词与模块结构显示其依赖：

- 表配置（`datas.pak`）；
- 图集/模型资源（多为 data 包内，具体实体文件需动态验证）；
- `Magic` 与场景渲染模块联动。

---

## 9. 工具链分析

### 9.1 PakTest

`PakTest/main.cpp` 直接使用 `PackAggregate` 打开 pak，遍历条目并可将包内文件写出，定位是 **PAK 索引/内容检查工具**（开发用途）。

### 9.2 ViewGL

`ViewGL` 工程文档显示其文档类型就是 `*.gl`，并包含 `GrpLib`、保存/导出相关模块，定位为 **GL 图集查看/导出工具**。

### 9.3 MergeTga

`MergeTga` 为独立图像工具工程，命名与模块结构对应 **TGA 合并处理**，用于图集生产链中的贴图合并步骤。

### 9.4 工具职责汇总

- 查看资源：`ViewGL`、`PakTest`；
- 导出资源：`ViewGL`（图集）、`PakTest`（包内文件）；
- 合并图片：`MergeTga`；
- 检查 PAK：`PakTest`；
- 检查 GL：`ViewGL`。

---

## 10. 资源缺失风险

| 缺失资源 | 现象 | 影响阶段 | 是否阻断启动 | 优先级 |
|---|---|---|---|---|
| `datas.pak` | 表/XML无法读取，大量系统初始化失败 | 启动+登录+场景 | 是 | P0 |
| `interfaces.gl` 系列 | 基础 UI 大面积缺图 | 启动+登录+场景 | 高概率阻断可用性 | P0 |
| `login.gl` / `logo.gl` | 登录界面显示异常 | 登录 | 可能阻断登录操作 | P0 |
| `icons.gl` | 图标空白、头像映射失败 | 登录+场景 | 否（但严重影响交互） | P1 |
| `items.gl` | 背包/物品显示异常 | 场景 | 否（功能降级） | P1 |
| `sound.pak` | 无背景音乐/音效 | 启动+场景 | 否 | P2 |
| `Minimap.gl` | 小地图异常 | 场景 | 否（功能降级） | P1 |
| 地图相关 pak | 地图加载失败/黑图 | 进场景 | 是（场景阻断） | P0 |

---

## 11. Unity / UE5 重构影响

### 11.1 优先导出资源

1. `datas.pak` 中结构化表与 XML（技能、地图、NPC、对象、UI配置）；
2. `interfaces*.gl` / `login.gl` / `icons.gl` / `items.gl` / `Minimap.gl`；
3. 地图相关 pak 资源与映射关系；
4. `sound.pak` 音频清单与路径映射；
5. Magic 特效配置与素材引用关系。

### 11.2 是否直接沿用 PAK/GL

不建议在 Unity/UE5 运行时直接沿用旧 PAK/GL 解析链；建议离线转换成中间格式，保留“原路径/原ID/原帧号”映射。

### 11.3 推荐中间格式

- 表：CSV/JSON/SQLite；
- 图集：PNG + Atlas(JSON)；
- 音频：WAV/OGG；
- 地图：引擎可用 tile/chunk/nav 数据；
- 特效：材质+时间轴+参数表（可映射到 VFX Graph/Niagara）。

### 11.4 迁移要点

- UI 图集迁移必须保留原帧号引用映射；
- 地图资源迁移必须保留地图 ID 与逻辑坐标体系；
- 音频保留事件名与触发点对应；
- 技能特效保留“技能ID -> 特效资源 -> 挂点/时序”关系。

---

## 12. 后续动态验证建议

按只读方式执行：

1. 列举 `data` 目录全部 `pak/gl` 文件；
2. 记录启动阶段访问资源（文件名 + 时序）；
3. 记录登录界面访问资源；
4. 记录进场景访问资源；
5. 使用 `PakTest/ViewGL` 做只读索引检查；
6. 生成“功能模块 -> 资源文件 -> 调用入口”依赖表。

---

## 13. 结论与下一步

### 13.1 结论

- 客户端存在清晰的双资源体系：
  - **PAK**：配置/表/地图/声音等结构化与大体量资源；
  - **GL**：UI/图标图集，启动期集中 `FindResource` 预加载。
- `OpenPackFileForRead + PackAggregate + PackSubStream` 构成统一读取栈，支持压缩/加密/CRC 元信息。
- 启动、登录、进场景三个阶段依赖集合不同，但 `datas.pak + interfaces*.gl` 属于高优先级核心。

### 13.2 下一步

- 若资源索引链路已满足当前阶段目标：进入 **P1-02 技能特效链路验证**；
- 若仍存在“包内路径与运行时访问对象”不清楚：新增 **D1-01 资源访问动态追踪方案**。

---

## 禁止事项（执行约束）

- 不要解包资源。
- 不要修改 pak/gl/dat/exe。
- 不要修改源码。
- 不要删除资源。
- 不要生成自动修复代码。
- 不要做空泛总结。

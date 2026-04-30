---
doc_id: rag-module-config
doc_type: module
project: zhengtu-server
module: Config
source_path: ztgame/Config
tags: ['Config', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：Config（v0.2）

## 1. 模块定位
- `Config` 是服务端链路中的独立模块，参与 `ztgame/Makefile` 编译流程（`Config` 为配置模块）。

## 2. 目录职责
- 依据类名可见职责边界：任务处理（`*Task*`）、连接管理（`*Client*`）、状态管理（`*Manager*`）、主进程入口（`Config.cpp`/同名主文件）。

## 3. 核心文件表（至少10个）
| 文件 | 线索 | 用途说明 |
|---|---|---|
| `GameConfig.xml` | 基于文件名/类名 | 作用：GameConfig 相关逻辑（细节待确认） |
| `Mall.xml` | 基于文件名/类名 | 作用：Mall 相关逻辑（细节待确认） |
| `MessageSystem.xml` | 基于文件名/类名 | 作用：MessageSystem 相关逻辑（细节待确认） |
| `12BeastsConfig.xml` | 基于文件名/类名 | 作用：12BeastsConfig 相关逻辑（细节待确认） |
| `FuBenConfig.xml` | 基于文件名/类名 | 作用：FuBenConfig 相关逻辑（细节待确认） |
| `JingMaiConfig.xml` | 基于文件名/类名 | 作用：JingMaiConfig 相关逻辑（细节待确认） |
| `ShenJianConfig.xml` | 基于文件名/类名 | 作用：ShenJianConfig 相关逻辑（细节待确认） |
| `ShengQiConfig.xml` | 基于文件名/类名 | 作用：ShengQiConfig 相关逻辑（细节待确认） |
| `TouxianConfig.xml` | 基于文件名/类名 | 作用：TouxianConfig 相关逻辑（细节待确认） |
| `XiulianConfig.xml` | 基于文件名/类名 | 作用：XiulianConfig 相关逻辑（细节待确认） |
| `ZhengTuZhuanConfig.xml` | 基于文件名/类名 | 作用：ZhengTuZhuanConfig 相关逻辑（细节待确认） |
| `HuishouConfig.xml` | 基于文件名/类名 | 作用：HuishouConfig 相关逻辑（细节待确认） |

## 4. 编译关系
- 由本目录 `Makefile` 产出目标（或配置输入）。
- 服务模块普遍链接 `../base/libzebra.a` 与 `-lencdec`。

## 5. 运行关系
- 通过 `*Client*` 与其他服务互联，具体端口和握手协议待确认。

## 6. 配置依赖
- 依赖 `Config/*.xml`、根目录 XML、任务脚本（`quest/`、`newquest/`）的可能性高；精确映射待确认。

## 7. 常见问题
- 启动顺序错误导致上游连接失败。
- 协议结构改动未同步多服务。
- 配置版本与二进制不匹配。

## 8. 二开入口
- 优先从同名主文件 + `*Task*` + `*Manager*` 三类入口切入。

## 9. 待确认事项
- 默认端口、数据库表映射、线程模型、重连策略待确认。

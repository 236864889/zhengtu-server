---
doc_id: rag-module-config
doc_type: module
project: zhengtu-server
module: Config
source_path: ztgame/Config
tags: ['Config','xml','runtime']
confidence: high
last_verified: 2026-04-30
---
# 模块文档：Config（v0.3）

## 1. 模块定位
- `Config` 是运行期 XML 配置目录，**不是独立服务进程**，也**不产出二进制**。

## 2. 目录职责
- 提供玩法、系统参数、成长线、商城与消息系统等配置数据。
- 由 `ScenesServer`、`SessionServer` 以及其他服务在启动或运行中读取（精确读取点待确认）。

## 3. 核心文件表
| 文件 | 类型 | 主要职责 |
|---|---|---|
| `GameConfig.xml` | 全局配置 | 基础全局参数入口之一。 |
| `Mall.xml` | 商城配置 | 商城条目与行为参数。 |
| `MessageSystem.xml` | 消息配置 | 消息系统规则。 |
| `FuBenConfig.xml` | 副本配置 | 副本类玩法参数。 |
| `JingMaiConfig.xml` | 养成配置 | 经脉相关数值。 |
| `ShenJianConfig.xml` | 装备配置 | 神剑相关参数。 |
| `ShengQiConfig.xml` | 装备配置 | 圣器相关参数。 |
| `XiulianConfig.xml` | 修炼配置 | 修炼成长参数。 |
| `HuishouConfig.xml` | 回收配置 | 资源回收规则。 |
| `12BeastsConfig.xml` | 玩法配置 | 十二兽玩法参数。 |

## 4. 编译关系
- 不参与 `ztgame/Makefile` 的子目录编译链，不产生可执行文件。

## 5. 运行关系
- 作为数据输入层被服务进程读取，尤其与 `ScenesServer` 的玩法逻辑耦合较高。

## 6. 配置依赖
- 与根目录 `scenesinfo.xml`、`skilltable.xml`、`npctrade.xml` 及脚本目录 `quest/`、`newquest/` 共同组成运行期配置面。

## 7. 常见问题
- 配置字段变更但代码未同步，导致加载失败或逻辑偏差。
- 多服务同时依赖同一配置时，版本不一致引发行为分叉。

## 8. 二开入口
- 优先建立“配置项 -> 代码读取点 -> 运行日志”的映射表，再进行改造。

## 9. 待确认事项
- 各 XML 的精确加载时机、热更新能力、失败回退机制待确认。

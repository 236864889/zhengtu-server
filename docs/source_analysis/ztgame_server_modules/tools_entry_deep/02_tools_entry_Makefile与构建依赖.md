# tools Makefile 与构建依赖（静态）

## Makefile 目标
- `ALL_TARGETS = zntop zebraclientService datetime`
- 明确定义额外目标：`base64`
- 未纳入构建：`skill_test_client`（源码存在但当前 Makefile 不产出该二进制）。

## 关键编译/链接参数
- include 路径：`-I../base -I../zise`
- 编译宏：`-D_USE_CMD_NAMESPACE -D_GNU_SOURCE -D_REENTRANT`
- 三方依赖：``mysql_config --cflags/libs_r``、``xml2-config --cflags/libs``、`-llog4cxx -lgd -ljpeg -lncurses`
- 自研依赖：`../base/libzebra.a` 与 `-lencdec`

## 依赖映射（按目标）
- `zntop` <- `zntop.o + CollectServerInfo.o + libzebra + ncurses`
- `zebraclientService` <- `zebraclientService.o + LoadMap.o + loginClient.o + zebraclient.o + zebraclientManager.o + TimeTick.o + ArrayTable.o + libzebra`
- `base64` <- `base64.o + libzebra`
- `datetime` <- 通过 `ALL_TARGETS` 声明（依赖由通用模式规则推导）

## 是否依赖 base/script/mysql/xml2/log4cxx
- base：是（核心必需，`libzebra.a`）。
- script：未在 tools/Makefile 直接声明脚本依赖。
- mysql：编译链接层面依赖（来自 base/公共链接选项），即使某些工具运行路径未显式用 DB。
- xml2：是（`ArrayTable`、`CollectServerInfo` 等走 `zXMLParser`）。
- log4cxx：是（日志系统）。

## 是否可独立编译
- 弱独立：`datetime`/`base64` 逻辑简单，但 Makefile 仍绑定公共链接选项，理论上受构建环境库完整性约束。
- 非独立：`zntop`、`zebraclientService` 明确依赖 base 网络/服务框架与系统库。

## 构建风险
1. 历史依赖链重（mysql/xml2/log4cxx/gd/jpeg/ncurses），新环境缺库概率高。
2. `*.d` 自动依赖文件为历史快照，若头文件路径变化可能失效。
3. `skill_test_client` 未纳入构建目标，易出现“源码存在但产物缺失”的认知偏差。

## 适合解决的问题
- “tools 能否拆成最小可编译子集？”
- “先做 dry-run 时，哪些目标可先从 Makefile 主链解耦？”

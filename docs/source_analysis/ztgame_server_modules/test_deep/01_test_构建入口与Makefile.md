# test 构建入口与 Makefile（静态）

## 是否有 Makefile
- 有：`ztgame/test/Makefile`。

## 目标名
- `ALL_TARGETS`：`fsmtest TCPClient TCPServer log ThreadTest timetest regextest ServiceTest MySqlTest argtest includefiletest entrytest confiletest NewMySqlTest logger`。

## include/库依赖
- 头文件路径：`-I../base`。
- 本地核心库：`../base/libzebra.a`、`-lencdec`。
- 系统/三方：`-lpthread -lz -llog4cxx -lgd -ljpeg`。
- MySQL：`mysql_config --cflags` + `mysql_config --libs_r`。
- XML：`xml2-config --cflags` + `xml2-config --libs`。

## 依赖判断
- 依赖 `base`：是（强依赖）。
- 依赖 `mysql`：是（MySqlTest/NewMySqlTest 明确）。
- 依赖 `libxml2`：是（confile/include xml 样例）。
- 依赖 `log4cxx`：是（logger/log 样例与全局日志设施）。
- 依赖 `script`：未见直接链接脚本库；如需脚本能力应来自 `libzebra` 间接能力（待确认）。

## 是否可独立编译
- 严格来说不能“纯独立”：依赖 `../base` 已构建产物与外部开发库。
- 在现代环境直接编译风险高：mysql/xml2/log4cxx 版本、ABI 与旧代码不兼容概率高。

## 风险
- Makefile 使用历史工具链假设（固定 `/usr/bin/g++`、早期依赖栈）。
- 默认一次性构建全部 target，难以最小化风险。
- 未提供按环境切换的 profile（dev/stage/prod）与凭据隔离。

## RAG 标签 & 适合问题
- 标签：`build-entry` `legacy-makefile` `dependency-map`。
- 适合问题：
  - “test 目录能编译出哪些二进制？”
  - “要跑 test 样例至少要准备哪些基础库？”

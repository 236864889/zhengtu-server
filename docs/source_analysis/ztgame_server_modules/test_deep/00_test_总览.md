# ztgame/test 总览（静态分析）

## 目录职责
- `ztgame/test` 是历史 C++ 基础能力样例集合，不是单一单元测试框架（无 gtest/catch2 结构）。
- 样例覆盖网络、线程、服务循环、MySQL、配置/XML、日志、参数/正则/时间等“base 层 API 用法演示”。

## 文件清单（22 个）
- 构建与说明：`Makefile`、`README.md`、`.NewMySqlTest.d.16634`。
- 网络：`TCPClient.cpp`、`TCPServer.cpp`。
- 线程：`ThreadTest.cpp`、`ThreadPoolTest.cpp`。
- 数据库：`MySqlTest.cpp`、`NewMySqlTest.cpp`。
- 配置/XML：`confiletest.cpp`、`includefiletest.cpp`、`root.xml`、`inc1.xml`、`inc2.xml`。
- 服务与通用：`ServiceTest.cpp`、`entrytest.cpp`、`argtest.cpp`、`fsmtest.cpp`、`regextest.cpp`、`timetest.cpp`、`log.cpp`、`logger.cpp`。

## Makefile/README 状态
- 存在 `Makefile`，并定义 14 个目标；使用 `../base/libzebra.a` 静态库、mysql/xml2/log4cxx 等依赖。
- `README.md` 目前仍是“用途待确认”占位文案，信息较弱。

## 测试类别划分
- 网络链路：`TCPClient` / `TCPServer`（connect/listen/accept/task pool）。
- 线程能力：`ThreadTest`、`ThreadPoolTest`（`zThread`、`zTCPTaskPool`）。
- DB：`MySqlTest`、`NewMySqlTest`（连接池、元数据、SQL 执行）。
- 配置/XML：`confiletest`（`zConfile`），`includefiletest`（`zIncludeXML::expand`）。

## 与 base 的关系（关键调用）
- 服务骨架：`zService`、`zNetService`。
- 网络：`zTCPClient`、`zTCPServer`、`zTCPTask`、`zTCPTaskPool`。
- 线程：`zThread`。
- 配置：`zConfile`、`zXMLParser`、`zProperties`。
- 数据库：`zDBConnPool`、`zMetaData`（经 test 代码使用）。

## 可复用价值
- 可作为后续 Claw/服务器实测前的“模板库”：
  - 网络联调模板：`TCPServer.cpp` + `TCPClient.cpp`。
  - 线程池最小模型：`ThreadPoolTest.cpp`。
  - 配置读取模板：`confiletest.cpp` + `root.xml`。
- 仅建议“代码结构复用”，不建议原样运行。

## 不建议直接运行的原因
- 多个样例写死内网 IP/端口/MySQL URL（如 `192.168.*`），环境不可达概率高。
- DB 样例含建表/插入/更新/删除风险，可能改写真实库。
- 网络/线程样例存在长期循环与阻塞等待，缺少现代超时/隔离策略。

## RAG 标签与适合问题
- 标签：`test-sample` `base-usage` `tcp` `threadpool` `mysql` `xml-config` `legacy-risk`。
- 适合回答：
  - “base 某类最小调用链怎么写？”
  - “早期服务循环/网络任务池是如何组织的？”
- 待确认：`entrytest.cpp`、`fsmtest.cpp` 的业务背景与实际投产关系（建议下一步按命令体系追踪 `base/*Command*.h`）。

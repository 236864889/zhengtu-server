# ztgame/tools 入口脚本补缺专题总览（静态分析）

## 范围与方法
- 范围：`ztgame/tools` 目录内源码、Makefile、README、示例资源文件。
- 方法：仅静态阅读与关键词检索；不编译、不运行、不连库。

## 目录职责
- `tools` 是“运维/压测/辅助工具”混合目录，既有可执行入口（如 `zntop`、`zebraclientService`、`base64`、`datetime`、`skill_test_client`），也有被复用的支持模块（`ArrayTable`、`LoadMap`、`CollectServerInfo` 等）。
- Makefile 当前默认构建目标是 `zntop zebraclientService datetime`，`base64` 需单独 target。`skill_test_client` 未纳入默认构建。

## 文件清单与结构特征
- 当前快照共 34 个文件（含 `*.cpp/*.h/*.xml/*.txt` 与若干历史二进制/依赖文件 `testGame/zntop/zebraclientService/datetime`）。
- README 为“待确认模板文档”，未给出可执行参数、风险、输入输出的可靠说明。

## Makefile / README 状态
- `Makefile`：定义编译链依赖 `../base/libzebra.a` 与 mysql/xml2/log4cxx/ncurses 等系统库；存在自动依赖 `.*.d` 规则。
- `README.md`：文档质量不足，缺少入口索引与安全边界，不适合直接作为运行指南。

## 工具类别划分（入口视角）
1. **系统监控工具**：`zntop.cpp` + `CollectServerInfo.cpp`（采集 /proc 与终端展示，可选写出监控文件）。
2. **协议客户端/压测工具**：`zebraclientService.cpp`、`skill_test_client.cpp`、`zebraclient.cpp`、`loginClient.cpp`（网络登录、建号、改密、角色流程）。
3. **纯算法/格式转换小工具**：`base64.cpp`、`datetime.cpp`。
4. **配置/资源加载支持模块**：`ArrayTable.cpp`（读取 `skilltable.xml`）、`LoadMap.cpp`（读取 `.mps` 地图）。

## 与 base / Config / test 的关系
- 与 `base`：强耦合 `zArg`、`zService`、`zTCPClient*`、`zLogger`、`zXMLParser`、`zConfile` 等基础设施。
- 与 `Config`：tools 未直接读取 `ztgame/Config/*.xml` 作为主配置入口；更偏向读取工具私有配置（`skilltable.xml`、命令行参数、全局变量）。
- 与 `test`：角色相近（历史验证/压测样例），但 `tools` 更接近“准生产运维工具”，风险高于 `test` 的纯样例。

## 入口风险总览
- **高风险**：`zebraclientService`、`skill_test_client`（连接真实登录/游戏服务，可能触发建号、改密、角色创建、持续循环压测）。
- **中风险**：`zntop`（可长时间运行；可写监控输出文件；终端占用）。
- **低风险**：`datetime`、`base64`（本地计算为主，仍需防误参数/误输出）。

## 适合后续问题
- “哪些 tools 可以直接改造成 Claw dry-run？”
- “哪些入口默认就可能触发网络副作用/资源写入？”
- “如何建立 tools 执行前置检查与回滚清单？”

## 待确认
- `testGame` 可执行文件来源与用途（仓库内无同名源码入口）。
- `ro.txt` 在工具链中的实际消费方（未在当前 tools 源码中检索到明确引用）。

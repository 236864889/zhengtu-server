# 配置 / XML 解析样例（静态）

## 覆盖文件
- `ztgame/test/confiletest.cpp`
- `ztgame/test/includefiletest.cpp`
- `ztgame/test/root.xml`、`inc1.xml`、`inc2.xml`

## confiletest.cpp
- 通过 `myConfile : zConfile` 重载解析回调，走 `zConfile` 的统一 parse 流程。
- 体现 `zConfile` + `zXMLParser` 组合在旧项目里的配置读取基础模式。

## includefiletest.cpp + root.xml
- `main()` 中直接调用 `zIncludeXML::expand("root.xml", s)`。
- `root.xml` 再 include `inc1.xml/inc2.xml`（由样例文件名与调用方式可知是 include 展开验证场景）。

## 与 Config_deep 的关系
- `test` 样例更像“配置解析能力单点验证”；
- `Config` 模块 deep 更偏“业务配置体系与部署链路”。
- 两者互补：`test` 证明底层 parser 行为，`Config` 说明生产配置组织。

## 风险
- XML include 在路径解析上依赖当前工作目录，直接运行可能因 cwd 不同失败。
- 若后续扩展到真实配置，需防止 include 环与重复定义。

## RAG 标签 & 适合问题
- 标签：`config-parse` `zconfile` `xml-include` `zxmlparser`。
- 适合问题：
  - “zConfile 在项目中的最小使用方式？”
  - “include XML 在该代码库如何做展开验证？”

# base XML 与配置基础层
## 1. 范围
`zXMLParser zProperties zConfile zIncludeXML`
## 2. 入口
- `zConfile::parse(name)` -> `zXMLParser::initFile` -> root `Zebra` -> `global` + 服务节点。
- `parseNormal/parseSuperServer/parseYour` 分层处理。
- `zIncludeXML::expand` 支持 include 节点递归展开。
## 3. 风险
- 缺节点时 parse 返回 false，导致服务 init/validate 失败。
- include 文件路径错误会静默影响配置完整性。
## 4. RAG
- level: A
- keywords: zConfile::parse,zXMLParser::initFile,zIncludeXML::expand,zProperties

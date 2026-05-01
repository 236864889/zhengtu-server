# 编码、路径与运行风险
- `quests.xml` 明确 GB2312；目录中存在异常文件名（带 `#U`、字母后缀），提示历史转码/平台差异。
- 风险编码：GB2312/GBK/UTF-8 混写、CRLF/LF 混用，可能在 Linux 发布链导致解析差异。

## 路径风险
- `source="xxx.xml"` 依赖 type->目录映射；改 type 不改 source 会失配。
- 相对路径固定在 `ztgame/quest/<type>/`；手工挪文件会产生静默失效。

## 运行时风险
- source 缺失：事件不执行。
- target 缺失：Lua 调用失败返回0。
- 签名不匹配：luabind 捕获异常，日志出现 `CATCHED Luabind EXCEPTION`。
- XML 解析失败：任务不可接/不可交或变量不刷新。

RAG标签：`encoding-risk`, `source-missing`, `target-missing`, `luabind-error`。

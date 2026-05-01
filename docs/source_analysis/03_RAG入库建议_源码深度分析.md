# 征途服务端源码深度分析：RAG 入库建议（v0.3+）

## 建议直接入库（高事实密度）
1. 本目录：
   - `docs/source_analysis/01_服务入口与生命周期.md`
   - `docs/source_analysis/02_跨服务关系_协议_配置_数据库_Lua.md`
2. 既有文档白名单：
   - `docs/rag_sources/02_编译顺序与产物.md`
   - `docs/rag_sources/03_服务端启动顺序.md`
   - `docs/rag_sources/10_已修复编译问题记录.md`
   - `docs/rag_sources/modules/base.md`
   - `docs/rag_sources/modules/script.md`
   - `docs/rag_sources/modules/ScenesServer.md`

## 暂缓入库
- 根 `README.md` 与各目录 `README.md`：仅导航用途。
- 模板化描述较高、缺少函数/类/调用点证据的文档。

## 切片建议
- 按“服务入口生命周期 / 协议命令链 / 配置加载链 / DB调用链 / Lua绑定链”五类切片。
- 每片附加标签：`service`、`command`、`config`、`db`、`lua`、`risk`、`todo_confirm`。

## 待确认优先补齐项
1. 各服务 `validate()` 在 main 级调用顺序与失败处理。
2. Confile 键值（端口、IP、DB）到代码变量的逐项映射。
3. Lua panic 与 userdata 生命周期保护策略。

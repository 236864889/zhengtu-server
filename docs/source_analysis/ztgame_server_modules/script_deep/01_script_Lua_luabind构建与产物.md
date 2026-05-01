# Lua / luabind 构建与产物关系
- Lua 版本判定：`LUA_VERSION_NUM >= 501` 分支存在，且 `lua.h` 接口为 5.1 风格（`lua_open` 宏、`lua_cpcall`）。
- luabind 形态：`ztgame/script/luabind/` 为头文件分发目录（含 detail），未见源码实现/构建脚本。
- 静态库：仓库中未发现 `liblua.a/libluabind.a/liblualib.a`，但 `ScenesServer/Makefile` 明确期望 `-llua -lluabind`。
- Makefile：`ztgame/script/` 未见独立 Makefile；推断库由外部流程预编译后投放至该目录。
- 链接方：已确认 `ScenesServer` 链接 script 目录库。
- ABI 风险：
  1) Lua 5.1/luabind 老版本与新编译器 ABI 兼容性脆弱；
  2) `-D_GNU_SOURCE -D_REENTRANT` + 老 Boost/luabind 模板，升级 GCC 风险高；
  3) 若仅替换头不替换库，易出现符号不匹配。
- 待确认：luabind 对应 Boost 版本锁定关系（下一步查构建镜像或历史 CI 脚本）。

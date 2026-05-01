# 构建与依赖
- Makefile：定义 `-D_USE_EPOLL_ -D_ENCDEC_MSG`，包含 `../base ../script ../zise`。
- 链接：`-lzebra -lencdec -llua -lluabind -lpthread -lz -llog4cxx -lssl -lgd -ljpeg` + `mysql_config/xml2-config`。
- 依赖层次：base(网络/生命周期) > script(luabind) > mysql/xml > Scene业务对象。
- 产物：`ScenesServer`。
- 编译风险：
  1) 旧版 Lua/luabind ABI；
  2) `mysql_config --libs_r` 在新系统可能缺失；
  3) OpenSSL/GD/JPEG 链接顺序敏感；
  4) 超大翻译单元（`SceneUser.cpp`/`SceneNpc.cpp`）导致 OOM/超时。
- 历史修复点（待确认）：`README/构建脚本` 未见标准化容器工具链，建议后续查 CI 或历史提交。

RAG标签：makefile, luabind, mysql_config, xml2-config, ABI风险。

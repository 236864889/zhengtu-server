# SuperServer 构建与依赖

- Makefile 关键项：`ALL_TARGETS=SuperServer`，链接 `-lzebra -lencdec -lpthread -lz -llog4cxx -lgd -ljpeg`，并依赖 `mysql_config/xml2-config`。
- 说明：SuperServer 强依赖 base 静态库（网络/线程/协议/DB）、MySQL 客户端库、libxml2。
- 产物对象包括 `FL/Rolereg/Info` 三套 client manager，表明它不仅是监听服，也主动外连多个上游。
- 编译风险：
  1) `mysql_config --libs_r` 在新系统可能缺失；
  2) `-llog4cxx/-lgd/-ljpeg` 版本不匹配；
  3) 老式 `__gnu_cxx::hash_map` 在新编译器兼容性风险。
- 历史修复关注点：建议沿 base_deep 的编译兼容策略（gcc版本、glibc、mysql client ABI）统一回放。

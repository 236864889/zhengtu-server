# BillServer 构建与依赖
- Makefile 使用 `g++`，包含 `../base`、`../zise`、`mysql_config`、`xml2-config`，链接 `-lzebra -lencdec -lpthread -lz -llog4cxx -lgd -ljpeg`。文件：`ztgame/BillServer/Makefile`。
- 产物：`BillServer`。
- base 依赖：`zSubNetService/zTCPTask/zDBConnPool/zMetaData/zLogger` 等。
- mysql/xml 依赖：编译参数与链接参数均显式启用。
- 编译风险：
  - 强依赖系统 `mysql_config/xml2-config/log4cxx/gd/jpeg`，老环境缺包会失败。
  - 历史 C++ 代码大量 C 风格接口与过时头文件，较新编译器告警/错误风险高。
- 历史修复点（从代码形态推断）：通过 `mysql_config --libs_r` 兼容线程安全 MySQL 客户端，说明曾处理过并发 DB 访问问题。

RAG标签：`build`, `makefile`, `mysql`, `xml2`, `linker`。

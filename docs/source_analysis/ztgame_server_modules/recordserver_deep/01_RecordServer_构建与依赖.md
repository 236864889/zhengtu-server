# RecordServer 构建与依赖
- Makefile 产物 `RecordServer`，编译项含 `mysql_config --cflags` 与 `xml2-config --cflags`，链接 `-lzebra -lencdec -lpthread -lz -llog4cxx -lgd -ljpeg`。  
- base 依赖：`libzebra.a` + `zSubNetService/zTCPTask/zDBConnPool`。  
- mysql/xml 依赖是硬依赖，环境缺失会直接构建失败。  
- 历史编译风险：GNU 扩展 `__gnu_cxx::hash_map`、`mysql --libs_r` 旧参数、C++ 旧 ABI。

## 编译风险与修复点
- `RecordEditor.cpp` 使用老式 MySQL API 与裸指针，若误纳入 OBJS 可能触发额外依赖错误。  
- `RecordTask.cpp` 体量大且宏条件 `_TEST_DATA_LOG` 影响符号。

## RAG 标签
`makefile,mysql_config,xml2-config,libzebra,legacy-toolchain,hash_map`

## 适合解决的问题
- “同代码别的服能编，这台不能编” 的依赖排查。

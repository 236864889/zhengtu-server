# FLServer 构建与依赖

- Makefile 产物：`ALL_TARGETS = FLServer`。
- 编译标志含 `-D_USE_CMD_NAMESPACE -D_ENCDEC_MSG`，并依赖 `mysql_config` 与 `xml2-config`。
- 链接：`-lzebra -lencdec -lpthread -lz -llog4cxx -lgd -ljpeg` + mysql/xml2。
- base 依赖：`zMNetService/zTCPTaskPool/zDBConnPool/zConfile/zXMLParser`。
- mysql/xml 依赖：
  - 登录账号校验直接访问 `ACCSTORE0000`（`LoginTask.cpp`）。
  - `zoneInfo.xml`（ACL）与 `dbAccessServerList.xml`（备用DBAccess）由 XML 读取。
- 编译风险：
  1) 对系统库版本耦合高（mysqlclient_r、log4cxx、gd/jpeg）；
  2) `-D_ENCDEC_MSG` 使登录连接默认 RC5 密钥路径启用；
  3) 旧 GNU ext/hash_map 与 C++ 新编译器兼容风险。
- 历史修复点线索：源码内大量 `//soke` 注释，显示曾有登录校验、角色状态、密码分支修补。

## RAG 标签
`build`, `makefile`, `mysql`, `xml2`, `encdec`, `legacy-toolchain`

## 适合解决的问题
- 为什么 FLServer 在新系统编不过。
- 为什么编译后运行缺少动态库。

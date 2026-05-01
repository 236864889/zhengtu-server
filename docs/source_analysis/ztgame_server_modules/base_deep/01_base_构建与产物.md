# base 构建与产物
## 1. 分析范围
- 文件：`ztgame/base/Makefile`、`ztgame/base/EncDec/Makefile`
## 2. 核心结论
- `libzebra.a` 由 35 个 `.o` 聚合，涵盖网络/线程/日志/DB/XML/Service 核心。
- `libencdec.a` 由 `EncDec/*.o + md5c.o` 归档，`base` 通过 `-lencdec` 链接。
- `libcrypto.a` 在当前 `base` Makefile 未直接构建，属于外部历史依赖待在上层 Makefile 确认。
## 3. 编译顺序
`EncDec/Makefile -> libencdec.a`（被依赖）=> `base/Makefile` 编译 OBJS => `ar cru libzebra.a`。
## 4. 外部依赖
`mysql_config`, `xml2-config`, `pthread`, `log4cxx`, `z`, `gd`, `jpeg`。
## 5. 兼容/风险
- `mysql --libs_r`、`log4cxx`、`gd/jpeg` 在新系统常缺失。
- `EncDec` 同时编译 `md5c.c`（gcc）+ C++ 对象，跨编译器 ABI 需警惕。
## 6. RAG
- level: A
- keywords: Makefile,libzebra,libencdec,OBJS,mysql_config,xml2-config
## 7. 适合问题
- 为什么服务链接失败/符号缺失
- base 编译通过意味着哪些运行基建可用

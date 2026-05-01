# SessionServer 构建与依赖

- 构建入口：`ztgame/SessionServer/Makefile`。
- 产物：`SessionServer`。
- 核心对象：`OBJS` 包含会话核心与大量业务子系统对象，说明 SessionServer 是“聚合型大进程”。

## 链接库
- `-L../base -lzebra -lencdec`
- `-lpthread -lz -llog4cxx -lgd -ljpeg`
- `mysql_config --libs_r`
- `xml2-config --libs`

## 依赖结论
- base 依赖：`zSubNetService/zTCPTask/zTCPClient/Command/日志/线程池`。
- mysql/xml 依赖：编译与运行均强依赖（DB、元数据、XML 配置）。
- script/lua：本目录 Makefile 未见 lua 链接参数。

## 编译风险
- 老式工具链与第三方库耦合深（log4cxx/mysql/xml2/gd/jpeg）。
- OBJS 极长，单进程职责过重；增量改动可能引发大范围回归。

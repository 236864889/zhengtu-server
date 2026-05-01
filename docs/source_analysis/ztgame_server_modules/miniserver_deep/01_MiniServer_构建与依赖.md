# MiniServer 构建与依赖

## Makefile 结论
- 目标产物：`ALL_TARGETS = MiniServer`。
- 关键编译宏：`-D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG -D_ALL_SUPER_GM`。
- 依赖库：`-lzebra -lencdec -lpthread -lz -llog4cxx -lgd -ljpeg + mysql_config + xml2-config`。

## base 依赖
- 通过 `-L../base -lzebra -lencdec` 绑定 base 网络/生命周期/协议封装。
- MiniService 直接使用 `zSubNetService/zTCPTaskPool/zDBConnPool/zMetaData/zConfile`。

## mysql/xml 依赖
- mysql：启动时 `dbConnPool->putURL(mysql)` + `metaData->init(mysql)`，且用户登录/存档/删档都走 `MINIGAME` 表。
- xml：`MiniHall::init()` 读取 `Mini.xml`（root: `Mini`）构建房间/玩法。

## 产物与模块结构
- 核心对象：`MiniServer`（服务壳）+ `MiniTask`（协议）+ `MiniHall/Room/Game`（玩法）+ `MiniUser`（状态）+ `Top100`（排行）。

## 编译风险
- mysql/xml 的 `*_config` 工具若缺失会直接编译失败。
- 运行时还需 `libgd/libjpeg/log4cxx`，老环境部署易遇动态库不匹配。
- C++ 老代码中大量 `char buf[MAX_DATASIZE] + 变长包`，编译虽可过，但后续二开容易引入长度越界风险。

## 历史编译修复点（推断）
- 依赖中已固定 `-D_GNU_SOURCE -D_REENTRANT` 与 epoll 宏，说明线程/socket 兼容是历史痛点。
- Mini 模块链接图像库（gd/jpeg）但当前核心链路未直接看到调用，可能是遗留依赖，建议下轮做“未使用依赖瘦身”排查。

## RAG 标签
- `build`, `makefile`, `mysql_config`, `xml2-config`, `libzebra`, `libencdec`

## 适合解决的问题
- MiniServer 在某些机器上编不过/起不来。
- 升级 gcc 或系统库后链接错误定位。

# GatewayServer 构建与依赖

## Makefile 结论
- 编译器：`/usr/bin/g++`。
- 关键宏：`-D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG -D_WQH_DEBUG -D_ALL_SUPER_GM`。
- 头文件依赖：`../base`, `../zise`, `mysql_config --cflags`, `xml2-config --cflags`。
- 链接依赖：`-lzebra -lencdec -lpthread -lz -llog4cxx -lgd -ljpeg + mysql/xml2`。
- 目标：`GatewayServer`。

## base 依赖
- 静态库：`../base/libzebra.a`、`../base/libencdec.a`。
- 运行时能力依赖 base 的网络、线程、日志、配置、协议头体系。

## mysql/xml 依赖
- 直接通过 `mysql_config`、`xml2-config` 引入，说明 Gateway 虽非核心 DB 服务，但构建期与环境强耦合。

## 产物
- 单可执行文件 `GatewayServer`。

## 编译风险
1. 依赖系统级开发包（mysql/xml2/log4cxx/gd/jpeg），老环境迁移成本高。
2. `-D_ALL_SUPER_GM` 可能改变安全行为（如删角二级口令分支）。
3. 链接顺序与老 GCC 行为绑定，升级工具链有二进制兼容风险。

## 适合解决的问题
- 为什么某台机器能跑另一台不能编译。
- 二开后引入新库导致链接冲突的定位。

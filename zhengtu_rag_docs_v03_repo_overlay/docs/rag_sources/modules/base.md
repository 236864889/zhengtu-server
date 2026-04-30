---
doc_id: rag-module-base
doc_type: module
project: zhengtu-server
module: base
source_path: ztgame/base
tags: ['base', 'rag', 'module']
confidence: medium
last_verified: 2026-04-30
---
# 模块文档：base（v0.2）

## 1. 模块定位
- `base` 是所有服务端进程共享的基础库层，提供网络、线程、日志、数据库、配置、协议与工具类。

## 2. 目录职责
- 产出核心静态库：`libzebra.a`。
- 与 `base/EncDec` 联动提供 `libencdec.a`，并在链接链路中使用 `libcrypto.a`。
- 为上层服务提供 `zTCP*`、`zSocket*`、`zLogger*`、`zDatabase*`、`zXMLParser*` 等通用能力。

## 3. 核心文件表
| 文件 | 依据 | 用途说明 |
|---|---|---|
| `Makefile` | 构建定义 | 产出 `libzebra.a`，并声明 mysql/xml2/log4cxx 等链接参数。 |
| `zSocket.h` | 网络底层 | Socket/ByteBuffer 相关接口与数据结构。 |
| `zSocket.cpp` | 网络底层实现 | 包含收发与错误处理实现。 |
| `zTCPServer.h` | 服务监听 | TCP 服务监听与接入封装。 |
| `zTCPClient.h` | 客户端连接 | 服务间主动连接基础封装。 |
| `zTCPTaskPool.h` | 任务池 | TCP 任务池/并发处理。 |
| `zLogger.h` | 日志接口 | log4cxx 封装与日志宏入口。 |
| `zDatabase.h` | 数据库抽象 | DB 查询与结果处理接口。 |
| `zMysqlDBConnPool.cpp` | 连接池实现 | MySQL 连接池与连接生命周期。 |
| `zXMLParser.h` | XML支持 | 配置/数据 XML 解析入口。 |
| `zConfile.h` | 配置解析 | 配置读取与键值管理。 |
| `Command.h` | 协议基础 | 命令体系基础头。 |

## 4. 编译关系
- `base/EncDec` 先构建，`base` 后构建。
- `base` 通过 `ar` 产出 `libzebra.a`。
- 服务端模块在各自 Makefile 中通过 `-L../base -lzebra -lencdec` 进行链接。

## 5. 运行关系
- `MiniServer/ScenesServer/SessionServer/BillServer/GatewayServer/SuperServer/RecordServer/FLServer/tools/test` 均直接或间接依赖 `base`。

## 6. 配置依赖
- `zConfile`、`zXMLParser` 暗示运行期会读取配置文件；具体配置路径由上层服务决定（待确认）。

## 7. 常见问题
- socket/线程池边界错误导致高并发不稳定。
- DB 连接池参数与数据库负载不匹配。
- log4cxx 版本差异导致编译/运行异常。

## 8. 二开入口
- 网络链路：`zSocket*`、`zTCP*`。
- 数据库链路：`zDatabase*`、`zMysqlDBConnPool*`。
- 配置链路：`zConfile*`、`zXMLParser*`。

## 9. 待确认事项
- `libcrypto.a` 的精确产出路径与构建脚本细节待确认。
- 某些历史兼容宏在不同 GCC/GLIBC 下的行为待确认。

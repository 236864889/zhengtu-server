# 征途私服编译修复记录 (2026-04-03)

## 环境信息
- **服务器**: CentOS 6.5 Final
- **GCC**: 8.3.1 20190311
- **Lua**: 5.1.4
- **Boost**: 1.41.0
- **log4cxx**: 10.0.0
- **MySQL**: 5.1.71

## 编译成果 - 8个服务器全部成功

| 服务器 | 路径 | 大小 |
|--------|------|------|
| SuperServer | /home/zhengtu/ztgame/SuperServer/SuperServer | 4.6M |
| BillServer | /home/zhengtu/ztgame/BillServer/BillServer | 4.8M |
| SessionServer | /home/zhengtu/ztgame/SessionServer/SessionServer | 15M |
| ScenesServer | /home/zhengtu/ztgame/ScenesServer/ScenesServer | 46M |
| GatewayServer | /home/zhengtu/ztgame/GatewayServer/GatewayServer | 6.4M |
| RecordServer | /home/zhengtu/ztgame/RecordServer/RecordServer | 2.8M |
| MiniServer | /home/zhengtu/ztgame/MiniServer/MiniServer | 4.9M |
| FLServer | /home/zhengtu/ztgame/FLServer/FLServer | 4.6M |

## 主要修复内容

### 1. zLogger - 添加setLevel实现
文件: base/zLogger.cpp

### 2. zSocket - 恢复模板定义
文件: base/zSocket.h, base/zSocket.cpp

### 3. GCC扩展语法修复
- SceneUser.cpp: 200 <? rev->size → (200 < rev->size ? 200 : rev->size)
- SceneUser.cpp: wg_log_len <? 4 → (wg_log_len < 4 ? wg_log_len : 4)
- Chat.cpp: define.num >? 1 → (define.num > 1 ? define.num : 1)

### 4. luabind升级到Lua 5.1
- 重新编译luabind库
- 添加Lua 5.1兼容层
- 修复raw(_1)模板错误
- 路径: /root/ztbuild/luabind/

### 5. Makefile修复
- ScenesServer/Makefile: 添加-L../script

## 备份位置
- 原始源码: /root/ztgame1/
- luabind源码: /root/ztbuild/luabind/
- 本地备份: ~/.openclaw/workspace/memory/2026-04-03-zhengtu-compile.md

## 编译命令
cd /home/zhengtu/ztgame && make clean && make

## 验证命令
ldd /home/zhengtu/ztgame/ScenesServer/ScenesServer | grep lua
# liblua-5.1.so => /usr/lib64/liblua-5.1.so

记录时间: 2026-04-03 00:47

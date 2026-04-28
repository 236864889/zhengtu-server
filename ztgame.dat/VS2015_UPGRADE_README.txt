========================================
   征途客户端 VS2015 + Lua 5.1 升级说明
========================================

## 升级内容

1. Lua 版本: 5.0.2 → 5.1.5
2. Visual Studio: 旧版本 → 2015 (v140)
3. luabind: 旧版本 → 0.9.1

## 文件变更

### 新增目录
- lua-5.1.5/          - Lua 5.1.5 头文件和库
- lua-5.0.2.backup/   - 原 Lua 5.0.2 备份
- luabind.backup/     - 原 luabind 备份

### 修改文件
- 所有 .vcproj 文件   - 更新 Lua 引用路径
- 所有 .vcxproj 文件  - 更新平台工具集到 v140

## Visual Studio 2015 配置步骤

### 1. 安装要求
- Visual Studio 2015 (Community/Professional/Enterprise)
- Windows SDK 8.1 或更高版本
- DirectX SDK (June 2010)

### 2. 项目升级步骤

1) 打开解决方案文件
   文件: ztgame.dat.sln (或相应的 .sln 文件)

2) Visual Studio 会提示升级项目
   - 选择升级
   - 平台工具集会自动设置为 v140

3) 修改项目属性
   对每个项目执行:
   
   a) 右键项目 → 属性
   b) 配置属性 → 常规
      - 平台工具集: Visual Studio 2015 (v140)
   
   c) C/C++ → 常规 → 附加包含目录
      - 添加: lua-5.1.5\include
      - 添加: luabind
      - 添加: boost_1_34_1
   
   d) 链接器 → 常规 → 附加库目录
      - 添加: lua-5.1.5\lib

4) 修改 Lua 相关代码兼容性

   需要修改的文件:
   - 所有使用 lua_open() 的文件 → 改为 luaL_newstate()
   - 所有使用 lua_strlen() 的文件 → 改为 lua_objlen()
   - 所有使用 lua_dostring() 的文件 → 改为 luaL_dostring()

   示例修改:
   # 0 "<stdin>"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "<stdin>"

### 3. 编译顺序

1. lua-5.1.5 (liblua)
2. luabind
3. zlib
4. EncDec
5. engine
6. gui
7. RenderD3D
8. Client (主程序)

### 4. 常见问题

Q1: 编译错误 lua_open 未定义
A1: 将 lua_open() 改为 luaL_newstate()

Q2: 编译错误 lua_strlen 未定义
A2: 将 lua_strlen() 改为 lua_objlen()

Q3: 链接错误 无法解析的外部符号
A3: 检查附加库目录是否包含 lua-5.1.5\lib

Q4: boost 相关错误
A4: 确保 boost_1_34_1 路径正确，或使用更高版本 boost

## 与服务端配套

确保以下配置与服务端一致:
- Lua 版本: 5.1
- 网络端口: 7000 (FLServer)
- 加密密钥: 与服务端 EncDec 配置一致
- 协议定义: 使用服务端 Command.h

## 测试步骤

1. 编译 Release 版本
2. 复制到测试环境
3. 修改 config.ini 连接测试服务器
4. 测试登录流程
5. 测试游戏功能

========================================

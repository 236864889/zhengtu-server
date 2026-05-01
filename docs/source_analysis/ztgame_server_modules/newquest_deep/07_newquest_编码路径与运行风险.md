# 编码、路径与运行风险

- `quest.lua`/C++ 注释出现乱码，疑似 GBK/UTF-8 混用。
- 环境缺少 `file` 命令，无法完成自动编码识别（需在具备 file/chardet 的环境复核）。
- `LUA_PATH` 使用相对路径 `./newquest/?.lua`，工作目录变化会导致 require 失败。
- luabind 运行时签名不匹配会抛异常并返回 0（静默业务失败风险）。
- `npc_5021.lua` require `define5021`，仓库未发现同名文件，存在潜在加载缺失。

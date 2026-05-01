# 附录B：常见错误速查表

| 错误关键词 | 所属阶段 | 优先检查文件/命令 | 可能原因 | 建议处理 | 关联手册章节 |
|---|---|---|---|---|---|
| mysql_config: command not found | 依赖检查/编译 | `which mysql_config` | MySQL 开发包未安装 | 补齐开发包并复检 | 第3章 |
| xml2-config: command not found | 依赖检查/编译 | `which xml2-config` | libxml2 开发包缺失 | 安装后重查 | 第3章 |
| cannot find -llog4cxx | 链接 | `ldconfig -p | grep log4cxx` | 库未安装或路径未加载 | 补库并刷新 linker cache | 第3/12章 |
| undefined reference to lua/luabind | ScenesServer 编译 | `ls script/*.a` | Lua/luabind 静态库缺失或顺序错 | 修正库产物与链接顺序 | 第6/12章 |
| No such file *.xml | 启动 | `find Config -name '*.xml'` | 配置缺失/路径错 | 从备份恢复配置集 | 第7/12章 |
| bind: Address already in use | 启动 | `ss -lntp` | 端口占用 | 释放冲突或改端口规划 | 第9/10/12章 |
| Segmentation fault | 启动/运行 | 服务日志、core 文件（若有） | 配置不一致、库ABI冲突 | 回滚到最近可用版本并单点复现 | 第12/13章 |
| connect mysql failed | 启动/运行 | `mysql -h ... -e 'select 1;'` | DB 不通/账号错误/白名单问题 | 核对连通与凭据 | 第8/12章 |
| login failed / timeout | 联调 | `config.ini`、`servers.json`、服务日志 | 地址端口不一致、链路未全起 | 逐层核对配置与服务状态 | 第10/11/12章 |

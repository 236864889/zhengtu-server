# tools 高风险点与使用规范

## 禁止直接运行（生产环境）
- `zebraclientService`, `skill_test_client`, `zebraclient`：会进行真实登录/发包/自动行为，可能干扰在线环境。
- 目录内来源不明二进制（`testGame`, `zntop`, `zebraclientService`, `datetime` 若为预编译件）默认禁止直接执行。

## 运行前备份与隔离要求
- 必须在隔离测试网段、测试账号池执行。
- 需记录目标 `server/port/zone` 和账号范围。
- 启用最小化并发（`count`）逐级放量。

## 读写分级
- 只读型：`CollectServerInfo`, `LoadMap`, `ArrayTable`（对本地文件/系统信息读取）。
- 写文件型：`zntop`（outfile append）。
- 网络写入型：`zebraclient*`, `loginClient`（向服务发包，间接影响线上状态）。
- 写库型：当前源码未见显式 SQL CRUD。

## 路径白名单
- 仅允许读取：`ztgame/tools/skilltable.xml`, `ztgame/tools/*.mps`（测试资源）
- `outfile` 必须限制到临时目录（如 `/tmp/zttools/`），禁止系统关键路径。

## 最小权限与审计
- 非 root 运行。
- 开启命令审计：记录启动命令、参数、执行人、时间、目标 IP/端口。
- 对并发登录测试配置熔断阈值（失败率、连接数、速率）。

## 待确认
- 无扩展名二进制是否可重现构建：待确认（建议校验 `file/sha256` 并统一重编译）。

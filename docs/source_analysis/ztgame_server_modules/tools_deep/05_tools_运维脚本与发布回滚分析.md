# tools 运维脚本与发布回滚分析

## 扫描结论
- 未发现 `*.sh/*.pl/*.py` 运维脚本。
- 运维能力由 C++ 可执行程序承载（主要是 `zntop`）。
- 未发现发布复制、备份恢复、回滚自动化脚本。

## 已有运维能力
- `zntop`：类似 top 的服务器信息展示，并可按配置周期写出观测结果文件。
- `CollectServerInfo`：CPU/网卡/内存/系统信息采集。

## 发布与回滚相关判断
- tools 不参与服务发布流程主链。
- 未提供内建 rollback/backup/restore 机制。
- 无 `rsync/scp/tar/sql dump` 类实现。

## 环境依赖
- Linux `/proc`，ncurses，历史 gcc/lib 版本。
- 若在容器或云主机运行，网卡名与 proc 暴露字段可能变化。

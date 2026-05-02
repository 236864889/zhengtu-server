# 附录D：服务器只读环境检查任务单（v1.0）

> 适用范围：`征途源码编译部署完整操作手册 v1.2` 下一阶段的**服务器/Claw/SSH/人工**真实环境前置检查。
>
> 角色边界：本任务单仅供真实环境执行方使用；Codex 仅接收日志并分析，不执行真实环境动作。

## 1. 任务目标

在目标服务器执行**只读检查**，确认是否具备进入 `make -C base` 编译复验的前置条件。

## 2. 严格限制

### 禁止

- 不要修改源码
- 不要修改配置
- 不要执行 `make`
- 不要启动服务
- 不要运行游戏二进制
- 不要 kill 进程
- 不要写数据库
- 不要导入 RAG
- 不要连接 Qdrant/Embedding

### 允许

- 读取文件
- 检查命令版本
- 检查磁盘
- 检查依赖库
- 检查端口占用
- 检查源码路径
- 保存日志

## 3. 一键检查命令（推荐）

```bash
mkdir -p /tmp/ztgame_env_check
LOG=/tmp/ztgame_env_check/env_check_$(date +%Y%m%d_%H%M%S).log

{
  echo "===== BASIC ====="
  pwd
  whoami
  hostname
  date
  uname -a
  df -h

  echo "===== COMMANDS ====="
  command -v gcc || true
  command -v g++ || true
  command -v make || true
  command -v mysql_config || true
  command -v xml2-config || true
  command -v mysql || true

  gcc --version 2>/dev/null | head -1 || true
  g++ --version 2>/dev/null | head -1 || true
  make --version 2>/dev/null | head -1 || true
  mysql_config --version 2>/dev/null || true
  xml2-config --version 2>/dev/null || true

  echo "===== LIBS ====="
  ldconfig -p 2>/dev/null | egrep 'log4cxx|lua|luabind|libgd|libjpeg|libssl|libcrypto|libz' || true

  echo "===== SOURCE TREE ====="
  ls -lh
  ls -lh ztgame 2>/dev/null || true
  ls -lh Makefile ztgame/Makefile 2>/dev/null || true
  ls -lh start.sh start1.sh stop.sh ztgame/start.sh ztgame/start1.sh ztgame/stop.sh 2>/dev/null || true

  echo "===== SCRIPT HEAD ====="
  head -5 ztgame/start.sh 2>/dev/null || head -5 start.sh 2>/dev/null || true
  head -5 ztgame/start1.sh 2>/dev/null || head -5 start1.sh 2>/dev/null || true
  head -5 ztgame/stop.sh 2>/dev/null || head -5 stop.sh 2>/dev/null || true

  echo "===== KILL RISK ====="
  grep -n "kill -9\|kill" ztgame/stop.sh 2>/dev/null || grep -n "kill -9\|kill" stop.sh 2>/dev/null || true

  echo "===== PORTS ====="
  ss -lntp 2>/dev/null | head -120 || true
  netstat -lntp 2>/dev/null | head -120 || true
} | tee "$LOG"

echo "LOG=$LOG"
```

## 4. 回传最小字段

至少回传以下 8 项：

1. `LOG` 文件路径
2. `gcc/g++/make` 版本
3. `mysql_config/xml2-config` 是否存在
4. 关键库是否能被 `ldconfig` 看到
5. 源码路径结构
6. `start.sh/start1.sh/stop.sh` 前 5 行
7. `stop.sh` 的 `kill` 相关行
8. 端口占用摘要

## 5. Codex 分析判定规则（收日志后）

- 通过：进入 `base` 编译复验任务
- 有条件通过：先补依赖或确认路径
- 不通过：先修环境，不执行编译

#!/bin/bash
set -e

ZT_DIR="/home/zhengtu/ztgame"
LOG="/tmp/zt_fix_continue_$(date +%Y%m%d_%H%M%S).log"

log() {
    echo "[$(date '+%H:%M:%S')] $1" | tee -a $LOG
}

cd $ZT_DIR

# ============================================
# 步骤1: 修复 luabind 库
# ============================================
log "步骤1: 修复 luabind 库..."

# 备份当前的 luabind
cp script/libluabind.a script/libluabind.a.before_fix 2>/dev/null || true

# 使用 GCC8 备份版本
if [ -f "script/libluabind.a.gcc8.bak" ]; then
    cp script/libluabind.a.gcc8.bak script/libluabind.a
    log "已恢复 GCC8 版本的 luabind"
else
    log "警告: 未找到 GCC8 备份版本"
fi

# ============================================
# 步骤2: 修复源码语法问题
# ============================================
log "步骤2: 修复源码语法问题..."

# 修复 base/zSocket.cpp - goto 跨越变量声明
if [ -f "base/zSocket.cpp" ]; then
    if grep -q "int retcode = epoll_wait" base/zSocket.cpp 2>/dev/null; then
        log "修复 base/zSocket.cpp..."
        sed -i 's/int retcode = epoll_wait(/int retcode;\n    retcode = epoll_wait(/g' base/zSocket.cpp
    fi
fi

# 修复 base/zAStar.h - 模板参数遮蔽
if [ -f "base/zAStar.h" ]; then
    if grep -q "const int radius = radius" base/zAStar.h 2>/dev/null; then
        log "修复 base/zAStar.h..."
        sed -i 's/const int radius = radius/const int radius_val = radius/g' base/zAStar.h
        sed -i 's/const int step = step/const int step_val = step/g' base/zAStar.h
    fi
fi

# 修复 SessionServer/Session.h
if [ -f "SessionServer/Session.h" ]; then
    if grep -q "SceneSession::SceneSession" SessionServer/Session.h 2>/dev/null; then
        log "修复 SessionServer/Session.h..."
        sed -i 's/SceneSession::SceneSession(/SceneSession(/g' SessionServer/Session.h
        sed -i 's/bool SceneSession::reg(/bool reg(/g' SessionServer/Session.h
    fi
fi

# 修复 SessionServer/SessionTask.h
if [ -f "SessionServer/SessionTask.h" ]; then
    if grep -q "bool SessionTask::change_country" SessionServer/SessionTask.h 2>/dev/null; then
        log "修复 SessionServer/SessionTask.h..."
        sed -i 's/bool SessionTask::change_country(/bool change_country(/g' SessionServer/SessionTask.h
    fi
fi

# 修复 ScenesServer/zObject.h - 析构函数访问权限
if [ -f "ScenesServer/zObject.h" ]; then
    if grep -A5 "private:" ScenesServer/zObject.h 2>/dev/null | grep -q "~zObject"; then
        log "修复 ScenesServer/zObject.h..."
        sed -i 's/private:/public:/g' ScenesServer/zObject.h
    fi
fi

# ============================================
# 步骤3: 修复 Makefile 链接顺序
# ============================================
log "步骤3: 修复 Makefile 链接顺序..."

for mf in $(find . -name "Makefile" -type f); do
    if [ -f "$mf" ]; then
        # 修复 -llog4cxx 在 -L 之前的问题
        if grep -q "\-llog4cxx.*-L" "$mf" 2>/dev/null; then
            log "修复 $mf..."
            sed -i 's/-llog4cxx \(.*\)-L/-L\1 -llog4cxx /g' "$mf"
        fi
    fi
done

# ============================================
# 步骤4: 清理并重新编译
# ============================================
log "步骤4: 清理并重新编译..."
make clean >> $LOG 2>&1

log "开始编译所有服务..."
make 2>&1 | tee -a $LOG

# ============================================
# 步骤5: 验证编译结果
# ============================================
log "步骤5: 验证编译结果..."

SERVERS="SuperServer BillServer SessionServer ScenesServer GatewayServer RecordServer MiniServer FLServer"
SUCCESS=0
FAILED=0

for server in $SERVERS; do
    if [ -f "$server/$server" ]; then
        size=$(du -h "$server/$server" | cut -f1)
        log "✅ $server 编译成功 ($size)"
        SUCCESS=$((SUCCESS + 1))
    else
        log "❌ $server 编译失败"
        FAILED=$((FAILED + 1))
    fi
done

log "============================================"
log "编译完成: $SUCCESS 成功, $FAILED 失败"
log "详细日志: $LOG"
log "============================================"

exit $FAILED

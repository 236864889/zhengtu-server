#!/bin/bash
set -e

ZT_DIR="/home/zhengtu/ztgame"
LOG="/tmp/zt_fix_$(date +%Y%m%d_%H%M%S).log"

log() {
    echo "[$(date '+%H:%M:%S')] $1" | tee -a $LOG
}

cd $ZT_DIR

# ============================================
# 步骤1: 修复 log4cxx - 降级到 0.9.7
# ============================================
log "步骤1: 检查并修复 log4cxx..."

if ldconfig -p | grep -q "liblog4cxx.so.10"; then
    log "检测到 log4cxx 0.10.x，需要降级到 0.9.7"
    
    cd /tmp
    if [ ! -f "apache-log4cxx-0.9.7.tar.gz" ]; then
        log "下载 log4cxx 0.9.7..."
        wget -q http://archive.apache.org/dist/logging/log4cxx/0.9.7/apache-log4cxx-0.9.7.tar.gz ||         curl -sL http://archive.apache.org/dist/logging/log4cxx/0.9.7/apache-log4cxx-0.9.7.tar.gz -o apache-log4cxx-0.9.7.tar.gz
    fi
    
    if [ -d "apache-log4cxx-0.9.7" ]; then
        rm -rf apache-log4cxx-0.9.7
    fi
    
    tar xzf apache-log4cxx-0.9.7.tar.gz
    cd apache-log4cxx-0.9.7
    
    log "配置 log4cxx..."
    ./configure --prefix=/usr/local --disable-doxygen >> $LOG 2>&1
    
    log "编译 log4cxx..."
    make -j4 >> $LOG 2>&1
    
    log "安装 log4cxx..."
    make install >> $LOG 2>&1
    ldconfig
    
    log "log4cxx 0.9.7 安装完成"
else
    log "log4cxx 版本检查通过"
fi

# ============================================
# 步骤2: 重新编译 luabind (适配 Boost 1.41)
# ============================================
log "步骤2: 重新编译 luabind..."

# 先备份当前的 luabind
cd $ZT_DIR
cp script/libluabind.a script/libluabind.a.before_fix 2>/dev/null || true

# 检查是否有 luabind 源码
if [ -d "/home/zhengtu/ztgame.dat/luabind" ]; then
    log "找到 luabind 源码，重新编译..."
    cd /home/zhengtu/ztgame.dat/luabind
    
    # 清理
    make clean 2>/dev/null || true
    
    # 修改 makefile 使用系统 Boost 1.41
    if [ -f "src/Makefile" ]; then
        log "修改 luabind makefile..."
        sed -i 's/-I\.\./-I\.\. -I\/usr\/include/g' src/Makefile
        sed -i 's/\-I-/ /g' src/Makefile
    fi
    
    # 编译
    log "编译 luabind..."
    cd src
    make 2>&1 | tee -a $LOG || {
        log "luabind 编译失败，尝试使用现有库..."
    }
    
    if [ -f "libluabind.a" ]; then
        cp libluabind.a $ZT_DIR/script/libluabind.a
        log "luabind 编译成功并复制到 script/"
    fi
else
    log "未找到 luabind 源码，使用现有 GCC8 备份..."
    if [ -f "$ZT_DIR/script/libluabind.a.gcc8.bak" ]; then
        cp $ZT_DIR/script/libluabind.a.gcc8.bak $ZT_DIR/script/libluabind.a
        log "已恢复 GCC8 版本的 luabind"
    fi
fi

# ============================================
# 步骤3: 修复源码语法问题
# ============================================
log "步骤3: 修复源码语法问题..."

cd $ZT_DIR

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
# 步骤4: 修复 Makefile 链接顺序
# ============================================
log "步骤4: 修复 Makefile 链接顺序..."

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
# 步骤5: 清理并重新编译
# ============================================
log "步骤5: 清理并重新编译..."
cd $ZT_DIR
make clean >> $LOG 2>&1

log "开始编译所有服务..."
make 2>&1 | tee -a $LOG

# ============================================
# 步骤6: 验证编译结果
# ============================================
log "步骤6: 验证编译结果..."

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

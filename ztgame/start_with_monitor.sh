#!/bin/bash
# 服务器启动脚本（带监控）

export LD_LIBRARY_PATH=/usr/local/lib:
cd /home/zhengtu/ztgame

rm -f /tmp/*.log*
mkdir -p tmp

echo '========================================'
echo '    启动征途私服服务器（带监控）'
echo '========================================'

# 启动函数
start_server() {
    local name=
    local dir=
    local delay=
    echo 启动 ...
    cd /home/zhengtu/ztgame/
    ./ > /home/zhengtu/ztgame/tmp/.log 2>&1 &
    sleep 
    if ps aux | grep ./ | grep -v grep > /dev/null; then
        echo ✅ 启动成功
        return 0
    else
        echo ❌ 启动失败
        return 1
    fi
}

# 按顺序启动
start_server FLServer FLServer 5
start_server SuperServer SuperServer 5
start_server RecordServer RecordServer 5
start_server BillServer BillServer 5
start_server MiniServer MiniServer 5
start_server SessionServer SessionServer 10
# ScenesServer 暂时禁用（崩溃问题）
echo ⏭️ ScenesServer 暂时跳过（崩溃问题）
start_server GatewayServer GatewayServer 10

echo ''
echo '========================================'
echo '启动完成'
echo '运行中的服务器:'
ps aux | grep -E './FLServer|./SuperServer|./RecordServer|./BillServer|./MiniServer|./SessionServer|./GatewayServer' | grep -v grep | wc -l
echo '运行中的服务器缺少ScenesServer:'
echo '========================================'

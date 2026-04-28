#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
cd /home/zhengtu/ztgame
rm -f /tmp/*.log*
mkdir -p tmp

echo '========================================'
echo '    启动征途私服服务器'
echo '========================================'

echo '1. 启动 FLServer...'
./FLServer/FLServer > tmp/flserver.log 2>&1 &
sleep 5

echo '2. 启动 SuperServer...'
./SuperServer/SuperServer > tmp/superserver.log 2>&1 &
sleep 5

echo '3. 启动 RecordServer...'
./RecordServer/RecordServer > tmp/recordserver.log 2>&1 &
sleep 5

echo '4. 启动 BillServer...'
./BillServer/BillServer > tmp/billserver.log 2>&1 &
sleep 5

echo '5. 启动 MiniServer...'
./MiniServer/MiniServer > tmp/miniserver.log 2>&1 &
sleep 5

echo '6. 启动 SessionServer...'
./SessionServer/SessionServer > tmp/sessionserver.log 2>&1 &
sleep 10

echo '7. ScenesServer - 暂时跳过（崩溃问题）'
# ./ScenesServer/ScenesServer > tmp/scenesserver.log 2>&1 &
# sleep 10

echo '8. 启动 GatewayServer...'
./GatewayServer/GatewayServer > tmp/gatewayserver.log 2>&1 &
sleep 10

echo ''
echo '========================================'
echo '启动完成! 运行中的服务器:'
ps aux | grep -E './FLServer|./SuperServer|./RecordServer|./BillServer|./MiniServer|./SessionServer|./GatewayServer' | grep -v grep | wc -l
echo '========================================'

#!/bin/bash
# 征途服务端 GCC 8 + Lua 5.1 升级脚本
set -e

ZT_DIR="/home/zhengtu/ztgame"
cd $ZT_DIR

echo "=== 步骤1: 备份关键文件 ==="
cp script/libluabind.a script/libluabind.a.backup 2>/dev/null || true

echo "=== 步骤2: 恢复正确的 luabind ==="
if [ -f "script/libluabind.a.gcc8.bak" ]; then
    cp script/libluabind.a.gcc8.bak script/libluabind.a
    echo "? 已恢复 GCC8 版本的 luabind"
fi

echo "=== 步骤3: 检查 log4cxx ==="
if ldconfig -p | grep -q "liblog4cxx.so.10"; then
    echo "??  检测到 log4cxx 0.10.x，建议降级到 0.9.7"
    echo "   执行: cd /tmp && wget http://archive.apache.org/dist/logging/log4cxx/0.9.7/apache-log4cxx-0.9.7.tar.gz"
fi

echo "=== 步骤4: 清理编译 ==="
make clean

echo "=== 步骤5: 开始编译 ==="
make 2>&1 | tee /tmp/zt_build.log

echo "=== 编译结果 ==="
for s in SuperServer BillServer SessionServer ScenesServer GatewayServer RecordServer MiniServer FLServer; do
    if [ -f "$s/$s" ]; then
        echo "? $s"
    else
        echo "? $s"
    fi
done
